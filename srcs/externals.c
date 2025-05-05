#include "../minishell.h"

int	add_envs(t_env_list *env, char **envp)
{
	int		i;
	char	*tmp;

	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (1);
		if (!env->data)
			envp[i] = tmp;
		else
		{
			envp[i] = ft_strjoin(tmp, env->data);
			if (!envp[i])
			{
				free(tmp);
				return (1);
			}
		}
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (0);
}

int	env_len(t_env_list *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

char	**env_list_to_array(t_env_list **env)
{
	int		envp_len;
	char	**envp;

	envp_len = env_len(*env);
	envp = malloc(sizeof(char *) * (envp_len + 1));
	if (!envp)
		return (NULL);
	if (add_envs(*env, envp))
		return (NULL);
	return (envp);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*get_env_path(t_env_list *env)
{
	t_env_list	*tmp;
	int			len;

	tmp = env;
	while (env)
	{
		len = ft_strlen(tmp->key); //problem here
		if (ft_strncmp(tmp->key, "PATH", len) == 0)
			return (tmp->data);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_env_list **env)
{
	char	*path;
	char	**paths;
	char	*tmp;
	char	*full;
	int		i;

	if (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0)
	{
		path = ft_strdup(cmd);
		if (!path)
			return (NULL);
		return (path);
	}
	path = get_env_path(*env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
		{
			free_paths(paths);
			return (NULL);
		}
		full = ft_strjoin(tmp, cmd);
		if (!tmp)
		{
			free_paths(paths);
			return (NULL);
		}
		free(tmp);
		if (access(full, X_OK) == 0)
		{
			free_paths(paths);
			return (full);
		}
		free(full);
		i++;
	}
	free_paths(paths);
	return (NULL);
}

int	execute_external(t_command *cmd, t_env_list **env)
{
	int		pid;
	int		status;
	char	*path;
	char	**envp;

	(void)env;
	if (!cmd || !cmd->av || !cmd->av[0])
		return (1);
	path = get_cmd_path(cmd->av[0], env);
	if (!path)
	{
		printf("%s: command not found\n", cmd->av[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		envp = env_list_to_array(env);
		if (!envp)
			return (1);
		execve(path, cmd->av, NULL);
	}
	else
		waitpid(pid, &status, 0);
	free(path);
	return (1);
}
