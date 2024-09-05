#include "../minishell.h"

int ft_update_val(t_env **envp, char *var, char *val)
{
    t_env *temp;
    int check;

    check = 0;
    temp = *envp;

    while (temp != NULL)
    {
        if (ft_strcmp2(temp->var, var) == 0)
        {
            if (val != NULL)
            {
                free(temp->val);
                free(temp->var);
                temp->var = ft_strdup(var);
                temp->val = ft_strdup(val);
            }
            check = 1;
        }
        temp = temp->next;
    }
    return (check);
}
void create_env(t_env **temp, t_env **envp, t_env *check)
{
    char *hold;

    if (ft_contain_plus(check->var) == 0)
    {
        hold = check->var;
        check->var = remove_plus(check->var);
        free(hold);
    }
    if ((*temp) == NULL)
        (*envp) = check;
    else
    {
        while ((*temp)->next)
            (*temp) = (*temp)->next;
        (*temp)->next = check;
    }
}
void add_to_env(t_env **temp, t_env **envp, t_env *node)
{
    t_env *iterate;
    char *hold;

    iterate = *envp;
    while (iterate)
    {
        if (ft_strcmp2(iterate->var, node->var) == 0)
        {
            if (iterate->val == NULL)
            {
                iterate->var = ft_strjoin(hold = iterate->var, "=", 1, 1);
                iterate->val = ft_strdup(node->val);
            }
            else
                iterate->val = ft_strjoin((hold = iterate->val), node->val, 1, 1);
            free(node->var);
            free(node->val);
            free(node);
            free(hold);
            return;
        }
        iterate = iterate->next;
    }
    create_env(temp, envp, node);
}

void ft_add_update_env(char *env, t_env **envp, t_env **temp)
{
    t_env *check;

    if (check_argument(env) == 3)
        check = ft_one_node2(env);
    else
        check = ft_one_node(env);
    if (ft_update_val(envp, check->var, check->val) == 0)
        create_env(temp, envp, check);
    else
    {
        free(check->var);
        free(check->val);
        free(check);
    }
}
int exec_export(char **commande, t_env **envp)
{
    t_env *temp;
    int j;
    int i;

    exit_code = 0;
    if (commande[1] == NULL)
        ft_print_env(*envp);
    else
    {
        j = 1;
        while (commande[j])
        {
            i = 0;
            temp = *envp;
            if (check_argument(commande[j]) == 0 || check_argument(commande[j]) == 3)
                ft_add_update_env(commande[j], envp, &temp);
            else if (check_argument(commande[j]) == 2)
                add_to_env(&temp, envp, ft_one_node(commande[j]));
            else
            {
                //printf("export: `%s': not a valid identifier\n", commande[j]);
                ft_print_in_stderr("export: '", commande[j],"': not a valid identifier\n");
                exit_code = 1;
            }
            j++;
        }
    }
    return(exit_code);
}
