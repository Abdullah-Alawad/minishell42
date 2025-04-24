#include "../minishell.h"

int	handle_exit(t_env_list **env, t_command *cmd)
{
	free_commands(&cmd);
	free_env_list(env);
	exit(0); // this is not right yet, exit status should be the status of last executed command
}