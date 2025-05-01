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
	char		*copy_pwd;
	char		*cwd;

	head = *env;
	pwd = NULL;
	old_pwd = NULL;
	copy_pwd = ft_strjoin("OLDPWD=", "tmp_path");
	if (!copy_pwd)
		return ;
	while (head)
	{
		if (ft_strncmp("PWD", head->key, ft_strlen(head->key)) == 0)
			pwd = head;
		if (ft_strncmp("OLDPWD", head->key, ft_strlen(head->key)) == 0)
			old_pwd = head;
		head = head->next;
	}
	if (!old_pwd)
	{
		old_pwd = init_env(copy_pwd, 0);
		if (!old_pwd)
		{
			free(copy_pwd);
			return ;
		}
		env_add_back(env, old_pwd);
	}
	free(copy_pwd);
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

int	handle_cd(char **cmd, t_env_list **env)
{
	char	*path;
	char	*tmp;
	int		num;
	int		is_tilde;

	is_tilde = 0;
	num = count_av(cmd);
	if (num > 2)
	{
		perror("cd: too many arguments\n");
		return (1);
	}
	if (num == 2 && cmd[1][0] == '~')
		is_tilde = 1;
	if (num == 1 || (num == 2 && is_tilde))
	{
		path = get_home_path(env);
		chdir(path);
		if (is_tilde && cmd[1][1] == '\0')
		{
			update_pwd(env);
			return (0);
		}
	}
	if (num == 2)
	{
		tmp = cmd[1];
		if (is_tilde && tmp[1] == '/')
			tmp += 2;
		if (chdir(tmp) != 0)
		{
			perror("cd");
			return (1);
		}
		update_pwd(env);
	}
	return (0);
}
