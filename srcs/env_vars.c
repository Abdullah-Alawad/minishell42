#include "../minishell.h"

char	*get_env(char *data)
{
	int		i;
	int		j;
	int		copy;
	char	*res;
	char	*var;
	char	*var_res;

	i = 0;
	var = malloc(ft_strlen(data));
	if (!var)
		return (NULL);
	while (data[i] && data[i] != $)
		i++;
	copy = i - 1;
	if (data[i] && data[i + 1] == ' ')
		return (data);
	else
	{
		i++;
		j = 0;
		while (data[i] && (data[i] == '_' || ft_isalnum(data[i])))
			var[j++] = data[i++];
		var[j] = '\0';
		var_res = getenv(var);
		if (var_res)
			//continue
	}
}