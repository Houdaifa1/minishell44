#include "../minishell.h"


void ft_putchar_fd(char c)
{
    write(2, &c, 1);
}
void	ft_putstr_fd(char const *str)
{
	while (*str)
		ft_putchar_fd(*str++);
}

void ft_print_in_stderr(char *s1, char *s2, char *s3)
{
	char *temp;
	char *join;

	temp = ft_strjoin(s1, s2, 1, 1);
	join = ft_strjoin(temp, s3, 1, 1);
	free(temp);
	ft_putstr_fd(join);
	free(join);

}