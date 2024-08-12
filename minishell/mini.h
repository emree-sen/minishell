#ifndef MINI_H
# define MINI_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

typedef enum e_token_type
{
	NONE,
	CMD,
	PIPE,
	ARG,
	REDR,
	REDL,
	REDRR,
	REDLL,
	HEREDOC,
	FILEE
} t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;
} t_token;

typedef struct s_variables
{
	char				*key;
	char				*value;
	struct s_variables	*next;
} t_variables;

//hazırlık
t_token *token_new(char *str, t_token_type type);
void token_del(t_token *token);
void token_add_last(t_token **root, t_token *token);
void token_add_next(t_token *token, t_token *new);
void token_list_printer(t_token *root);
void token_add_prev(t_token **root, t_token *token, t_token *new);
void token_add_start(t_token **root, t_token *token);


t_variables *variables_new(char *key, char *value);
void variables_list_printer(t_variables *root);
void variables_add_last(t_variables **root, t_variables *variables);


int pass_quote(char *str, int qi);
int pass_str(char *str, int si);

void toggle_quote(int *quote, char c);

//stryi alıp GERÇEK boşluklara göre link list'e yerleştirecek, halkın adamı!
// tokeni fonksiyonda oluştur. 
t_token	*str_to_token(char *str); 

// str_to_token'den gelen link list'i meta karakterlere göre değiştirecek
int is_has_meta(char *str);
int is_only_meta(char *str);
void token_extract_all_meta(t_token **token_root);
int token_extract_metas(t_token *tmp, t_token **token_root,t_token *new,int i);
void token_extract_creator(t_token *tmp, t_token **token_root,t_token *new,int i);

// dolar işaretlerini değişken içerikleriyle değiştir. Bunun için variables structını kullan 
// bu işlemden sonra gerçek boşluk ayırmaya terkar ihtiyaç olabileceğinden root pointer'ı verilir
void token_split_dollars(t_token **token_root, t_variables *var_root);
t_variables *dup_veriables(char **environ);


// gereksiz tırnakları temizler
void token_del_quote(t_token *token_root);
// token listesini pipe + 1 parçaya ayırıp listesini döner
t_token **token_seperate_by_pipe(t_token *token_root);
// token listesini anlamlı parçalar olarak tanımlar
void token_arr_set_type(t_token **token_arr);



#endif
