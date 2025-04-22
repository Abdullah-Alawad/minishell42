#include "../minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	check_operator(char *command, int start, t_token **tokens_list)
{
	int		i;
	t_qtype	q_type;

	i = start;
	q_type = quote_type(command[start]);
	if ((command[i] == '<' && command[i + 1] == '<')
		|| (command[i] == '>' && command[i] == '>'))
		i += 2;
	else
		i += 1;
	if (!add_to_list(command, start, i, tokens_list, q_type))
		return (-1);
	return (i);
}

int	lexer(char *command, t_token **tokens_list)
{
	int		i;
	int		start;
	t_qtype	q_type;

	i = 0;
	while (command[i])
	{
		while (command[i] && command[i] == ' ')
			i++;
		if (command[i] && is_operator(command[i]))
		{
			i = check_operator(command, i, tokens_list);
			if (i == -1)
				return (0);
		}
		else if (command[i] && (command[i] == '\"' || command[i] == '\''))
		{
			i = handle_quotes(command, i, tokens_list);
			if (i == -1)
				return (0);
		}
		else
		{
			start = i;
			q_type = quote_type(command[start]);
			while (command[i] && command[i] != ' ' && !is_operator(command[i]))
				i++;
			if (!add_to_list(command, start, i, tokens_list, q_type))
				return (0);
		}
	}
	return (1);
}

int	good_quotes(char *command)
{
	char	single_quote;
	char	double_quote;
	int		i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (command[i])
	{
		if (command[i] == '\"')
		{
			if (double_quote == 0)
				double_quote = command[i];
			else if (double_quote == command[i])
				double_quote = 0;			
		}
		else if (command[i] == '\'')
		{
			if (single_quote == 0)
				single_quote = command[i];
			else if (single_quote == command[i])
				single_quote = 0;			
		}
		command++;
	}
	if (!double_quote && !single_quote)
		return (1);
	else
		return (0);
}

t_command	*handle_command(char *command)
{
	t_token		*tokens_list;
	t_command	*cmds_list;

	tokens_list = NULL;
	cmds_list = NULL;
	if(good_quotes(command))
	{
		if(!lexer(command, &tokens_list))
		{
			printf(RED"[ERROR], faild mallocs\n"RESET);
			return (NULL);
		}
		if (!check_tokens(tokens_list))
		{
			free_tokens(&tokens_list);
			printf(RED"[ERROR], syntax error\n"RESET);
			return (NULL);
		}
		if (!parse_tokens(tokens_list, &cmds_list))
		{
			free_tokens(&tokens_list);
			free_commands(&cmds_list);
			printf(RED"[ERROR], failed to parse tokens\n"RESET);
			return (NULL);
		}
		free_tokens(&tokens_list);
		return (cmds_list);
	}
	else
	{
		printf(RED"[ERROR], invalid quotes number\n"RESET);
		return (NULL);
	}
}