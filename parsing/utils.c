#include "../minishell.h"

t_data *creat_node(char **arguments)
{
	t_data *new_node = malloc(sizeof(t_data));
	if (!new_node)
		return NULL;
	new_node->argumment = arguments;
	new_node->next = NULL;
	return (new_node);
}

void ft_add_node(t_data **head, char **arguments)
{
	t_data *new_node;
	t_data *tmp = *head;

	new_node = creat_node(arguments);
	if (!new_node)
		return;
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

int ft_skip_space(char c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\v' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int ft_count_args(char *input)
{
	int count;
	int i;
	char quote;

	count = 0;
	i = 0;
	quote = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote == 0)
				quote = input[i];

			else if (quote == input[i])
			{
				quote = 0;
			}
			i++;
		}
		else if (ft_skip_space(input[i]) == 1 && quote == 0)
		{
			while (ft_skip_space(input[i]) == 1)
				i++;
			if (input[i] == '\0')
				break;
			count++;
		}
		else
			i++;
	}
	return (count + 1);
}

int check_qout(char *input)
{
	int i;
	int flag;
	int quote;
	int count;

	i = 0;
	flag = 0;
	quote = 0;
	count = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote == 0)
			{
				quote = input[i];
				flag = 1;
			}
			else if (quote == input[i])
			{
				quote = 0;
				flag = 0;
			}
		}
		else if (input[i] == '|' && quote == 0)
		{
			if (input[i] == '|' && input[i + 1] == '|')
				return (1);
		}
		i++;
	}
	return (flag);
}

int check_redirections(char *input)
{
	int i;
	int quote;

	i = 0;
	quote = 0;
	while (input[i] != '\0' && ft_skip_space(input[i]) == 1)
		i++;
	if (input[i] == '|')
		return (1);
	while (input[i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (quote == 0)
				quote = input[i];
			else if (quote == input[i])
				quote = 0;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>' && quote == 0)
		{
			i++;
			while (input[i] != '\0' && ft_skip_space(input[i]) == 1)
				i++;
			if (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>')
				return (1);
		}
		i++;
	}
	return (0);
}

char *strsplit_by_pipe(char **str)
{
	char *start;
	char *pipe_pos;
	char quote;
	int i;

	start = *str;
	pipe_pos = NULL;
	quote = 0;
	i = 0;
	if (start == NULL)
		return NULL;
	while (start[i] != '\0')
	{
		if (start[i] == '\'' || start[i] == '"')
		{
			if (quote == 0)
				quote = start[i];
			else if (quote == start[i])
				quote = 0;
		}
		else if (start[i] == '|' && quote == 0)
		{
			pipe_pos = &start[i];
			break;
		}
		i++;
	}
	if (pipe_pos != NULL)
	{
		*pipe_pos = '\0';
		*str = pipe_pos + 1;
	}
	else
		*str = NULL;
	return (start);
}

size_t ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

char *ft_strdup(const char *s1)
{
	int j;
	char *dup;

	dup = malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	j = 0;
	while (s1[j] != '\0')
	{
		dup[j] = s1[j];
		j++;
	}
	dup[j] = '\0';
	return (dup);
}

char *ft_strjoine(char const *s1, char const *s2)
{
	size_t i;
	size_t j;
	char *newstr;

	if (s1 == NULL)
		return (s2);
	i = 0;
	j = 0;
	newstr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (newstr == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		newstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		newstr[i + j] = s2[j];
		j++;
	}
	newstr[i + j] = '\0';
	return (newstr);
}
