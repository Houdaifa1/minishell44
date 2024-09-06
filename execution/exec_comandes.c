#include "../minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    if (s1 == NULL || s2 == NULL)
        return (1);
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

int check_if_contain_redirections(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '>' || cmd[i] == '<')
            return (1);
        i++;
    }
    return (0);
}

int check_if_contain_only_redirection(char *cmd)
{
    if (cmd[0] == '>' && cmd[1] == '\0')
        return (1);
    else if (cmd[0] == '<' && cmd[1] == '\0')
        return (2);
    else if (cmd[0] == '>' && cmd[1] == '>' && cmd[2] == '\0')
        return (3);
    else if (cmd[0] == '<' && cmd[1] == '<' && cmd[2] == '\0')
        return (4);
    return (0);
}
char *ft_cpy_commande(char *argumment, int i)
{
    char *commande;
    int l;

    l = i;
    while (argumment[l] && argumment[l] != '>' && argumment[l] != '<')
        l++;
    commande = malloc(l + 1);
    l = 0;
    while (argumment[i] && argumment[i] != '>' && argumment[i] != '<')
        commande[l++] = argumment[i++];
    commande[l] = '\0';
    return (commande);
}
int ft_handle_output(char *argument, int i)
{
    char *file_name;
    int j;
    int output_fd;

    file_name = ft_cpy_commande(argument, i + 1);
    while (argument[i + 1] && argument[i + 1] != '>' && argument[i + 1] != '<')
        i++;
    output_fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (output_fd < 0)
    {
        perror(file_name);
        free(file_name);
        return (-1);
    }
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    free(file_name);
    return (i);
}

int ft_handle_input(char *argument, int i)
{
    char *file_name;
    int j;
    int input_fd;

    file_name = ft_cpy_commande(argument, i + 1);
    while (argument[i + 1] && argument[i + 1] != '>' && argument[i + 1] != '<')
        i++;
    input_fd = open(file_name, O_RDONLY);
    if (input_fd < 0)
    {
        perror(file_name);
        free(file_name);
        return (-1);
    }
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    free(file_name);
    return (i);
}
int ft_which_redirection(char *argumment, int i)
{
    if (argumment[i] == '>')
    {
        if ((i = ft_handle_output(argumment, i)) == -1)
            return (-1);
    }
    if (argumment[i] == '<')
    {
        if ((i = ft_handle_input(argumment, i)) == -1)
            return (-1);
    }
    return (i);
}

char *handle_redirections(char *argumment)
{
    char *commande;
    int i;

    i = 0;
    commande = NULL;
    exit_code = 0;
    if (argumment[i] != '>' && argumment[i] != '<')
        commande = ft_cpy_commande(argumment, 0);
    while (argumment[i])
    {
        while (argumment[i] != '>' && argumment[i] != '<')
            i++;
        i = ft_which_redirection(argumment, i);
        if (i == -1)
        {
            exit_code = 1;
            free(commande);
            return (NULL);
        }
        i++;
    }
    return (commande);
}
int free_and_shift(char **argumment, int j)
{
    int k;

    free(argumment[j]);
    k = j;
    while (argumment[k])
    {
        argumment[k] = argumment[k + 1];
        k++;
    }
    j--;
    return (j);
}
int check_if_redirection_end(char *argumment)
{
    int i;

    i = 0;
    while (argumment[i])
        i++;
    if (argumment[i - 1] == '>' || argumment[i - 1] == '<')
        return (1);
    return (0);
}
int handle_redirection_end(char **argumment, int j)
{
    int i;
    char *join;
    char *arg;

    join = ft_strjoin(argumment[j], argumment[j + 1], 1, 1);
    arg = handle_redirections(join);
    if (exit_code == 1)
    {
        free(join);
        return (-1);
    }
    if (arg != NULL)
    {
        free(argumment[j]);
        argumment[j] = arg;
        j = free_and_shift(argumment, j + 1);
    }
    else
    {
        j = free_and_shift(argumment, j);
        j = free_and_shift(argumment, j + 1);
    }
    free(join);
    return (j);
}
int check_handle_redirections(char **argumment)
{
    int j;
    char *arg;
    char *join;

    j = 0;
    while (argumment[j])
    {
        if (check_if_contain_only_redirection(argumment[j]) > 0)
        {
            join = ft_strjoin(argumment[j], argumment[j + 1], 1, 1);
            handle_redirections(join);
            if (exit_code == 1)
            {
                free(join);
                return (1);
            }
            j = free_and_shift(argumment, j);
            j = free_and_shift(argumment, j + 1);
            free(join);
        }
        else if ((check_if_contain_redirections(argumment[j])) == 1)
        {
            if (check_if_redirection_end(argumment[j]) == 1)
            {
                if ((j = handle_redirection_end(argumment, j)) == -1)
                    return (1);
            }
            else
            {
                arg = handle_redirections(argumment[j]);
                if (exit_code == 1)
                {
                    free(arg);
                    return (1);
                }
                if (arg != NULL)
                {
                    free(argumment[j]);
                    argumment[j] = arg;
                }
                else
                    j = free_and_shift(argumment, j);
            }
        }
        j++;
    }
}
int exec_simple_commande(char **argumment, t_env **envp, t_data **data, t_hold **hold_vars)
{
    if (check_handle_redirections(argumment) == 1)
        return (exit_code);
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
    return (exit_code);
}

void exec_commandes(t_data *commandes, t_env **envp, t_data **data, t_hold **hold_vars)
{
    if (commandes->next == NULL && commandes != NULL)
        exec_simple_commande(commandes->argumment, envp, data, hold_vars);
    else if (commandes->next != NULL)
        exec_with_pipes(envp, data, hold_vars);
}