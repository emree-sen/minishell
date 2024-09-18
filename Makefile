NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address 
LIBFT_PATH = libft
LIBFT = libft/libft.a
SRCS = src/lexer.c src/token.c src/token2.c src/set_type.c \
		src/token_sep_by_pipe.c src/token_sep_by_pipe2.c src/variables.c \
 		src/quote_cleaner.c src/ext_meta.c src/after_space_cont.c \
		src/ext_meta2.c src/split_dollar.c src/str_to_token.c \
		src/syntax_check.c src/syntax_error.c src/pipe_redir_check.c \
		src/mixed_redir_check.c src/executor.c src/commands.c src/exec.c \
		src/fds.c src/heredoc.c src/built_in.c src/exec_fill.c \
		src/ft_error.c src/built_in2.c src/built_in3.c src/commands2.c \
		src/commands3.c src/split_dollar2.c src/init_meta.c \
		src/exec_fill2.c src/exec2.c src/free.c src/built_in4.c \
		src/split_dollar3.c src/main.c src/executor2.c\

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

run: all
	./$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	@make -s -C $(LIBFT_PATH)

clean:
	@$(RM) $(OBJS)
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_PATH) fclean

re: fclean all
