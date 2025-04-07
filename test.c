#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	else
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
int	check_builtin(char *s)
{
	int			len;
	int			i;
	static char	*b_ins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	len = ft_strlen(s);
	i = 0;
	while (b_ins[i])
	{
		if (!ft_strncmp(s, b_ins[i], len))
			return (1);
		i++;
	}
	return (0);
}

int main()
{
	printf("%d\n", check_builtin("envo"));
}