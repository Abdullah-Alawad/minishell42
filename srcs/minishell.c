# include "../minishell.h"

int main(int ac, char **av, char **env)
{
	char		*command;
	t_env_list	*env_lst;

	(void)av;
	(void)ac;
	env_lst = create_env_list(env); // NOTE: need to be freed
	if (!env_lst)
		return (1);
	while (FOREVER)
	{
		command = readline(GREEN"A10-shell"RESET"$ ");
		printf("%s\n", command);
		if (!command)
		{
			rl_clear_history();
			exit(1);
		}
		add_history(command);
		handle_command(command);
		free(command);
	}
	return (0);
}