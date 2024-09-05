#include "../minishell.h"


void ft_putstr(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

int check_valid_options(char **commande)
{
    int j;
    int i;

    j = 1;
    while (commande[j] != NULL)
    {
        if (commande[j][0] == '-')
            i++;
        else
            return (j);
        i = 1;
        while (commande[j][i])
        {
            if (commande[j][i] != 'n')
                return (j);
            while(commande[j][i] == 'n')
                i++;
        }
        j++;
    }
    return(j);
}

int exec_echo(char **commande)
{
    int j;
    int n;
    
    j = check_valid_options(commande);
    n = j;
    while(commande[j] != NULL)
    {
        ft_putstr(commande[j]);
        if (commande[j + 1] != NULL)
            ft_putstr(" ");
        j++;
    }
    if (n == 1)
        ft_putstr("\n");
    return(0);
}
