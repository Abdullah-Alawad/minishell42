# include "../minishell.h"

int main(void)
{
	char	*command;

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