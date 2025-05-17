#include "../minishell.h"

int	execute_builtin_cmd(t_command *cmd, t_env_list **env, int res)
{
	if (!cmd || !cmd->av)
		return (0);
	if (ft_strncmp("echo", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_echo(cmd->av);
	else if (ft_strncmp("env", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_env(env);
	else if (ft_strncmp("pwd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_pwd();
	else if (ft_strncmp("cd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_cd(cmd->av, env);
	else if (ft_strncmp("exit", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_exit(env, cmd, res);
	else if (ft_strncmp("export", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_export(cmd->av, env);
	else if (ft_strncmp("unset", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_unset(cmd->av, env);
	return (res);
}

void	execute_command(t_command *cmds, t_env_list **env_lst)
{
	t_command	*cmd = cmds;
	static int	status = 0;
	int			pipe_fd[2];
	int			prev_fd = -1; // read end of previous pipe

	while (cmd)
	{
		if (open_redirections(cmd))
		{
			cmd = cmd->next;
			continue;
		}
		if (cmd->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return;
		}

		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}
		else if (pid == 0) // child
		{
			if (cmd->in_fd != -1)
				dup2(cmd->in_fd, STDIN_FILENO);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);

			if (cmd->out_fd != -1)
				dup2(cmd->out_fd, STDOUT_FILENO);
			else if (cmd->next)
				dup2(pipe_fd[1], STDOUT_FILENO);

			// close unused fds
			if (pipe_fd[0]) close(pipe_fd[0]);
			if (pipe_fd[1]) close(pipe_fd[1]);
			if (prev_fd != -1) close(prev_fd);

			if (cmd->is_builtin)
				exit(execute_builtin_cmd(cmd, env_lst, status));
			else
				exit(execute_external(cmd, env_lst));
		}

		// parent process
		if (cmd->in_fd != -1) close(cmd->in_fd);
		if (cmd->out_fd != -1) close(cmd->out_fd);
		if (prev_fd != -1) close(prev_fd);
		if (cmd->next)
		{
			prev_fd = pipe_fd[0];
			close(pipe_fd[1]);
		}

		cmd = cmd->next;
	}
	while (wait(&status) > 0);
	printf("status: %d\n", WEXITSTATUS(status));
}