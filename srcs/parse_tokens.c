#include "../minishell.h"

int	set_operator(t_command *cmd, t_token *tokens)
{
	if (tokens->type == T_REDIRECT_IN || tokens->type == T_HEREDOC)
	{
		cmd->in_file = ft_strdup(tokens->next->data);
		if (!cmd->in_file)
			return (0);
		if (tokens->type == T_HEREDOC)
			cmd->heredoc = 1;
	}
	else if (tokens->type == T_REDIRECT_OUT || tokens->type == T_APPEND)
	{
		cmd->out_file = ft_strdup(tokens->next->data);
		if (!cmd->out_file)
			return (0);
		if (tokens->type == T_APPEND)
			cmd->append = 2;
		else
			cmd->append = 1;
	}
	return (1);
}

int	check_builtin(char *s)
{
	int			len;
	int			i;
	static char	*b_ins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	len = ft_strlen(s);
	i = 0;
	while (b_ins[i])
	{
		if (!ft_strncmp(s, b_ins[i], len))
			return (1);
		i++;
	}
	return (0);
}

int	set_av(t_command *cmd, char *data, t_qtype q_type)
{
	char	**new_av;
	int		i;

	i = 0;
	while (cmd->av && cmd->av[i])
		i++;
	new_av = malloc(sizeof(char *) * (i + 2));
	if (!new_av)
		return (0);
	i = -1;
	while (cmd->av && cmd->av[++i])
		new_av[i] = cmd->av[i];
	if ((q_type == NO_Q || q_type == DOUBLE_Q) && ft_strchr(data, '$'))
		new_av[i] = get_env(data);
	else
		new_av[i] = ft_strdup(data);
	if (!new_av[i])
	{
		free(new_av); // NOTE: I think not enough i's need to be freed
		return (0);
	}
	new_av[i + 1] = NULL;
	free(cmd->av); // NOTE: same as above note
	cmd->av = new_av;
	return (1);
}

int	process_token(t_command **new_cmd, t_command **cmds, t_token *tokens)
{
	t_command	*cmd;
	int			i;

	i = 0;
	if (tokens->type == T_PIPE)
	{
		(*new_cmd)->pipe = 1;
		cmd = cmd_create();
		if (!cmd)
			return (0);
		(*new_cmd) = cmd;
		cmds_add_back(cmds, (*new_cmd));
		i = 1;
	}
	else if (tokens->type == T_DATA)
	{
		i = set_av((*new_cmd), tokens->data, tokens->quote_type);
		(*new_cmd)->is_builtin = check_builtin((*new_cmd)->av[0]);
	}
	else
		i = set_operator((*new_cmd), tokens);
	return (i);
}

int	parse_tokens(t_token *tokens, t_command **cmds)
{
	t_command	*new_cmd;

	new_cmd = cmd_create();
	if (!new_cmd)
		return (0);
	cmds_add_back(cmds, new_cmd);
	while (tokens)
	{
		if (!process_token(&new_cmd, cmds, tokens))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}