#include <fcntl.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// TODO: Add process groups     [ ]
// TODO: Add file redirects     [ ]
// TODO: Add pipes              [ ]
// TODO: Add signal handling    [ ]
// TODO: Figure out where my
//      memory leaks are        [ ]

// Some macros used for keeping code nice
#define PROMPT "# "
#define OUTPUT_REDIR ">"
#define INPUT_REDIR "<"
#define ERR_REDIR "2>"
// #define PIPE "|"

// A struct used for holding parsed versions of commands
typedef struct tokenized_cmd {
  char **argv; // Args for execvp
  char *output_file;
  char *input_file;
  char *error_file;
  // bool pipe; // bool set to true when the command is part of a pipe

} tokenized_cmd;

// Used to tokenize the user's input
// Returns the total number of tokens
int tokenize(char **input, char **tokenized_input_ptr[]);

int main() {
  pid_t cpid;
  int status;

  // Main command loop
  while (true) {
    // Flag used for if errors are found
    bool found_error = false;
    // Read the input into the string and then tokenize it
    char *input = readline(PROMPT);
    char **tokenized_input;

    // Exit the shell if it's passed the EOF character
    if (input == NULL) {
      printf("\n");
      break;
    }

    int num_tokens = tokenize(&input, &tokenized_input);

    // Check for any redirections in the command
    // cmd2 is only used if there's a pipe
    tokenized_cmd cmd1 = {tokenized_input, NULL, NULL, NULL};
    // tokenized cmd cmd2 = {NULL, NULL, NULL, NULL};
    bool redirect_found = false;

    for (int i = 0; i < num_tokens; i++) {
      // Check for various file redirections
      if (!strcmp(tokenized_input[i], OUTPUT_REDIR)) {
        redirect_found = true;

        if (i + 1 < num_tokens) {
          cmd1.output_file = tokenized_input[i + 1];
        } else {
          // The input was invalid if i >= num_tokens here (ended cmd w >)
          found_error = true;
          break;
        }
      } else if (!strcmp(tokenized_input[i], INPUT_REDIR)) {
        redirect_found = true;

        if (i + 1 < num_tokens) {
          cmd1.input_file = tokenized_input[i + 1];
        } else {
          // The input was invalid if i >= num_tokens here (ended cmd w >)
          found_error = true;
          break;
        }
      } else if (!strcmp(tokenized_input[i], ERR_REDIR)) {
        redirect_found = true;

        if (i + 1 < num_tokens) {
          cmd1.error_file = tokenized_input[i + 1];
        } else {
          // The input was invalid if i >= num_tokens here (ended cmd w >)
          found_error = true;
          break;
        }
      }

      // If a redirect hasn't been found yet, we know we're still adding args
      if (redirect_found) {
        tokenized_input[i] = NULL;
      }
    }

    // If I found an error then there's no point in trying the command
    if (found_error) {
      continue;
    }

    // Fork
    cpid = fork();
    if (cpid == 0) {
      // child code
      // Do any redirects
      if (cmd1.output_file) {
        int ofd = open(cmd1.output_file, O_CREAT | O_WRONLY | O_TRUNC,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        dup2(ofd, STDOUT_FILENO);
      }
      if (cmd1.input_file) {
        int ifd = open(cmd1.input_file, O_RDONLY);

        // If the file doesn't exist then just skip this command and go to the
        // top
        if (ifd == -1) {
          continue;
        }
        dup2(ifd, STDIN_FILENO);
      }
      if (cmd1.error_file) {
        int ofd = open(cmd1.error_file, O_CREAT | O_WRONLY | O_TRUNC,
                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        dup2(ofd, STDERR_FILENO);
      }

      execvp(tokenized_input[0], tokenized_input);
      free(input);
      break;
    } else {
      // Parent code
      free(tokenized_input);
      free(input);
    }

    // Wait for the child processes to finish
    // TODO: Update this whenever I add background processes
    waitpid(cpid, &status, 0);
  }

  return 0;
}

int tokenize(char **input, char **tokenized_input_ptr[]) {
  // I used 70 because 3000/20~=70 :)
  // TODO: Free this!
  char **tokenized_input = (char **)malloc(sizeof(char *) * 70);

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

