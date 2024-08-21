NAME = mini

CC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g
LIBFT_PATH = libft
LIBFT = libft/libft.a
SRCS = minishell/lexer.c minishell/token.c minishell/variables.c minishell/ext_meta.c minishell/str_to_token.c
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
