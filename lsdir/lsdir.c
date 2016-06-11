/*
Daniel Rojas
Comp 322
TuTh 12:30PM 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

void lsdir(char *path)
{
	DIR *dp;
	struct dirent *dir;

	dp = opendir(path);
	if (dp == NULL) {
		fprintf(stderr, "cannot open directory %s: %s\n",
			path, strerror(errno));
		exit(EINVAL);
	}

	while ( errno = 0, (dir = readdir(dp)) != NULL ) 
		puts(dir->d_name);

	if (errno != 0) fprintf(stderr, "Error Reading %s: %s\n",
				path, strerror(errno));

	return;
    
} 

int main(int argc, char* argv[])
{
	char *directory;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
		exit(EINVAL);
	}
	if (argc == 2)
		directory = argv[1];
	else
		directory = ".";

	lsdir(directory);
	return 0;

}