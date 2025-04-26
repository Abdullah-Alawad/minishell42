#include "../minishell.h"

int	ft_strchr_i(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if ((char)c == '\0')
		return (i);
	return (0);
}

int	check_export_format(char *cmd)
{
	int	pos;

	if (ft_strchr_i(cmd, '='))
	{
		pos = ft_strchr_i(cmd, '=');
		if (cmd[pos--] && cmd[pos++])
			return (1);
	}
	return (0);
}

int	new_export(char *cmd, t_env_list **env)
{
	int			len;
	t_env_list	*tmp;

	tmp = *env;
	len = ft_strchr_i(cmd, '=');
	while (tmp)
	{
		if (ft_strncmp(cmd, tmp->key, len - 1) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	update_env(char *cmd, t_env_list **env)
{
	int			start;
	int			len;
	char		*new_data;
	t_env_list	*tmp;

	tmp = *env;
	len = ft_strlen(cmd);
	start = ft_strchr_i(cmd, '=');
	new_data = ft_substr(cmd, start + 1, len - (start + 1));
	if (!new_data)
		return (0); //double check
	while (tmp)
	{
		if (ft_strncmp(cmd, tmp->key, start - 1) == 0)
		{
			free(tmp->data);
			tmp->data = new_data;
			tmp->exported = 1;
			return ;
		}
		tmp = tmp->next;
	}
}

int	handle_export(char **cmd, t_env_list **env)
{
	int			i;
	t_env_list	*new;

	i = 1;
	while (cmd[i])
	{
		if (check_export_format(cmd[i]))
		{
			if (new_export(cmd[i], env))
			{
				new = init_env(cmd[i], 1);
				if (!new)
					return (0); // double check
				env_add_back(env, new);
			}
			else
				update_env(cmd[i], env);
		}
		i++;
	}
	return (0);
}
