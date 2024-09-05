#include "minishell.h"

void print_use_list(t_data *head) // for testing
{
    t_data *temp = head;
    int i = 0;
    while (temp)
    {
        while (temp->argumment[i] != NULL)
        {
            printf("Arg %d:%s\n", i, temp->argumment[i]);
            i++;
        }
        i = 0;
        temp = temp->next;
        if (temp)
        {
            printf("---- Next Command ----\n");
        }
    }
}

int exit_code ;

int main(int arc, char **arv, char **envp)
{
    t_data *data;
    t_env *env_var;
    t_hold *hold_vars;
    char *input;
    char *temp;

    env_var = env_to_list(envp);
    hold_vars = malloc(sizeof(t_hold));
    data = NULL;
    while (1)
    {
        input = readline(temp = print_prompt(env_var, NULL, NULL));
        
        if (input[0] != '\0')
        {
            add_history(input);
            if (parse_line(&data, input, env_var) == 0)
            {
                hold_vars->input = input;
                hold_vars->temp = temp;
                exec_commandes(data, &env_var, &data, &hold_vars);
            }
    
        }
        ft_free_list(data);
        data = NULL;
        free(temp);
        free(input);
    }
}
