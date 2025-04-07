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
	T_DATA,
	T_PIPE,
	T_REDIRECT_IN,  // <
	T_REDIRECT_OUT, // >
	T_APPEND,		// >>
	T_HEREDOC,		// <<
}	t_ttype;

typedef enum e_qtype
{
	NO_Q,
	SINGLE_Q,
	DOUBLE_Q,
}	t_qtype;

typedef struct s_token
{
	char			*data;
	t_ttype			type;
	t_qtype			quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**av;
	char				*in_file;
	char				*out_file;
	int					pipe;
	int					heredoc;
	int					append;
	int					is_builtin;
	struct s_command	*next;
}	t_command;	


void		handle_command(char *command);
int			handle_quotes(char *command, int start, t_token **tokens_list);
void		add_to_list(char *command, int start, int end, t_token **tokens_list, t_qtype q_type);
void		lst_add_back(t_token **tokens_list, t_token *token);
t_qtype		quote_type(char q);
int			check_tokens(t_token *tokens_list);
int			parse_tokens(t_token *tokens, t_command **cmds);
t_command	*cmd_create(void);
char		*get_env(char *data);
void		cmds_add_back(t_command **cmds_list, t_command *cmd);





#endif