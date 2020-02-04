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
// TODO: Add file redirects     [x]
// TODO: Add pipes              [ ]
// TODO: Add signal handling    [ ]
// TODO: Create bools_t & nums_t[ ]

// Some macros used for keeping code nice
#define PROMPT "# "
#define OUTPUT_REDIR ">"
#define INPUT_REDIR "<"
#define ERR_REDIR "2>"
#define PIPE "|"

// A struct used for holding parsed versions of commands
typedef struct tokenized_cmd_t {
  char **argv; // Args for execvp
  char *output_file;
  char *input_file;
  char *error_file;
  bool pipeArg1; // set to true when the command is on the left side of a pipe
  bool pipeArg2; // set to true when the command is on the right side of a pipe

} tokenized_cmd_t;

// Used to tokenize the user's input
// Returns the total number of tokens
int tokenize(char **input, char **tokenized_input_ptr[]);

// Used to set up a tokenized_cmd
// nums {&num_tokens, &start_index}
// bools {&error_found, &redirect_found}
bool setup_tok_cmd(char **tokenized_input_ptr[], tokenized_cmd_t *cmd,
                   int *nums[], bool *bools[]);

int main() {
  pid_t cpid;
  int status;

  // Main command loop
  while (true) {
    // Read the input into the string and then tokenize it
    char *input = readline(PROMPT);
    char **tokenized_input;

    // Exit the shell if it's passed the EOF character
    if (input == NULL) {
      printf("\n");
      break;
    }

    // Check for any redirections in the command
    // cmd2 is only used if there's a pipe
    tokenized_cmd_t cmd1 = {tokenized_input, NULL, NULL, NULL, false, false};
    tokenized_cmd_t cmd2 = {NULL, NULL, NULL, NULL, false, false};

    // Nums used for looking for redirects
    int num_tokens = tokenize(&input, &tokenized_input);
    int start_index = 0;

    // Flag used for if errors are found
    bool redirect_found = false;
    bool found_error = false;

    int *nums[] = {&num_tokens, &start_index};
    bool *bools[] = {&found_error, &redirect_found};

    bool pipe_found = setup_tok_cmd(&tokenized_input, &cmd1, nums, bools);

    // If I found an error then there's no point in trying
    // the command
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

        // If the file doesn't exist then just skip this
        // command and go to the top
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

// bool setup_tok_cmd(tokenized_cmd_t *cmd, char **tokenized_input_ptr[],
//                  int start_index, int num_tokens, bool *found_error) {
bool setup_tok_cmd(char **tokenized_input_ptr[], tokenized_cmd_t *cmd,
                   int *nums[], bool *bools[]) {

  bool pipe_found = false;
  int i;

  for (i = (*nums)[1]; i < (*nums)[0]; i++) {
    // Check for pipe
    if (!strcmp((*tokenized_input_ptr)[i], PIPE)) {
      if (i + 1 < (*nums)[0]) {
        pipe_found = true;
      } else {
        *bools[0] = true;
        break;
      }
    }

    // Check for various file redirections
    if (!strcmp((*tokenized_input_ptr)[i], OUTPUT_REDIR)) {
      *bools[1] = true;

      if (i + 1 < (*nums)[0]) {
        (*cmd).output_file = (*tokenized_input_ptr)[i + 1];
      } else {
        // The input was invalid if i >= num_tokens here (ended cmd w >)
        *bools[0] = true;
        break;
      }
    } else if (!strcmp((*tokenized_input_ptr)[i], INPUT_REDIR)) {
      *bools[1] = true;

      if (i + 1 < (*nums)[0]) {
        (*cmd).input_file = (*tokenized_input_ptr)[i + 1];
      } else {
        // The input was invalid if i >= num_tokens here (ended cmd w >)
        *bools[0] = true;
        break;
      }
    } else if (!strcmp((*tokenized_input_ptr)[i], ERR_REDIR)) {
      *bools[1] = true;

      if (i + 1 < (*nums)[0]) {
        (*cmd).error_file = (*tokenized_input_ptr)[i + 1];
      } else {
        // The input was invalid if i >= num_tokens here (ended cmd w >)
        *bools[0] = true;
        break;
      }
    }

    // If a redirect hasn't been found yet, we know we're still adding args
    if (*bools[1]) {
      (*tokenized_input_ptr)[i] = NULL;
    }
  }

  // Used to return the value, needed for when a pipe is found
  (*nums)[0] = i;

  return pipe_found;
}
