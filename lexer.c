#include "minishell.h"

int	good_quotes(char *command)
{
	char	single_quote;
	char	double_quote;
	int		i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (command[i])
	{
		if (command[i] == '\"')
		{
			if (double_quote == 0)
				double_quote = command[i];
			else if (double_quote == command[i])
				double_quote = 0;			
		}
		else if (command[i] == '\'')
		{
			if (single_quote == 0)
				single_quote = command[i];
			else if (single_quote == command[i])
				single_quote = 0;			
		}
		command++;
	}
	if (!double_quote && !single_quote)
		return (1);
	else
		return (0);
}

void	handle_command(char *command)
{
	if(good_quotes(command))
	{
		printf("%s\n", command);
	}
	else
	{
		printf(RED"[ERROR]invalid quotes number\n"RESET);
		return ;
	}
}