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
	char				*key;
	char				*value;
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
	int 		arr_len;
}	t_state;

typedef struct s_heredocs
{
	int *heredoc_fds;
}	t_heredocs;

typedef struct s_exec
{
	char	*path; // -> /bin/ls
	char	**args; // -> ["/bin/ls", "-l", "-a", NULL]
	char	*input_file; // -> NULL, file.txt
	char	*output_file; // -> NULL, file.txt
	int		in_fd; // -> 0, 3
	int		out_fd; // -> 1, 4
	int		input_type; // -> file_input, herdoc, none
	char	**heredocs; // -> {"a", "b", NULL}
	int		heredoc_idx;
	// int		err_val;
	// char	*err_str;
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
void	set_token_type(t_token *token, int *flag);
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
void 		state_arr_len_set(t_state *state);
int 		*heredoc_create(t_state *state);
void		executor(t_state *state, t_variables *var_root);

#endif
