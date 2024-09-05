#include "../minishell.h"


void ft_free_list(t_data *head)
{
    t_data *temp;
    int i;

    while (head)
    {
        temp = head;
        head = head->next;
        if (temp->argumment)
        {
            i = 0;
            while (temp->argumment[i] != NULL)
            {
                free(temp->argumment[i]);
                i++;
            }
            free(temp->argumment);
        }
        free(temp);
    }
}
void ft_free_list2(t_env *head)
{
    t_env *temp;

    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->var);
        free(temp->val);
        free(temp);
    }
}
