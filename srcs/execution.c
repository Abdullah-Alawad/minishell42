#include "../minishell.h"

int	execute_builtin_cmd(t_command *cmd, t_env_list **env)
{
	int	res;

	res = 0;
	if (!cmd || !cmd->av)
		return (0);
	if (ft_strncmp("echo", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_echo(cmd->av);
	else if (ft_strncmp("env", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_env(env);
	else if (ft_strncmp("pwd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_pwd();
	else if (ft_strncmp("cd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_cd(cmd->av, *env);
	else
		printf("builtin not handled\n");
	return (res);
}

void	execute_command(t_command *cmds, t_env_list *env_lst)
{
	t_command	*cmd;
	int			status;

	cmd = cmds;
	status = 0;
	while (cmd)
	{
		if (cmd->is_builtin)
			status = execute_builtin_cmd(cmd, &env_lst);
		else
			printf("external command\n");
		cmd = cmd->next;
	}
	printf("status: %d\n",status);
}