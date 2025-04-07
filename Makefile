NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra

RM = rm -f

SRC_DIR = srcs
OBJ_DIR = objs

SRCS = ${SRC_DIR}/minishell.c ${SRC_DIR}/lexer.c ${SRC_DIR}/utils1.c ${SRC_DIR}/utils2.c \
		${SRC_DIR}/parse_tokens.c ${SRC_DIR}/env_vars.c ${SRC_DIR}/utils3.c
OBJS = ${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c minishell.h
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

all: libft ${NAME}

libft:
	make -C libft

${NAME}: ${OBJS}
		${CC} ${CFLAGS} ${MAIN_OBJ} ${OBJS} -Llibft -lft -lreadline -o ${NAME}

clean:
		${RM} ${OBJS}
		rm -rf ${OBJ_DIR}
		make -C libft fclean

fclean: clean
		${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re libft