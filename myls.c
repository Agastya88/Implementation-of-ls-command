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
#include <errno.h>

int longOut(struct stat);
int fileCheck(char *path, int longOutput, struct stat statbuf);

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

    if (optind >= argc){
        directory = opendir(".");
        currentdirflag = 1;
    }

    int severaldirs = 0;
    if (optind < argc-1){
        severaldirs = 1;
    }

    char *path;
    while (optind < argc || currentdirflag){
        if (!currentdirflag){
            directory = opendir(argv[optind]);
            if (fileCheck(argv[optind], longOutput, statbuf)){
                optind++;
                continue;
            } else if (severaldirs){
                printf("%s:\n", argv[optind]);
            }
        }

        while ((entry = readdir(directory))!=NULL){
            if (currentdirflag){
                path = entry->d_name;
            } else{
                path = argv[optind];
            }
            if (stat(path, &statbuf) == 0){
                if (!includeHidden && entry->d_name[0]=='.'){
                    continue;
                }
                if (longOutput){
                    longOut(statbuf);
                }
                printf("%s\n", entry->d_name);
            } else{
                perror("stat");
                exit(EXIT_FAILURE);
            }
        }
        if (severaldirs){
            printf("\n");
        }
        currentdirflag = 0;
        closedir(directory);
        optind++;
    }
}

int fileCheck(char *path, int longOutput, struct stat statbuf){
    if (stat(path, &statbuf) == 0){
        if (!S_ISDIR(statbuf.st_mode)){
            if (longOutput){
                longOut(statbuf);
            }
            printf("%s\n", path);
            return 1;
        } else {
            return 0;
        }
    } else {
        perror(path);
        return 1;
    }
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
    } else{
        perror("getpwuid");
    }
    if ((grp = getgrgid(statbuf.st_gid)) != NULL){
        printf(" %-6.8s", grp->gr_name);
    } else{
        perror("getpwuid");
    }
    printf(" %9jd", (intmax_t)statbuf.st_size);
    tm = localtime(&statbuf.st_mtime);
    strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);
    printf(" %s ", datestring);
    //TODO have return value be based on success or error
    return 1;
}
