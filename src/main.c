#include <stdio.h>
#include <ncurses.h> // download it on linux pls if u dont hav it
#include <unistd.h> // Required library
#include "daemon_check.h"

#include <signal.h>
#include <unistd.h>

#include <pthread.h>
#include <stdatomic.h>

static volatile sig_atomic_t running = 1;

static Vector*          g_snap = NULL;
static pthread_mutex_t  g_lock = PTHREAD_MUTEX_INITIALIZER;
static atomic_bool      g_run  = 1;


static void on_sigint(int sig){
    (void)sig;
    running = 0;
}

static void free_daemon(void* daemon){
    struct daemon* dmon = *(struct daemon**)daemon;
    if(!dmon) return;
    free(dmon->pid);
    free(dmon->name);
    free(dmon->description);
    free(dmon->is_running);
    free(dmon);
}

static void* scanFiles(void* arg){
    (void)arg;

    while(atomic_load(&g_run)){
        Vector* fresh = get_all_daemons();
        pthread_mutex_lock(&g_lock);
        Vector* old = g_snap;
        g_snap = fresh;
        pthread_mutex_unlock(&g_lock);
        if(!old) continue;
        destroy_vec_all(old, free_daemon);
        usleep(200000);
    }
    return NULL;
}

int main() {
    struct daemon* current_choice;
    signal(SIGINT, on_sigint);
    initscr();              // Start ncurses mode.
    noecho();               // Don't echo user input.
    cbreak();               // Disable line buffering.
    curs_set(0);            // Hide the cursor.
    keypad(stdscr, TRUE);   // Enable special keys (arrow keys are special keys).
    timeout(3000);

    pthread_t th;
    pthread_create(&th, NULL, scanFiles, NULL);
    // yeah, I js copied it from docs 
    int yMax, xMax;
    
    int y = 0;
    int yoffs = 0;
    int ch;
    while(running){
        erase();
        getmaxyx(stdscr, yMax, xMax);
        int rows = yMax - 1;
        pthread_mutex_lock(&g_lock);
        if(g_snap){
            mvprintw(0, 0, "%-8s        %s", "PID", "NAME"); 
            current_choice = *(struct daemon**)get_from_vec(g_snap, y); 
            for(int i = yoffs; i < rows + yoffs && (size_t)i < g_snap->size; ++i){
                struct daemon* d = *(struct daemon**)get_from_vec(g_snap, i);
                if(!d) continue;
                mvprintw(i + 1 - yoffs, 0, "%-8s        %.*s", d->pid, xMax - 16, d->name);
                if(d == current_choice){
                    printw(" <*}");
                }
            } 
        }
        ch = getch();       
        switch (ch)
        {
            case KEY_UP:
                if(y > 0){
                    y--;
                    if(y == yoffs - 1){
                        yoffs--;
                    }
                    
                } 
                break;
            case KEY_DOWN:
                if(y + rows < g_snap->size)
                { 
                    y++;
                    if(y == rows + yoffs){
                        yoffs++;
                    }
                }
                break;
            case KEY_LEFT:
                //
                break;
            case KEY_RIGHT:
                //
                break;
        }    
        pthread_mutex_unlock(&g_lock);      
        if ('q' == ch) { running = 0; }
        refresh();
    }   
    endwin();
    atomic_store(&g_run, false);
    pthread_join(th, NULL); 
    return 0;
}