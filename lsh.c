#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int lsh_launch(char **args) {

  int pid, wpid;
  int status;
  pid = fork();

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);

  } else if (pid < 0) {
    perror("lsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line) {
  int bufsize = LSH_TOK_BUFSIZE;
  int position = 0;

  char **tokens = malloc(bufsize * sizeof(char *));

  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh:allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "lsh:allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }

  tokens[position] = NULL;

  return tokens;
}

#define LSH_RL_BUFFERSIZE 1024
char *lsh_read_line(void) {

  int bufsize = LSH_RL_BUFFERSIZE;
  int position = 0;

  char *buffer = malloc((sizeof(char) * bufsize));
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh:allocation error");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += LSH_RL_BUFFERSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh:allocationi error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void lsh_loop(void) {
  char *line;
  char **args;
  int status = 0;

  do {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_launch(args);

    free(line);
    free(args);

  } while (status);
}

int main(int argc, char **argv) { lsh_loop(); }
