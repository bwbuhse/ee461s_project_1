#include <readline/readline.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
  int cpid;
  // char cmd[80]

  // Main command loop
  while (1) {
    // Read the command into the cmd string
    char *cmd = readline("# ");

    // Tokenize the command
    char *cmdArgv[70]; // I used 70 because 3000/20~=70 :)
    int cmdArgc = 0;
    char *token;

    /* get the first token */
    token = strtok(cmd, " ");

    /* walk through other tokens */
    while (token != NULL) {
      cmdArgv[cmdArgc++] = token;

      token = strtok(NULL, " ");
    }

    // Add a NULL to the end of the argv array
    cmdArgv[cmdArgc] = NULL;

    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      execvp(cmdArgv[0], cmdArgv);
    }
  }

  return 0;
}
