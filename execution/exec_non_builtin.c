#include "../minishell.h"

void ft_free_arr(char **paths)
{
    int i;

    i = 0;
    while (paths[i] != NULL)
    {
        free(paths[i]);
        i++;
    }
    free(paths);
}

int test_paths(char **commande, char **paths, char **envp)
{
    int j;
    char *join;
    char *temp;

    if (execve(commande[0], commande, envp) == -1)
    {
        j = 0;
        while (paths[j])
        {
            temp = ft_strjoin(paths[j], "/", 1, 1);
            join = ft_strjoin(temp, commande[0], 1, 1);
            free(temp);
            execve(join, commande, envp);
            free(join);
            j++;
        }
    }
    return (1);
}

int count_nodes(t_env *envp)
{
    int count;

    count = 0;
    while (envp)
    {
        count++;
        envp = envp->next;
    }
    return (count);
}
char **convert_envp_to_arr(t_env *envp)
{
    int count;
    char **envp_arr;
    int j;

    count = count_nodes(envp);
    envp_arr = malloc(sizeof(char *) * (count + 1));
    j = 0;
    while (envp)
    {
        if (envp->val != NULL)
        {
            envp_arr[j] = ft_strjoin(envp->var, envp->val, 1, 1);
            j++;
        }
        envp = envp->next;
    }
    envp_arr[j] = NULL;
    return (envp_arr);
}

int exec_non_builtin(char **commande, t_env **envp, t_data **data, t_hold **hold_vars)
{
    int pid;
    char **paths;
    char **envp_arr;
    int status;

    pid = fork();
    if (pid == 0)
    {
        paths = ft_split(ft_getenv(*envp, "PATH"), ':');
        envp_arr = convert_envp_to_arr(*envp);
        if (test_paths(commande, paths, envp_arr) == 1)
        {
            if (commande[0][0] == '\0')
                //printf("command '%s' not found\n", commande[0]);
                ft_print_in_stderr("commande '", commande[0],"': not a valid identifier\n");
            else
                //printf("%s: command not found\n", commande[0]);
                ft_print_in_stderr(commande[0],": commande not found\n", "");
            exit(127);
        }
        ft_free_arr(paths);
        ft_free_arr(envp_arr);
        exit(0);
    }
    waitpid(pid, &status, 0);
    exit_code = WEXITSTATUS(status);
    return(exit_code);
}

