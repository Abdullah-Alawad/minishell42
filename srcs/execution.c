#include "../minishell.h"

int open_heredocs(t_command *cmd)
{
	int     pipefd[2];
	char    *line;
	int     i = 0;

	while (cmd->here_arr && cmd->here_arr[i])
	{
		if (pipe(pipefd) == -1)
			return (perror("pipe"), 1);

		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, cmd->here_arr[i], ft_strlen(cmd->here_arr[i])) == 0)
				break;
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		free(line);
		close(pipefd[1]); // done writing

		// Only save the last heredoc as stdin
		if (cmd->here_arr[i + 1] == NULL)
			cmd->in_fd = pipefd[0]; // save read-end
		else
			close(pipefd[0]); // discard read-end for earlier heredocs

		i++;
	}
	return (0);
}


int	open_redirections(t_command *cmd)
{
	if (cmd->here_arr)
	{
		if (open_heredocs(cmd))
			return (1);
	}
	if (cmd->in_file && cmd->heredoc == 0)
	{
		cmd->in_fd = open(cmd->in_file, O_RDONLY);
		if (cmd->in_fd < 0)
			return (perror(cmd->in_file), 1);
	}
	if (cmd->out_file)
	{
		if (cmd->append == 2)
			cmd->out_fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			cmd->out_fd = open(cmd->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->out_fd < 0)
			return (perror(cmd->out_file), 1);
	}
	return (0);
}

void	redirect_io(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (cmd->in_fd != -1)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd != -1)
		dup2(cmd->out_fd, STDOUT_FILENO);
}

void	restore_io(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

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
	t_command	*cmd;
	static int	status;
	int			stdin = -1;
	int			stdout = -1;
	bool		redirected;

	cmd = cmds;
	while (cmd)
	{
		if (open_redirections(cmd))
		{
			cmd = cmd->next;
			continue ;
		}
		redirected = (cmd->in_fd != -1 || cmd->out_fd != -1);
		if (redirected)
			redirect_io(cmd, &stdin, &stdout);

		if (cmd->is_builtin)
			status = execute_builtin_cmd(cmd, env_lst, status);
		else
			status = execute_external(cmd, env_lst);

		if (redirected)
			restore_io(stdin, stdout);

		if (cmd->in_fd != -1)
			close(cmd->in_fd);
		if (cmd->out_fd != -1)
			close(cmd->out_fd);

		cmd = cmd->next;
	}
	printf("status: %d\n", status);
}

