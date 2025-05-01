#include "../minishell.h"

int	execute_builtin_cmd(t_command *cmd, t_env_list **env, int res)
{
	if (!cmd || !cmd->av)
		return (0);
	if (ft_strncmp("echo", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_echo(cmd->av);
	else if (ft_strncmp("env", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_env(env);
	else if (ft_strncmp("pwd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_pwd();
	else if (ft_strncmp("cd", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_cd(cmd->av, env);
	else if (ft_strncmp("exit", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_exit(env, cmd, res);
	else if (ft_strncmp("export", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_export(cmd->av, env);
	else if (ft_strncmp("unset", cmd->av[0], ft_strlen(cmd->av[0])) == 0)
		res = handle_unset(cmd->av, env);
	return (res);
}

void	execute_command(t_command *cmds, t_env_list **env_lst)
{
	t_command			*cmd;
	static int			status;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->is_builtin)
			status = execute_builtin_cmd(cmd, env_lst, status);
		else
			printf("external command\n");
		cmd = cmd->next;
	}
	printf("status: %d\n",status);
}
