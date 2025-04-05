#include "../minishell.h"

t_qtype	quote_type(char q)
{
	if (q == '\"')
		return (DOUBLE_Q);
	else if (q == '\'')
		return (SINGLE_Q);
	return (NO_Q);
}

int	handle_quotes(char *command, int start, t_token **tokens_list)
{
	int		i;
	char	q;
	t_qtype	q_type;

	q = command[start];
	q_type = quote_type(q);
	start += 1;
	i = start;
	while (command[i] && command[i] != q)
		i++;
	add_to_list(command, start, i, tokens_list, q_type);
	i++;
	return (i);
}

int	check_tokens(t_token *tokens_list)
{
	if (!tokens_list)
		return (0);
	if (tokens_list->type != T_DATA)
		return (0);
	while (tokens_list)
	{
		if (tokens_list->type != T_DATA)
		{
			if (tokens_list->next == NULL)
				return (0);
			if (tokens_list->next->type != T_DATA)
				return (0);
		}
		tokens_list = tokens_list->next;
	}
	return (1);
}