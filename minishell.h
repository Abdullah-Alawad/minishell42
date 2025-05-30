#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <readline/readline.h>
# include <fcntl.h>
# include <readline/history.h>
# include <sys/wait.h>

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
	char				*in_file;	//file name if < or <<
	char				**here_arr;
	char				*out_file;	//file name if > or >>
	int					in_fd;
	int					out_fd;
	int					pipe;		// 0 -> command will not be piped / 1 -> will be piped
	int					heredoc; // 0 -> <  / 1 -> << 
	int					append;  // 1 -> > / 2 -> >>
	int					is_builtin;
	struct s_command	*next;
}	t_command;	

typedef struct s_env_list
{
	char				*key;
	char				*data;
	int					exported;
	struct s_env_list	*next;
}	t_env_list;



t_command	*handle_command(char *command);
int			handle_quotes(char *command, int start, t_token **tokens_list);
int			add_to_list(char *command, int start, int end, t_token **tokens_list, t_qtype q_type);
void		lst_add_back(t_token **tokens_list, t_token *token);
t_qtype		quote_type(char q);
int			check_tokens(t_token *tokens_list);
int			parse_tokens(t_token *tokens, t_command **cmds);
t_command	*cmd_create(void);
char		*get_env(char *data);
void		cmds_add_back(t_command **cmds_list, t_command *cmd);
void		free_tokens(t_token **tokens_list);
void		free_commands(t_command **cmds);
void		free_av(char **s);
t_env_list	*create_env_list(char **env);
t_env_list	*init_env(char *str, int status);
void		env_add_back(t_env_list **lst, t_env_list *env);
void		free_env_list(t_env_list **env);
void    	execute_command(t_command *cmds, t_env_list **env_lst);
int 		handle_echo(char **cmd);
int			handle_env(t_env_list **env);
int			handle_pwd(void);
int			handle_cd(char **cmd, t_env_list **env);
int			handle_exit(t_env_list **env, t_command *cmd, int status);
int			handle_export(char **cmd, t_env_list **env);
int			handle_unset(char **cmd, t_env_list **env);
int			execute_external(t_command *cmd, t_env_list **env);
int 		open_heredocs(t_command *cmd);
int			open_redirections(t_command *cmd);
void		redirect_io(t_command *cmd, int *saved_stdin, int *saved_stdout);
void		restore_io(int saved_stdin, int saved_stdout);

#endif