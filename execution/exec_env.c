#include "../minishell.h"

void ft_print_env(t_env *envp)
{
    while (envp)
    {
        printf("%s", envp->var);
        if (envp->val)
            printf("\"%s\"\n", envp->val);
        else
            printf("\n");
        envp = envp->next;
    }
}

int exec_env(char **commande, t_env *envp)
{
    exit_code = 0;
    if (commande[1] == NULL)
    {
        ft_print_env(envp);
    }
    else
    {
        //printf("env: ‘%s’: No such file or directory\n", commande[1]);
        ft_print_in_stderr("env: '", commande[1], "': No such file or directory\n");
        exit_code = 127;
    }
    return(exit_code);
}