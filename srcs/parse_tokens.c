#include "../minishell"

t_command	*cmd_create(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->av = malloc(sizeof(char *) * 1);
	if (!cmd->av)
		return (NULL);
	cmd->av[0] = NULL;
	cmd->in_file = NULL;
	cmd->out_file = NULL;
	cmd->pipe = 0;
	cmd->heredoc = 0;
	cmd->append = 0;
	cmd->is_builtin = 0;
	cmd->next = NULL;
	return (cmd);
}

int	set_av(t_command *cmd, char *data, t_qtype q_type)
{
	char	**av;
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
		lst_add_back(cmds, (*new_cmd));
		i = 1;
	}
	else if (tokens->type == T_TEXT)
	{
		i = set_av((*new_cmd), tokens->data, tokens->quote_type);

	}
}

int	parse_tokens(t_token *tokens, t_command **cmds)
{
	t_command	*new_cmd;

	new_cmd = cmd_create();
	if (!new_cmd)
		return (0);
	lst_add_back(cmds, new_cmd);
	while (tokens)
	{
		if (!process_token(&new_cmd, cmds, tokens))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}