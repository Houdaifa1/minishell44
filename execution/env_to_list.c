#include "../minishell.h"

t_env *ft_one_node(char *envp)
{
    t_env *node;
    char *temp;
    int i;
    int j;

    node = malloc(sizeof(t_env));
    if (!node)
        return(NULL);
    i = 0;
    while(envp[i] != '=' && envp[i] != '\0')
        i++;
    temp = malloc(i + 2);
    i = 0;
    while(envp[i] != '=' && envp[i] != '\0')
    {
        temp[i] = envp[i];
        i++;
    }
    temp[i] = '=';
    temp[i + 1] = '\0';
    node->var = ft_strdup(temp);
    node->val = grep_env_value(envp, i + 1);
    node->next = NULL;
    free(temp);
    return(node);
}
t_env *ft_one_node2(char *envp)
{
    t_env *node;

    node = malloc(sizeof(t_env));
    if (!node)
        return(NULL);
    node->var = ft_strdup(envp);
    node->val =  NULL;
    node->next = NULL;
    return(node);
}

void ft_create_nodes(t_data **head, char *envp)
{
    t_env *temp;
    t_env  *new_node;
    int     i;
    
    temp = *head;
    i = 0;
    while(envp[i] != '=' && envp[i] != '\0')
        i++;
    if (envp[i] == '=')
        new_node = ft_one_node(envp);
    else
        new_node = ft_one_node2(envp);
    if (temp == NULL)
    {
        *head = new_node;
    }
    else
    {
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = new_node; 
    }
}
t_env   *env_to_list(char **envp)
{
    t_env   *env_var;
    int i;

    i = 0;
    env_var = NULL;
    while (envp[i] != NULL)
    {
        ft_create_nodes(&env_var, envp[i]);
        i++;
    }
    return(env_var);
}