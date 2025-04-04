#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define FOREVER	1
# define GREEN		"\033[0;32m"
# define RED		"\033[0;31m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define AQUA		"\033[0;36m"
# define RESET		"\033[0m"

typedef enum e_ttype
{
	T_TEXT,
	T_PIPE,
	T_REDIRECT_IN,  // <
	T_REDIRECT_OUT, // >
	T_APPEND,		// >>
	T_HEREDOC,		// <<
}	t_ttype;


void	handle_command(char *command);

#endif