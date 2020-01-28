#include <readline/readline.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// TODO: Add process groups     [ ]
// TODO: Add file redirects     [ ]
// TODO: Add pipes              [ ]

int main() {
  pid_t cpid;
  int status;
  // char cmd[80]

  // Main command loop
  while (1) {
    // Read the command into the cmd string
    char *cmd = readline("# ");

    // Tokenize the command
    char *cmdArgv[70]; // I used 70 because 3000/20~=70 :)
    int cmdArgc = 0;
    char *token;

    token = strtok(cmd, " ");
    while (token != NULL) {
      cmdArgv[cmdArgc++] = token;
      token = strtok(NULL, " ");
    }
    cmdArgv[cmdArgc] = NULL;

    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      execvp(cmdArgv[0], cmdArgv);
    }

    // Wait for the child processes to finish
    // TODO: Update this whenever I add background processes
    waitpid(-1, &status, 0);
  }

  return 0;
}
