#include "minishell.h"

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
	add_to_list(command, start, i, tokens_list, q_type);
	return (i);
}

void	lexer(char *command, t_token **tokens_list)
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
			i = check_operator(command, i, tokens_list);
		else if (command[i] && (command[i] == '\"' || command[i] == '\''))
			i = handle_quotes(command, i, tokens_list);
		else
		{
			start = i;
			q_type = quote_type(command[start]);
			while (command[i] && command[i] != ' ' && !is_operator(command[i]))
				i++;
			add_to_list(command, start, i, tokens_list, q_type);
		}
	} 
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

void	handle_command(char *command)
{
	t_token	*tokens_list;

	tokens_list = NULL;
	if(good_quotes(command))
	{
		lexer(command, &tokens_list);
		t_token	*current = tokens_list;
		while (current)
		{
			printf("Token: %-10s | Type: %d | Quote: %d\n", 
				current->data, current->type, current->quote_type);
			current = current->next;
		}
	}
	else
		printf(RED"[ERROR]invalid quotes number\n"RESET);
}