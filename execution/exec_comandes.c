#include "../minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;
    
    i = 0;
    if (s1 == NULL || s2 == NULL)
        return (1);
    while(s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return(s1[i] - s2[i]);
}

int exec_simple_commande(char **argumment, t_env **envp, t_data **data, t_hold **hold_vars)
{
    if (ft_strcmp(argumment[0], "echo") == 0)
        exit_code = exec_echo(argumment);
    else if (ft_strcmp(argumment[0], "pwd") == 0)
        exit_code = exec_pwd(argumment);
    else if (ft_strcmp(argumment[0], "cd") == 0)
        exit_code = exec_cd(argumment, *envp);
    else if (ft_strcmp(argumment[0], "export") == 0)
        exit_code = exec_export(argumment, envp);
    else if (ft_strcmp(argumment[0], "env") == 0)
        exit_code = exec_env(argumment, *envp);
    else if (ft_strcmp(argumment[0], "unset") == 0)
        exit_code = exec_unset(argumment, envp);
    else if (ft_strcmp(argumment[0], "exit") == 0)
        exit_code = exec_exit(argumment, envp, data, hold_vars);
    else
        exit_code = exec_non_builtin(argumment, envp, data, hold_vars);
    return(exit_code);
}

void exec_commandes(t_data *commandes, t_env **envp, t_data **data, t_hold **hold_vars)
{
    if (commandes->next == NULL && commandes != NULL)
        exec_simple_commande(commandes->argumment, envp, data, hold_vars);
    else if(commandes->next != NULL)
       exec_with_pipes(envp, data, hold_vars);
}