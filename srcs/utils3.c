#include "../minishell.h"

t_command	*cmds_lst_last(t_command *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	cmds_add_back(t_command **cmds_list, t_command *cmd)
{
	if (!cmds_list)
		return ;
	if (!*cmds_list)
		*cmds_list = cmd;
	else
		(cmds_lst_last(*cmds_list))->next = cmd;
}