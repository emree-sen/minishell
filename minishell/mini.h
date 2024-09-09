#ifndef MINI_H
# define MINI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <termios.h>

# define ERR_CMD_NOT_FOUND 127
# define ERR_IS_A_DIRECTORY 126
# define ERR_PERMISSION_DENIED 1261
# define ERR_NUMERIC_ARG 255
# define ERR_PIPE_INIT 124
# define ERR_NOT_VALID_IDFR 11
# define ERR_NOT_A_DIRECTORY 122
# define ERR_NO_FILE_OR_DIR 1
# define ERR_EMPTY_COMMAND 1271
# define ERR_UNEXPECTED_TOKEN 258
# define ERR_MALLOC 3
# define ERR_UNCOMPLETED_REDIRECT 4
# define SUCCESS 0

# define BUILTIN 37
# define YES_CMD 38
# define NO_CMD 39

typedef enum e_token_type
{
	NONE,
	CMD,
	ARG,
	PIPE,
	REDR,
	REDL,
	REDRR,
	REDLL,
	HEREDOC,
	FILEE
}	t_token_type;

typedef struct s_token
{
	char				*str;
	t_token_type		type;
	struct s_token		*prev;
	struct s_token		*next;
}	t_token;

typedef struct s_variables
{
	char				*line;
	char				*key;
	char				*value;
	struct s_variables	*prev;
	struct s_variables	*next;
}	t_variables;

typedef struct s_dollar
{
	int			i;
	char		*key;
	char		*value;
	int			flag;
	int			flag2;

}	t_dollar;

typedef struct s_state
{
	int			status;
	t_token		**token_arr;
	int			arr_len;
}	t_state;

typedef struct s_heredocs
{
	int			*heredoc_fds;
}	t_heredocs;

typedef struct s_exec
{
	char	*path; // -> /bin/ls
	char	**args; // -> ["/bin/ls", "-l", "-a", NULL]
	char	*input_file; // -> NULL, file.txt
	char	*output_file; // -> NULL, file.txt
	int		in_fd; // -> 0, 3
	int		out_fd; // -> 1, 4
	int		output_type; // -> file_input, herdoc, none
	char	**heredocs; // -> {"a", "b", NULL}
	int		heredoc_idx;
	int		in_type;
	int 	cmd_type;

	int		err_val;
	char	*err_str;
}	t_exec;
// execve("/bin/ls", ["/bin/ls", "-l", "-a", NULL], envp);

//hazırlık
t_token		*token_new(char *str, t_token_type type);
void		token_del(t_token *token);
void		token_add_last(t_token **root, t_token *token);
void		token_add_next(t_token *token, t_token *new);
void		token_list_printer(t_token *root);
void		token_add_prev(t_token **root, t_token *token, t_token *new);
void		token_add_start(t_token **root, t_token *token);

t_variables	*variables_new(char *key, char *value);
void		variables_list_printer(t_variables *root);
void		variables_add_last(t_variables **root, t_variables *variables);
void		variables_del(t_variables *var);

int			pass_quote(char *str, int qi);
int			pass_str(char *str, int si);
int			pass_str_pls(char *str, int i);
int			pass_any(char *str, int *ai, char any);

void		toggle_quote(int *quote, char c);
void		toggle_single_quote(int *quote, char c, int *dquote);
int			is_only_quote(char *str);
//stryi alıp GERÇEK boşluklara göre link list'e yerleştirecek, halkın adamı!
// tokeni fonksiyonda oluştur. 
t_token		*str_to_token(char *str);

// str_to_token'den gelen link list'i meta karakterlere göre değiştirecek
int			is_has_meta(char *str);
int			is_only_meta(char *str);
void		token_extract_all_meta(t_token **token_root);
int			token_extract_metas(t_token *tmp, t_token **token_root,
				t_token *new, int i);
void		token_extract_creator(t_token *tmp, t_token **token_root,
				t_token *new, int i);
void		token_meta_type_changer(t_token *tmp, int i);
void		token_new_add_prev(t_token **token_root, t_token *tmp,
				t_token *new, int *i);

/* dolar işaretlerini değişken içerikleriyle değiştir.
Bunun için variables structını kullan 
bu işlemden sonra gerçek boşluk ayırmaya tekrar ihtiyaç
olabileceğinden root pointer'ı verilir*/

