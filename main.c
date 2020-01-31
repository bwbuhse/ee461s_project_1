#include <readline/readline.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// TODO: Add process groups     [ ]
// TODO: Add file redirects     [ ]
// TODO: Add pipes              [ ]
// TODO: Add signal handling    [ ]

// Used to tokenize the user's input
// Returns the total number of tokens
int tokenize(char **input, char **tokenizedInputPtr[]);

int main() {
  pid_t cpid;
  int status;

  // Main command loop
  while (1) {
    // Read the input into the string and then tokenize it
    char *input = readline("# ");
    char **tokenizedInput;
    int numTokens = tokenize(&input, &tokenizedInput);

    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      execvp(tokenizedInput[0], tokenizedInput);
    }

    // Wait for the child processes to finish
    // TODO: Update this whenever I add background processes
    waitpid(-1, &status, 0);
  }

  return 0;
}

int tokenize(char **input, char **tokenizedInputPtr[]) {
  // I used 70 because 3000/20~=70 :)
  char *tokenizedInput[70];

  // Create my variables
  int numTokens = 0;
  char *token;

  token = strtok(*input, " ");
  while (token != NULL) {
    tokenizedInput[numTokens++] = token;
    token = strtok(NULL, " ");
  }
  tokenizedInput[numTokens] = NULL;

  *tokenizedInputPtr = tokenizedInput;

  return numTokens;
}

