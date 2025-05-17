#include "../minishell.h"

int open_heredocs(t_command *cmd)
{
	int     pipefd[2];
	char    *line;
	int     i;

	i = 0;
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
		close(pipefd[1]);
		if (cmd->here_arr[i + 1] == NULL)
			cmd->in_fd = pipefd[0];
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
