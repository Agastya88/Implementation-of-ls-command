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

//TODO choose better names for longoutput flag and function
int longOut(struct stat);

int main(int argc, char *argv[]){
    DIR *directory;
    struct dirent *entry;
    int includeHidden = 0;
    int longOutput = 0;
    int currentdirflag = 0;
    struct stat statbuf;

    int option;
    while ((option=getopt(argc, argv, "al")) != -1){
        switch (option) {
            case 'a':
                includeHidden = 1;
                break;
            case 'l':
                longOutput = 1;
                break;
            default:
                break;
        }
    }

    //TODO proper handling of multiple non-option arguments
    //TODO need to change below to a while loop i think...
    if (optind < argc){
        directory = opendir(argv[optind]);
    } else{
        directory = opendir(".");
        currentdirflag = 1;
    }

    while (optind < argc || currentdirflag == 1){
        if (!currentdirflag){
            directory = opendir(argv[optind]);
        }
        if (stat(argv[optind], &statbuf) == 0){
            if (!S_ISDIR(statbuf.st_mode) && currentdirflag == 0){
                if (longOutput){
                    longOut(statbuf);
                }
                printf(" %s\n", argv[optind]);
                optind++;
                continue;
            }
        } else if (currentdirflag != 1){
            exit (0);
        }

        while ((entry = readdir(directory))!=NULL){
            stat(entry->d_name, &statbuf);
            if (!includeHidden && entry->d_name[0]=='.'){
                continue;
            }
            if (longOutput){
                longOut(statbuf);
            }
            printf(" %s\n", entry->d_name);
        }
        currentdirflag = 0;
        optind++;
    }
    closedir(directory);
}

int longOut(struct stat statbuf){
    struct passwd *pwd;
    struct group *grp;
    struct tm *tm;
    char datestring[256];

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
    strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);
    printf(" %s", datestring);
    //TODO have return value be based on success or error
    return 1;
}
