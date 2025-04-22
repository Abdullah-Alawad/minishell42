#include "../minishell.h"

int	count_av(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*get_home_path(t_env_list *env)
{
	char	*path;

	while (env)
	{
		if (ft_strncmp("HOME", env->key, ft_strlen(env->key)) == 0)
			path = env->data;
		env = env->next;
	}
	return (path);
}

void	update_pwd(t_env_list *env, char *new)
{

}

int	handle_cd(char **cmd, t_env_list *env)
{
	char	*path;
	int		num;

	num = count_av(cmd);
	if (num == 1 || (num == 2 && ft_strncmp("~", cmd[1], ft_strlen(cmd[1])) == 0))
	{
		path = get_home_path(env);
		update_pwd(env, path);
		chdir(path);
	}
	if (num > 2)
	{
		perror("bash: cd: too many arguments/n");
		return (1);
	}
	return (0);
}