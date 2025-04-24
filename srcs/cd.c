#include "../minishell.h"

int	count_av(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*get_home_path(t_env_list **env)
{
	char		*path;
	t_env_list	*head;

	path = NULL;
	head = *env;
	while (head)
	{
		if (ft_strncmp("HOME", head->key, ft_strlen(head->key)) == 0)
			path = head->data;
		head = head->next;
	}
	return (path);
}

void	update_pwd(t_env_list **env)
{
	t_env_list	*pwd;
	t_env_list	*old_pwd;
	t_env_list	*head;
	char		*cwd;

	head = *env;
	pwd = NULL;
	old_pwd = NULL;
	while (head)
	{
		if (ft_strncmp("PWD", head->key, ft_strlen(head->key)) == 0)
			pwd = head;
		if (ft_strncmp("OLDPWD", head->key, ft_strlen(head->key)) == 0)
			old_pwd = head;
		head = head->next;
	}
	if (old_pwd && pwd)
	{
		free(old_pwd->data);
		old_pwd->data = ft_strdup(pwd->data);
	}
	if (pwd)
	{
		free(pwd->data);
		cwd = getcwd(NULL, 0);
		if (cwd)
			pwd->data = cwd;
		else
			pwd->data = NULL;
	}
}
// consider handiling "cd ~/Documents"
int	handle_cd(char **cmd, t_env_list **env)
{
	char	*path;
	int		num;

	num = count_av(cmd);
	if (num > 2)
	{
		perror("cd: too many arguments\n");
		return (1);
	}
	else if (num == 1 || (num == 2 && ft_strncmp("~", cmd[1], ft_strlen(cmd[1])) == 0))
	{
		path = get_home_path(env);
		chdir(path);
		update_pwd(env);
	}
	else if (num == 2)
	{
		if (chdir(cmd[1]) != 0)
		{
			perror("cd");
			return (1);
		}
		update_pwd(env);
	}
	return (0);
}
