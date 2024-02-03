#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TASH_RL_BUFSIZE 1024
#define TASH_TOK_BUFSIZE 64
#define TASH_TOK_DELIM " \t\r\n\a"

// Function declarations for builtin shell commands:
int tash_cd(char **args);
int tash_help(char **args);
int tash_exit(char **args);

// List of builtin commands, followed by their corresponding functions.
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &tash_cd,
  &tash_help,
  &tash_exit
};

int tash_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

// Builtin function implementations.
int tash_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "tash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("tash");
    }
  }
  return 1;
}

int tash_help(char **args)
{
  int i;
  printf("TASH: The Tutorial Shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < tash_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}

int tash_exit(char **args)
{
  return 0;
}

// Function to launch a program.
int tash_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("tash");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("tash");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// Function to execute shell built-in or launch program.
int tash_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < tash_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return tash_launch(args);
}

// Function to read a line of input from stdin.
char *tash_read_line(void)
{
  int bufsize = TASH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "tash: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += TASH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "tash: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

// Function to split a line into tokens.
char **tash_split_line(char *line)
{
  int bufsize = TASH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "tash: allocation error\n");
}