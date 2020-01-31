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
int tokenize(char **input, char **tokenized_input_ptr[]);

int main() {
  pid_t cpid;
  int status;

  // Main command loop
  while (1) {
    // Read the input into the string and then tokenize it
    char *input = readline("# ");
    char **tokenized_input;
    int num_tokens = tokenize(&input, &tokenized_input);

    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      execvp(tokenized_input[0], tokenized_input);
    }

    // Wait for the child processes to finish
    // TODO: Update this whenever I add background processes
    waitpid(-1, &status, 0);
  }

  return 0;
}

int tokenize(char **input, char **tokenized_input_ptr[]) {
  // I used 70 because 3000/20~=70 :)
  char *tokenized_input[70];

  // Create my variables
  int num_tokens = 0;
  char *token;

  token = strtok(*input, " ");
  while (token != NULL) {
    tokenized_input[num_tokens++] = token;
    token = strtok(NULL, " ");
  }
  tokenized_input[num_tokens] = NULL;

  *tokenized_input_ptr = tokenized_input;

  return num_tokens;
}

