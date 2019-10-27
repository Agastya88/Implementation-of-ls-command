#include <getopt.h>
#include <string.h>
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

void longOut(struct stat);

int main(int argc, char *argv[])
{
    DIR *directory;
    struct dirent *entry;
    int includehidden = 0;
    int longoutput = 0;
    int currentdirflag = 0;
    struct stat statbuf;

    int option;
    while ((option=getopt(argc, argv, "al")) != -1){
        switch (option) {
            case 'a':
                includehidden = 1;
                break;
            case 'l':
                longoutput = 1;
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    }

    char path[PATH_MAX];
    if (optind >= argc){
        if ((directory = opendir(".")) == NULL){
            perror("failed to open current dir");
            exit(1);
        }
        currentdirflag = 1;
    }

    int severaldirs = 0;
    if (optind < argc-1){
        severaldirs = 1;
    }

    while (optind < argc || currentdirflag){
        if (!currentdirflag)
        {
            strcpy(path, argv[optind]);
            if (stat(path, &statbuf) == 0){
                if (!S_ISDIR(statbuf.st_mode))
                {
                    if (longoutput) {
                        longOut(statbuf);
                    }
                    printf("%s\n", path);
                    optind++;
                    continue;
                } else if (severaldirs) {
                    printf("%s:\n", argv[optind]);
                }
            } else {
                perror(path);
                optind++;
                continue;
            }       
            if ((directory = opendir(path)) == NULL)
            {
                perror("failed to open directory");
                continue;
            }
        }

        while ((entry = readdir(directory))!=NULL)
        {
            if (!currentdirflag)
            {
                strcpy(path, argv[optind]);
                strcat(path, "/");
            } else {
                strcpy(path, "./");
            }
            strcat(path, entry->d_name);
            if (!includehidden && entry->d_name[0]=='.') {
                continue;
            }
            if (stat(path, &statbuf) == 0)
            {
                if (longoutput) {
                    longOut(statbuf);
                }
                printf("%s\n", entry->d_name);
            } else {
                perror("stat");
            }
        }
        if (severaldirs) {
            printf("\n");
        }
        currentdirflag = 0;
        closedir(directory);
        optind++;
    }
}

void longOut(struct stat statbuf)
{
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
    if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
    {
        printf(" %-6.8s", pwd->pw_name);
    } else {
        printf("%d", statbuf.st_uid);
    }
    if ((grp = getgrgid(statbuf.st_gid)) != NULL)
    {
        printf(" %-6.8s", grp->gr_name);
    } else {
        printf("%d", statbuf.st_gid);
    }
    printf(" %9jd", (intmax_t)statbuf.st_size);
    tm = localtime(&statbuf.st_mtime);
    strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);
    printf(" %s ", datestring);
}
