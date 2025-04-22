# include "../minishell.h"

void	print_command_list(t_command *cmd)
{
	int i;

	while (cmd)
	{
		printf("Arguments:\n");
		if (cmd->av)
		{
			i = 0;
			while (cmd->av[i])
			{
				printf("  av[%d]: %s\n", i, cmd->av[i]);
				i++;
			}
		}
		printf("Input File: %s\n", cmd->in_file ? cmd->in_file : "NULL");
		printf("Output File: %s\n", cmd->out_file ? cmd->out_file : "NULL");
		printf("Pipe: %d\n", cmd->pipe);
		printf("Heredoc: %d\n", cmd->heredoc);
		printf("Append: %d\n", cmd->append);
		printf("Is Builtin: %d\n", cmd->is_builtin);
		printf("----------------------\n");
		cmd = cmd->next;
	}
}


int main(int ac, char **av, char **env)
{
	char		*command;
	t_env_list	*env_lst;
	t_command	*cmds_list;

	(void)av;
	(void)ac;
	env_lst = create_env_list(env); // NOTE: need to be freed
	if (!env_lst)
		return (1);
	while (FOREVER)
	{
		command = readline(GREEN"A10-shell"RESET"$ ");
		//printf("%s\n", command);
		if (!command)
		{
			rl_clear_history();
			exit(1);
		}
		add_history(command);
		cmds_list = handle_command(command);
		//print_command_list(cmds_list);
		execute_command(cmds_list, env_lst);
		free_commands(&cmds_list);
		free(command);
		// if (env_lst)
		// 	free_env_list(&env_lst);
	}
	return (0);
}