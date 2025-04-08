#include "../minishell.h"

char	*get_env(char *data)
{
	int		i;
	int		j;
	char	*res;
	char	one[2];
	char	var[100];
	char	*var_res;

	i = 0;
	res = malloc(1200);
	if (!res)
		return (NULL);
	res[i] = '\0';
	while (data[i])
	{
		if (data[i] == '\\' && data[i + 1] == '$')
		{
			ft_strlcat(res, "$", 1200);
			i = i + 2;
		}
		else if (data[i] == '$' && data[i + 1] && ft_isalnum(data[i + 1])) // little change here
		{
			i++;
			j = 0;
			while (data[i] && (ft_isalnum(data[i]) || data[i] == '_'))
				var[j++] = data[i++];
			var[j] = '\0';
			var_res = getenv(var);
			if (var_res)
				ft_strlcat(res, var_res, 1200);
		}
		else
		{
			one[0] = data[i];
			one[1] = '\0';
			ft_strlcat(res, one, 1200);
			i++;
		}
	}
	return (res);
}

// int main()
// {
// 	char *res = get_env("hello \\$NAME");
// 	printf("%s\n", res);
// }