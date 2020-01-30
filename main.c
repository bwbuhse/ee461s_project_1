#include <readline/readline.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// TODO: Add file redirects     [ ]
// TODO: Add process groups     [ ]
// TODO: Add pipes              [ ]

// Used for tokenizing user input
// Params:
//      cmd - a pointe rthe FULL command input from the user
//      cmdArgv - a pointer to the argv for the current command
//      startIndex - the index to start parsing cmd at
void tokenizeToNextCmd(char **cmd, char ***cmdArgv, int *startIndex);

int main() {
  pid_t cpid;
  int status;
  // char cmd[80]

  // Main command loop
  while (1) {
    // Read the command into the cmd string
    char *cmd = readline("# ");

    // TODO: Maybe move Tokenization into a separate function that stops once a
    // redirect/pipe is found? Tokenize the command
    char *cmdArgv[70][70]; // I used 70 because 2000/30~=70 :)
    int previousFileIndex = 0, cmdIndex = 0, argIndex;

    while (argIndex != -1) {
    }

    // while (

    /* TODO: Fix this for new command
    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      execvp(cmdArgv[0], cmdArgv);
    } */

    // Wait for the child processes to finish
    // TODO: Update this whenever I add background processes
    waitpid(-1, &status, 0);
  }

  return 0;
}

void tokenizeToNextCmd(char **cmd, char ***cmdArgv, int *startIndex) {
  int cmdArgc = 0;
  char *token = strtok(cmd, " ");

  while (token != NULL) {
    if (strcmp("<", token) == 0 || strcmp(">", token) == 0 ||
        strcmp("2>", token) == 0 || strcmp("|", token) == 0) {
      // Returns if
      return;
    }

    *cmdArgv[cmdArgc++] = token;
    token = strtok(NULL, " ");
  }

  cmdArgv[cmdArgc] = NULL;
  *startIndex = -1;
}

