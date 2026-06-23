//このプログラムには日本語が含まれます。←By this line, compilers will detect the charset.
#define __PROGRAM_VER__ "1.1.0"
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

int duration = 10;

time_t current_time = 0;
time_t end_time;

int benchmark() {
    unsigned int a = 0;

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
        current_time = time(NULL);
        a += 1;
    }
    
    return a;
}


void print_help(char *progcmd) {
    printf ("AhoBench for DOS %s\n"
            "\n"
            "Usage:\n"
            "%s [-j <THREADS>] [-d <DURATION>]\n"
            "Options:\n"
            "-d           : Specify the number of duration in seconds. (Default: 10)\n"
            "-v --version : Print version information.\n"
            "-h --help    : Print this help.\n",__PROGRAM_VER__,progcmd);
}

void print_ver() {
    printf("AhoBench for DOS\nVersion : %s\n\n"
           "Copyrignt (C) 2026 TLFoxHuman (TlFoxHuman, TranslucentFoxHuman, 半狐, 半透明狐人間)\n\n"
           "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n"
			"\n"
		   "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n"
			"\n"
			"You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.\n",__PROGRAM_VER__);
}


int main(int argc,char** argv) {
    int i;
    unsigned int score;

    if (argc >= 2) {
        for (i=1;i<argc;i++){
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
            } else {
                fprintf(stderr,"Unrecognized option: %s\n",argv[i]);
                return 2;
            }
        }
    }
    printf ("AhoBench for DOS %s\n"
            "\n"
            "Duration : %d\n\n",__PROGRAM_VER__,duration);

    // Start benchmark 
    end_time =  time(NULL) + duration + 1;
    current_time = time(NULL);
    
    while (current_time != end_time - duration) {
        current_time = time(NULL);
    }
    
    score = benchmark();
    

    printf("Total score   : %d\n",score);

    return 0;
}