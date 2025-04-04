NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra

RM = rm -f

SRCS = lexer.c
MAIN_src = minishell.c

OBJS = ${SRCS:.c=.o}
MAIN_OBJ = minishell.o

%.o:%.c minishell.h
	${CC} ${CFLAGS} -c $< -o $@

all: ${NAME} libft

libft:
	make -C libft

${NAME}: ${MAIN_OBJ} ${OBJS}
		${CC} ${CFLAGS} ${MAIN_OBJ} ${OBJS} -Llibft -lft -lreadline -o ${NAME}

clean:
		${RM} ${MAIN_OBJ} ${OBJS}
		make -C libft fclean

fclean: clean
		${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re libft