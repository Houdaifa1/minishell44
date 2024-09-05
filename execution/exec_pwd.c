#include "../minishell.h"

int exec_pwd(char **commande)
{
    char path[PATH_MAX];

    if (commande[1] != NULL && commande[1][0] == '-' &&  commande[1][1] != '\0')
    {
        if (commande[1][1] != '-' || commande[1][2] != '\0')
        {
            ft_putstr_fd("pwd takes no option\n");
            return (2);
        }
    }
    getcwd(path, PATH_MAX);
    printf("%s\n", path);
    return (0);
}
