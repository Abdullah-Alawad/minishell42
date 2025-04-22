#include "../minishell.h"

int	handle_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putchar_fd('\n', 1);
		free(cwd);
	}
	else
		perror("get cwd failed\n");
	return (0);
}