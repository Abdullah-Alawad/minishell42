#include "minishell.h"

void	add_to_list(char *command, int start, int end, t_token **tokens_list)
{
	t_token	*token;
	char	*text;
	t_qtype	q_type;

	if (command[i] == '\"')
		q_type = DOUBLE_Q;
	else if (command[i] == '\'')
		q_type = SINGLE_Q;
	else
		q_type = NO_Q;
	text = ft_substr(command, start, end - start);
	if (!text)
		// failed memory allocation function
}