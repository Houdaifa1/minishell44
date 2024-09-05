#include "../minishell.h"

int ft_strcmp3(char *s1, char *s2)
{
	int i;

	i = 0;

	while (s1[i] && s2[i] && s1[i] != '=' && s1[i] == s2[i])
		i++;
	if (s1[i] == '=' && s2[i] == '\0')
		return (0);
	if (s2[i] == '=')
		return (1);
	return (s1[i] - s2[i]);
}

void ft_unset_env(t_env **envp, char *var)
{
	t_env *remove;
	t_env *current;

	current = *envp;
	while (current && current->next)
	{
		if (ft_strcmp3(current->next->var, var) == 0)
		{
			remove = current->next;
			current->next = current->next->next;
			free(remove->val);
			free(remove->var);
			free(remove);
		}
		current = current->next;
	}
	current = *envp;
	if (current && ft_strcmp3(current->var, var) == 0)
	{
		*envp = current->next;
		free(current->val);
		free(current->var);
		free(current);
	}
}

int exec_unset(char **commande, t_env **envp)
{
	int i;

	exit_code = 0;
	i = 1;
	while (commande[i])
	{
		ft_unset_env(envp, commande[i]);
		i++;
	}
	return(exit_code);
}