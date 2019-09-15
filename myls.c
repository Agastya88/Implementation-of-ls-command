#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	/* check arguments */
	int option;
	while ((option=getopt(argc, argv, "al")) != -1){
		switch (option) {
			case 'a':
				printf("option a\n");
				/* include hidden files */
				break;
			case 'l':
				printf("option l\n");
				/* include extra info */
				break;
			default:
				break;
		}
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
