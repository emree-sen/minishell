NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address 
READLINE = -L./lib/readline/lib -I./lib/readline/include/readline -lreadline
RL = ./lib/readline/lib/libreadline.a
RM = rm -rf
DIR     = $(shell echo $(PWD))

LIB_SRC = lib/ft_isalnum.c lib/ft_isalpha.c lib/ft_isdigit.c lib/ft_isascii.c lib/ft_isprint.c \
		lib/ft_strchr.c lib/ft_strlcat.c lib/ft_strlen.c lib/ft_strncmp.c lib/ft_strnstr.c \
		lib/ft_strrchr.c lib/ft_tolower.c lib/ft_toupper.c lib/ft_strlcpy.c lib/ft_atoi.c \
		lib/ft_memset.c lib/ft_memcpy.c lib/ft_bzero.c lib/ft_memchr.c lib/ft_memcmp.c \
		lib/ft_memmove.c lib/ft_calloc.c lib/ft_strdup.c lib/ft_substr.c lib/ft_strjoin.c \
		lib/ft_putchar_fd.c lib/ft_putstr_fd.c lib/ft_putendl_fd.c lib/ft_putnbr_fd.c \
		lib/ft_striteri.c lib/ft_strmapi.c lib/ft_strtrim.c lib/ft_split.c lib/ft_itoa.c \
		lib/ft_strcmp.c\

SRC = src/lexer.c src/token.c src/token2.c src/set_type.c \
		src/token_sep_by_pipe.c src/token_sep_by_pipe2.c src/variables.c \
 		src/quote_cleaner.c src/ext_meta.c src/after_space_cont.c \
		src/ext_meta2.c src/split_dollar.c src/str_to_token.c \
		src/syntax_check.c src/syntax_error.c src/pipe_redir_check.c \
		src/mixed_redir_check.c src/executor.c src/commands.c src/exec.c \
		src/fds.c src/heredoc.c src/built_in.c src/exec_fill.c \
		src/ft_error.c src/built_in2.c src/built_in3.c src/commands2.c \
		src/commands3.c src/split_dollar2.c src/init_meta.c \
		src/exec_fill2.c src/exec2.c src/free.c src/built_in4.c \
		src/split_dollar3.c src/main.c src/executor2.c src/signals.c\

OBJS = $(SRC:.c=.o) $(LIB_SRC:.c=.o)

all: $(NAME)

$(NAME): $(RL) $(OBJS) 
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE) 

$(RL):
		@echo "Downloading readline"
		@curl -O https://ftp.gnu.org/gnu/readline/readline-8.2-rc1.tar.gz
		@tar -xvf readline-8.2-rc1.tar.gz
		@$(RM) readline-8.2-rc1.tar.gz
		@cd readline-8.2-rc1 && ./configure --prefix=$(DIR)/lib/readline && make && make install
		@$(RM) readline-8.2-rc1
		@ echo "Readline installed"

RLclean:
		@$(RM) lib/readline
		@echo "Readline removed"

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re