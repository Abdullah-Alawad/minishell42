#include "../minishell.h"

t_ttype	token_type(char *data)
{
	if (!data || !*data)
		return (T_DATA);
	else if (data[0] == '<' && data[1] && data[1] == '<')
		return (T_HEREDOC);
	else if (data[0] == '>' && data[1] && data[1] == '>')
		return (T_APPEND);
	else if (data[0] == '<')
		return (T_REDIRECT_IN);
	else if (data[0] == '>')
		return (T_REDIRECT_OUT);
	else if (data[0] == '|')
		return (T_PIPE);
	return (T_DATA);
}

t_token	*new_token(char	*data, t_ttype t_type, t_qtype q_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->data = data;
	token->type = t_type;
	token->quote_type = q_type;
	token->next = NULL;
	return (token);
}

t_token	*lst_last(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	lst_add_back(t_token **tokens_list, t_token *token)
{
	if (!tokens_list)
		return ;
	if (!*tokens_list)
		*tokens_list = token;
	else
		(lst_last(*tokens_list))->next = token;
}

int	add_to_list(char *command, int start, int end, t_token **tokens_list, t_qtype q_type)
{
	t_token	*token;
	char	*data;
	t_ttype	t_type;

	data = ft_substr(command, start, end - start);
	if (!data)
	{
		free_tokens(tokens_list);
		return (0);
	}
	t_type = token_type(data);
	token = new_token(data, t_type, q_type);
	if (!token)
	{
		free(data);
		free_tokens(tokens_list);
		return (0);
	}
	lst_add_back(tokens_list, token);
	return (1);
}