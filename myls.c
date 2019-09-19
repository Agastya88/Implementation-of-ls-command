#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]){
    DIR *directory;
    struct dirent *entry;
    int aflag = 0;
    int lflag = 0;

    /* check arguments */
    int option;
    while ((option=getopt(argc, argv, "al")) != -1){
        switch (option) {
            case 'a':
                printf("option a\n");
                aflag = 1;
                break;
            case 'l':
                printf("option l\n");
                lflag = 1;
                break;
            default:
                break;
        }
    }

    if (optind < argc){
      printf(argv[optind]);
    } else{
      directory = opendir (".");
    }

    while ((entry = readdir(directory))!=NULL){
      printf("%s\n", entry->d_name);
    }

    /* if no arguments, list files in curent directory */
        /* open current directory */
        /* print out names of files */

    /* otherwise, parse arguments */

    /* given -a, include hidden files */

    /* given -l, include extra info */

    /* given directory name, list items in directory */

    /* given file name, output file name */
}
