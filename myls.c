#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>

int main(int argc, char *argv[]){
    DIR *directory;
    struct dirent *entry;
    int aflag = 0;
    int lflag = 0;
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grp;
    int status;

    /* check arguments */
    int option;
    while ((option=getopt(argc, argv, "al")) != -1){
        switch (option) {
            case 'a':
                aflag = 1;
                break;
            case 'l':
                lflag = 1;
                break;
            default:
                break;
        }
    }

    if (optind < argc){
        directory = opendir(argv[optind]);
    } else{
        directory = opendir (".");
    }

    char *filename;
    while ((entry = readdir(directory))!=NULL){
        filename=entry->d_name;
        if (lflag == 0){
            if (filename[0]=='.'){
                if (aflag==1){
                    printf("%s\n", filename);
                }
            }
            else{
                printf("%s\n", filename);
            }
        }
        else{
            stat(entry->d_name, &statbuf);
            //printf("%10.10s", statbuf.st_mode&0777);
            printf("%4d", statbuf.st_nlink);
            if ((pwd = getpwuid(statbuf.st_uid)) != NULL){
                printf(" %-8.8s", pwd->pw_name);
            }
            if ((grp = getgrgid(statbuf.st_gid)) != NULL){
                printf(" %-8.8s", grp->gr_name);
            }
            printf("%s\n", filename);
        }
    }

    /* given directory name, list items in directory */

    /* given file name, output file name */
}
