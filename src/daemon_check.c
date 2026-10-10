#include "daemon_check.h"

Vector* get_all_daemons(){
    Vector* dirArray = create_vec(sizeof(struct daemon*)); // initialzing vector for daemon list
    DIR* pidDirectory = opendir("/proc"); // Function that opens the dest directory
    if(!pidDirectory){ // if pointer is NULL, return ERROR instead of vec
        perror("opendir");
        return NULL;
    }

    struct dirent *entry; // directory entry...

    while ( (entry = readdir(pidDirectory) ) != NULL) // readdir is actually reads directory, iterating through all of it. returns NULL if end.
    {
        if(is_number(entry->d_name)){
            struct daemon *dmon = calloc(1, sizeof(*dmon));  // daemon memory allocation
            char path[512]; // path of the proccess id
            snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name); // copy path to the 'path'

            FILE* daemonFileInformation = fopen(path, "rb"); // open file in read + binary

            char line[256];
            if(daemonFileInformation){
                dmon->pid = strdup(entry->d_name);
                while(fgets(line, sizeof(line), daemonFileInformation)){
                    if(strncmp(line, "Name:", 5) == 0){
                        char* v = line + 5;
                        v += strspn(v, " \t");        // skip all tabs 
                        v[strcspn(v, "\n")] = '\0';   // remove '\n'
                        dmon->name = strdup(v);
                        continue; 
                    }
                    if(strncmp(line, "State:", 6) == 0){
                        char* v = line + 5;
                        v += strspn(v, " \t");        
                        v[strcspn(v, "\n")] = '\0';   
                        dmon->is_running = strdup(v);
                        break;                        
                    } 
                }
            }
            else{
                free(dmon);
                continue;
            }
            fclose(daemonFileInformation);
            push_back(dirArray, &dmon);
        }
    }
    closedir(pidDirectory);
    return dirArray;
}