void		token_split_dollars(t_token **token_root,
				t_variables *var_root, t_state *state);
t_variables	*dup_veriables(char **environ);
void		token_replace_value(t_token **token_root,
				t_dollar *dollar, int *i, t_state *state);
char		*token_value_finder(t_token *tmp, t_dollar *dollar,
				t_variables *var_root);
void		token_value_checker(t_variables *var_tmp, char *key, char **value);

//dolar sonrası space kontrolü
void		token_extract_spaces(t_token **token_root);
void		token_extract_sp_creator(t_token *tmp,
				t_token **token_root, t_token *new, int i);

// gereksiz tırnakları temizler
void		token_del_quote(t_token *token_root);
void		token_quote_detective(t_token *tmp);
char		*token_dup_quote(t_token *tmp, int *flag, int i, int j);
int			is_has_quote(char *str);

// token listesini pipe + 1 parçaya ayırıp listesini döner
t_token		*copy_token_list(t_token *start, t_token *end);
t_token		**add_to_token_list_array(t_token **token_list_array,
				t_token *new_list, int *size);
t_token		**finalize_token_list_array(t_token **list_array, int size);
void		process_tokens(t_token *start, t_token *current,
				t_token ***separated_lists, int *size);
t_token		**finalize_token_array(t_token **list_array, int size);
t_token		**token_separate_by_pipe(t_token *token_root);
// token listesini anlamlı parçalar olarak tanımlar
void		set_token_type(t_token *token, int *flag);
void		handle_redirection(t_token **token);
void		token_arr_set_type(t_token **token_arr);

//syntax kontrol
int			check_the_syntax(char *input);
int			is_pipe_first(char *input);
int			is_pipe_last(char *input);
int			double_pipe(char *input);
int			print_syntax_error_quote(void);
int			print_syntax_error_pipe(void);
int			print_unexpected_char_error(void);
int			pass_the_quotes(char c, int quote);
int			print_syntax_error_redir(void);
int			redirect_check(char *input);
int			pass_the_spaces(char *input, int i);
int			redir_plus_pipe(char *input);
int			redir_plus_pipe_two(char *input);
int			mixed_redir(char *input);
int			mixed_redir_two(char *input);
int			mixed_redir_three(char *input);
int			mixed_redir_four(char *input);
int			last_arg_is_redir(char *input);

// exec
void		state_arr_len_set(t_state *state);
int			*heredoc_create(t_state *state);
void		heredoc_setter(t_exec *exec);
t_exec		**exec_create(t_state *state);
void		ft_free_split(char **arr);
int			is_has_slash(char *str);
char		*ft_getenv(char *key, t_variables *var_root);
char		*path_finder(char *cmd, t_variables *var_root);
int			arg_num_finder(t_token *tmp);
char		**args_filler(t_token *tmp, char *path);
int			count_heredocs(t_token *tmp);
t_exec		**exec_filler(t_state *state, t_variables *var_root);
void		exec_print(t_exec **exec);
int			**fds_filler(int **fds, t_state *state);
void		fd_closer(int **fds, int i, t_state *state);
void		close_all_fd(int **fds, t_state *state);
void		wait_all_children(int arr_len);
void		ft_print_exec_errors(t_exec **exec);
char		**env_list_creator(t_variables *var_root);
void		single_command(t_exec **exec, int i);
void		fd_setter_without_redr(int **fds, int i, t_state *state);
void		fd_setter_with_redr(t_exec **exec, int **fds, int i, t_state *state);
void		multi_command_without_redr(int **fds, int i, t_state *state);
void		multi_command_with_redr(t_exec **exec, int i, int **fds, t_state *state);
void		executor(t_state *state, t_variables *var_root);
void		exec_init(t_exec *exec);
int 		is_built_in(t_token *tmp);
void		ft_export(t_state *state, t_variables *var_root, int i);
void		single_command_built_in(t_exec **exec, t_state *state, t_variables *var_root, int i);
void		ft_env(t_variables *var_root);
void		ft_unset(t_variables *var_root, t_state *state, int i);
void 		ft_pwd();

#endif
