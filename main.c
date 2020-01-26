#include <readline/readline.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    int cpid;
    // char cmd[80];

    // Main command loop
    while (1)
    {
        // Read the command into the cmd string
        char *cmd = readline("# ");

        // Tokenize the command
        char *cmdArgs[70];
        int cmdArgc = 0;
        char *token;

        /* get the first token */
        token = strtok(cmd, " ");

        /* walk through other tokens */
        while (token != NULL)
        {
            cmdArgs[cmdArgc++] = token;

            token = strtok(NULL, " ");
        }
        
        // Add a NULL to the end of the argv array
        cmdArgs[cmdArgc] = NULL;

        // Fork
        cpid = fork();
        if (cpid == 0)
        {
            // child code
            execvp(cmdArgs[0], cmdArgs);
        }
    }

    return 0;
}
