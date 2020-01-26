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

        // TODO: Tokenize the command

        // Fork
        cpid = fork();
        if (cpid == 0)
        {
            // child code
            execlp(cmd, cmd, (char *)NULL);
        }
    }

    return 0;
}
