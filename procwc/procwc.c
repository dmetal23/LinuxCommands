/*
Daniel Rojas
Comp 322
TuTh 12:30PM 
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *pipes = NULL;
pid_t *pids = NULL;

void count_words(char *file, int outfd)
{
  char buffer[256];
  int n;
	int c, cc = 0, wc = 0, lc = 0;
	FILE *f;

  f = fopen(file, "r");
  if (f == NULL) {
    printf("Invalid file %s\n", file);
    return;
  }

	while ((c = getc(f)) != EOF) {
		++cc;
		if (isgraph(c)) {
			++wc;
			do {
				c = getc(f);
				if (c == EOF)
					goto done;
				++cc;
			} while (isgraph(c));
		}
		if (c == '\n')
			++lc;
	}
done:
  snprintf(buffer, sizeof(buffer),"%8d%8d%8d", lc, wc, cc );
  write(outfd, buffer, strlen(buffer)+1);
}

int main(int argc, char *argv[])
{
  int i;
  int total_chars = 0, total_words = 0, total_lines = 0;


  if (argc < 2) {
    printf("usage: %s <file1> <file2> ... <filen>\n", argv[0]);
    return -1;
  }

  pipes = malloc((argc -1) * sizeof(int) *2);
  if (pipes == NULL) {
    printf("malloc pipes failed\n");
    return -1;
  }

  pids = malloc((argc -1) * sizeof(pid_t));
  if (pids == NULL) {
    printf("malloc pids failed\n");
    free(pipes);
    return -1;
  }

  for (i=0; i < argc - 1; i++) {
    pipe(&pipes[i*2]);
    pids[i] = fork();
    if (pids[i] == 0) {
      /* child */
      close(pipes[i*2]);
      count_words(argv[i+1], pipes[i*2 + 1]);
      return 0;
    }
    else {
      close(pipes[i*2 + 1]);
    }
  }

  for (i=0; i < argc - 1; i++) {
    char buffer[256];
    int cc = 0, wc= 0, lc = 0;
    int ret;

    ret = read(pipes[i*2], buffer, sizeof(buffer));
    if (ret > 0) {
      printf("%s  [%d]%s\n", buffer, pids[i], argv[i+1]);
      sscanf(buffer, "%8d%8d%8d", &lc, &wc, &cc);
      total_lines += lc;
      total_words += wc;
      total_chars += cc;
    }
    waitpid(pids[i], NULL, 0);
  }

  printf("%8d%8d%8d  %s\n", total_lines, total_words, total_chars, "total" );
  free(pipes);
  free(pids);
  return 0;
}
