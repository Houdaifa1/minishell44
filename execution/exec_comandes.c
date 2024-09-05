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

int check_if_contain_redirections(char *cmd)
{
    int i;

    i = 0;
    while(cmd[i])
    {
        if (cmd[i] == '>' || cmd[i] == '<')
            return(1);
        i++;
    }
    return(0);
}

int check_if_contain_only_redirection(char *cmd)
{
    if (cmd[0] == '>' && cmd[1] == '\0')
        return(1);
    else if (cmd[0] == '<' && cmd[1] == '\0')
        return(2);
    else if (cmd[0] == '>' && cmd[1] == '>' && cmd[2] == '\0')
        return(3);
    else if (cmd[0] == '<' && cmd[1] == '<' && cmd[2] == '\0')
        return(4);
    return(0);
}

int ft_handle_output(char *argument, int i)
{
    char file_name[1024];
    int j;
    int output_fd;

    j = 0;
    
    while(argument[i + 1] && argument[i + 1] != '>' && argument[i + 1] != '<')
    {
        file_name[j++] = argument[i + 1];
        i++;
    }
    file_name[j] = '\0';
    //check if already exist
    output_fd = open(file_name, O_CREAT | O_RDWR , 0644);
    if (output_fd < 0)
    {
        perror("open");
        return -1;
    }
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
}
char *handle_redirections(char *argumment)
{
    char *commande;
    int i;

    i = 0;
    commande = NULL;
    while(argumment[i] != '>' && argumment[i] != '<')
    {
        commande[i] = argumment[i];
        i++;
    }
    //if (argumment[i + 1] == '>')
        //handlle_append
    //if (argumment[i + 1] == '<')
        //handlle_herdoc
    //if(argummrnt[i] == '<')
        //handlle_input
    if(argumment[i] == '>')
    {
        //handlle_output
        i = ft_handle_output(argumment, i);
    }
    return(commande);

}

void check_redirection(char ***argumment)
{
    int j;
    char *new_argumment;

    j = 0;
    while((*argumment)[j])
    {
        if ((check_if_contain_only_redirection((*argumment)[j])) > 0)
        {
            //later
        }
        if ((check_if_contain_redirections((*argumment)[1])) == 1)
        {
            new_argumment = handle_redirections((*argumment)[1]);
            if (new_argumment == NULL)
            {
                free((*argumment)[j]);
                (*argumment)[j] = (*argumment)[j + 1];
            }
            else
            {
                free((*argumment)[j]);
                (*argumment)[j] = ft_strdup(new_argumment);
            }
        }

        j++;
    }
    printf("After: argumment[1] = %s\n", (*argumment)[1]);

}
int exec_simple_commande(char **argumment, t_env **envp, t_data **data, t_hold **hold_vars)
{
    check_redirections(&argumment);
    printf("arg [1]   :  %s\n", argumment[1]);
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