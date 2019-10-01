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
#include <stdint.h>
#include <langinfo.h>

int main(int argc, char *argv[]){
    DIR *directory;
    struct dirent *entry;
    int aflag = 0;
    int lflag = 0;
    int dirflag = 0;
    struct stat statbuf;
    struct passwd *pwd;
    struct group *grp;
    struct tm *tm;
    char datestring[256];

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
        dirflag = 1;
    }

    //TODO: fix bug where it goes to the root of the file system when given a
    // directory

    //TODO: fix segmentation fault when given file name

    char *filename;

    stat(argv[optind], &statbuf);

    if (dirflag == 0 && !S_ISDIR(statbuf.st_mode)){
        printf("%s\n", argv[optind]);
        exit(0);
    }

    while ((entry = readdir(directory))!=NULL){
        stat(entry->d_name, &statbuf);
        if (aflag == 0 && entry->d_name[0]=='.'){
            continue;
        }
        if (lflag == 1){
            /* permissions bitmask*/
            printf( (S_ISDIR(statbuf.st_mode)) ? "d" : "-");
            printf( (statbuf.st_mode & S_IRUSR) ? "r" : "-");
            printf( (statbuf.st_mode & S_IWUSR) ? "w" : "-");
            printf( (statbuf.st_mode & S_IXUSR) ? "x" : "-");
            printf( (statbuf.st_mode & S_IRGRP) ? "r" : "-");
            printf( (statbuf.st_mode & S_IWGRP) ? "w" : "-");
            printf( (statbuf.st_mode & S_IXGRP) ? "x" : "-");
            printf( (statbuf.st_mode & S_IROTH) ? "r" : "-");
            printf( (statbuf.st_mode & S_IWOTH) ? "w" : "-");
            printf( (statbuf.st_mode & S_IXOTH) ? "x" : "-");

            printf("%4lu", statbuf.st_nlink);
            if ((pwd = getpwuid(statbuf.st_uid)) != NULL){
                printf(" %-6.8s", pwd->pw_name);
            }
            if ((grp = getgrgid(statbuf.st_gid)) != NULL){
                printf(" %-6.8s", grp->gr_name);
            }

            printf(" %9jd", (intmax_t)statbuf.st_size);
            tm = localtime(&statbuf.st_mtime);
            strftime(datestring, sizeof(datestring), "%b %d %H:%M",
                    tm);

        }
        printf(" %s %s\n", datestring, entry->d_name);
    }
}
