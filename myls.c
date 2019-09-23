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
        if (filename[0]=='.'){
            if (aflag==1){
                printf("%s\n", filename);
            }
        }
        else{
            printf("%s\n", filename);
        }
    }

    /* given -l, include extra info */

    /* given directory name, list items in directory */

    /* given file name, output file name */
}
