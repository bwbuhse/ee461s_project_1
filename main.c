#include <readline/readline.h>

int main()
{
    int cpid;
    // char cmd[80];

    while (1)
    {
        char *cmd = readline("#");
        printf("%s\n", cmd);
    }

    return 0;
}
