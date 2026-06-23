//このプログラムには日本語が含まれます。←By this line, compilers will detect the charset.
#define __PROGRAM_VER__ "1.1.2"
/*
 * AhoBench - A very simple benchmark program for any devices.
 * 
 * Copyrignt (C) 2026 TLFoxHuman (TlFoxHuman, TranslucentFoxHuman, 半狐, 半透明狐人間)
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")

unsigned long long current_time = 0;
unsigned long long end_time = 0;

unsigned long long* score_list;

int *idlist;

int cores = 0;
int duration = 10;

pthread_t *thread_list;

void* timerthread(void* arg) {
    struct timespec ts;
    while (current_time < end_time) {
        clock_gettime(CLOCK_MONOTONIC, &ts);
        current_time =  ((unsigned long long)ts.tv_sec * 1000000000ull + (unsigned long long)ts.tv_nsec);
    }
    return NULL;
}

void* benchmark(void* arg) {
    int thread_id = (int)arg;
    unsigned long long a = 0;


    while (current_time < end_time) {
        char *test_string = (char*)malloc(sizeof(char)*64);
        char *test_string_2 = (char*)malloc(sizeof(char)*64);
        strcpy(test_string,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
        test_string[26] = 0;
        strcat(test_string,"123456789abcdefghijklmnopqrstuvwxyz");
        strcpy(test_string_2,test_string);
        strcpy(test_string,test_string_2);
        free(test_string);
        free(test_string_2);
        a += 1;
    }

    score_list[thread_id] = a;
    return NULL;
}

int get_cpucore() {
    #ifdef __WIN32
        return 8;
    #else
        FILE *fp;
        char cores_text[8];
        fp = popen("/bin/nproc","r");
        if (fp == NULL) {
            return 8;
        }
        fgets(cores_text,sizeof(cores_text),fp);
        pclose(fp);
        return atoi(cores_text);
    #endif
}

unsigned long long get_best(unsigned long long* scores,int core_count) {
    unsigned long long resval = scores[0];
    for (int i=1;i<core_count;i++) {
        if (resval < scores[i]) {
            resval = scores[i];
        }
    }
    return resval;
}

unsigned long long get_worst(unsigned long long* scores,int core_count) {
    unsigned long long resval = scores[0];
    for (int i=1;i<core_count;i++) {
        if (resval > scores[i]) {
            resval = scores[i];
        }
    }
    return resval;
}

void print_help(char *progcmd) {
    printf ("AhoBench %s\n"
            "\n"
            "Usage:\n"
            "%s [-j <THREADS>] [-d <DURATION>]\n"
            "Options:\n"
            "-d           : Specify the number of duration in seconds. (Default: 10)\n"
            "-j           : Specify the number of threads to run benchmark. (Default: %d)\n"
            "-v --version : Print version information.\n"
            "-h --help    : Print this help.\n",__PROGRAM_VER__,progcmd,get_cpucore());
}

void print_ver() {
    printf("AhoBench C\nVersion : %s\n\n"
           "Copyrignt (C) 2026 TLFoxHuman (TlFoxHuman, TranslucentFoxHuman, 半狐, 半透明狐人間)\n\n"
           "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n"
			"\n"
		   "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n"
			"\n"
			"You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.\n",__PROGRAM_VER__);
}


int main(int argc,char** argv) {
    struct timespec ts;
    pthread_t timerThreadID;
    unsigned long long score = 0;

    if (argc >= 2) {
        for (int i=1;i<argc;i++){
            if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0 || strcmp(argv[i],"-?") == 0) {
                print_help(argv[0]);
                return 0;
            } else if (strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"--version") == 0 ) {
                print_ver();
                return 0;
            } else if (strcmp(argv[i],"-d") == 0) {
                if (argc == i+1) {
                    fputs("Missing option for -d\n",stderr);
                    return 1;
                }
                duration = atoi(argv[i+1]);
                i++;
            } else if (strcmp(argv[i],"-j") == 0) {
                if (argc == i+1) {
                    fputs("Missing option for -j\n",stderr);
                    return 1;
                }
                cores = atoi(argv[i+1]);
                i++;
            } else {
                fprintf(stderr,"Unrecognized option: %s\n",argv[i]);
                return 2;
            }
        }
    }

    if(cores == 0) {
        cores = get_cpucore();
        if (cores == 0) {
            fprintf(stderr,"Error: Failed to get the number of threads. Please specify the number of threads manually with -j option.\n");
            return 3;
        }
    }

    thread_list = (pthread_t*)malloc(sizeof(pthread_t)*cores);
    score_list = (unsigned long long*)malloc(sizeof(unsigned long long)*cores);
    idlist = (int*)malloc(sizeof(int)*cores);
    for (int i=0;i<cores;i++) {
        score_list[i] = 0ull;
    }
    printf ("AhoBench C version %s\n"
            "Threads  : %d\n"
            "Duration : %d\n\n",__PROGRAM_VER__,cores,duration);

    // Start benchmark 
    clock_gettime(CLOCK_MONOTONIC, &ts);
    end_time =  ((unsigned long long)ts.tv_sec * 1000000000ull + (unsigned long long)ts.tv_nsec) + (1000000000ull * (unsigned long long)duration);

    for (int i=0;i<cores;i++) {
        idlist[i] = i;
        pthread_create(&thread_list[i],NULL,benchmark,(void*)i);
    }
    pthread_create(&timerThreadID,NULL,&timerthread,NULL);

    // End benchmark and make results
    pthread_join(timerThreadID,NULL);
    for (int i=0;i<cores;i++) {
        pthread_join(thread_list[i],NULL);
        printf("Thread %d     : %llu\n",i,score_list[i]);
        score += score_list[i];
    }

    printf("\n"
           "Worst score   : %llu\n"
           "Best score    : %llu\n"
           "Average score : %llu\n"
           "Total score   : %llu\n",get_worst(score_list,cores),get_best(score_list,cores),score/cores,score);

    return 0;
}