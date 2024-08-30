NAME = mini

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address -g
LIBFT_PATH = libft
LIBFT = libft/libft.a
SRCS = minishell/lexer.c minishell/token.c minishell/token2.c minishell/set_type.c \
		minishell/token_sep_by_pipe.c minishell/token_sep_by_pipe2.c minishell/variables.c \
 		minishell/quote_cleaner.c minishell/ext_meta.c minishell/after_space_cont.c \
		minishell/ext_meta2.c minishell/split_dollar.c minishell/str_to_token.c \
		minishell/syntax_check.c minishell/syntax_error.c minishell/pipe_redir_check.c \
		minishell/mixed_redir_check.c
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
