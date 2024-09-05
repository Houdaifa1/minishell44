#include "../minishell.h"

int ft_isalpha(int c, int i)
{
    if (i == 2)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            return (0);
    }
    else
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            return (0);
    }
    return (1);
}

char *remove_plus(char *var)
{
    int i;
    char *cpy;

    i = 0;
    while(var[i] != '+')
        i++;
    cpy = malloc(i + 2);
    i = 0;
    while(var[i] != '+')
    {
        cpy[i] = var[i];
        i++;
    }
    cpy[i] = '=';
    cpy[i + 1] = '\0';
    return(cpy);
}

int ft_contain_plus(char *commande)
{
    int i;
    int check;

    i = 0;
    check = 1;
    while (commande[i])
    {
        if (commande[i] == '+')
            check = 0;
        i++;
    }
    return (check);
}

int check_argument(char *commande)
{
    int i;

    if (commande[0] == '-')
    {
        //printf("export: %s: invalid option\n", commande);
        ft_print_in_stderr("export: ", commande,": invalid option\n");    
        return (1);
    }
    if (ft_isalpha(commande[0], 1) == 1)
        return (1);
    i = 0;
    while (commande[i] != '=' && commande[i] != '\0')
    {
        if (commande[i + 1] == '=' && commande[i] == '+')
        {
            return (2);
        }
        else if (ft_isalpha(commande[i], 2) == 1)
            return (1);
        else
            i++;
    }
    if (commande[i] == '\0')
        return (3);
    return (0);
}

int ft_strcmp2(char *s1, char *s2)
{
    int i;

    i = 0;
    if (!s1 || !s2)
        return (1);
    while (s1[i] && s2[i] && s1[i] != '=' && s2[i] != '+' && s1[i] == s2[i])
        i++;
    if ((s1[i] == '=' && s2[i] == '\0') || (s2[i] == '=' && s1[i] == '\0'))
        return (0);
    if (s2[i] == '+' && (s1[i] == '\0' || s1[i] == '='))
        return(0);
    return (s1[i] - s2[i]);
}