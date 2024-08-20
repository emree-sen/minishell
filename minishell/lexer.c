#include "mini.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void toggle_single_quote(int *quote, char c , int *fl)
{
	if(*fl == -1 && c == '\"')
		*fl = (int)c;
	else if (*fl == c)
		*fl = -1;
	
	if (*fl == -1 && *quote == -1 && c == '\'')
		*quote = (int)c;
	else if (*quote == c)
		*quote = -1;
}

t_variables *dup_veriables(char **environ)
{
	t_variables *root;
	t_variables *variables;
	int i;
	char *key;
	char *value;

	i = -1;
	root = NULL;
	while (environ[++i])
	{
		key = ft_substr(environ[i], 0, ft_strchr(environ[i], '=') - environ[i]);
		value = ft_strdup(ft_strchr(environ[i], '=') + 1);
		variables = variables_new(key, value);
		variables_add_last(&root, variables);
	}
	return (root);
}

void token_value_checker(t_variables *var_tmp, char *key, char **value)
{
	if (!ft_strcmp(var_tmp->key, key))
	{
		*value = ft_strdup(var_tmp->value);
		return ;
	}
}

int is_has_space(char *str)
{
	int i;
	int flag;

	flag = -1;
	i = -1;
	while (str[++i])
	{
		toggle_quote(&flag, str[i]);
		if (flag == -1 && str[i] == ' ')
			return (1);
	}
	return (0);
}

void token_extract_sp_creator(t_token *tmp, t_token **token_root, t_token *new, int i)
{
	int start;
	
	while (tmp->str[i])
	{
		if(tmp->str[i] == ' ')
			i++;
		else
		{
			start = i;
			while (tmp->str[i] && tmp->str[i] != ' ')
				i++;
			new = token_new(ft_substr(tmp->str, start, i - start), NONE);
			token_add_prev(token_root, tmp, new);
		}
	}
}

void token_extract_spaces(t_token **token_root)
{
	t_token *tmp;
	t_token *new;
	int i;

	i = 0;
	tmp = *token_root;
	new = NULL;
	while (tmp)
	{
		if(is_has_space(tmp->str))
		{
			token_extract_sp_creator(tmp, token_root, new, 0);
			if(tmp->prev)
			{
				tmp = tmp->prev;
				token_del(tmp->next);
			}
		}
		else
		{
			tmp = tmp->next;
		}
	}
}



char *token_value_finder(t_token *tmp, t_dollar *dollar, t_variables *var_root)
{
	int start;
	int end;
	char *value;
	t_variables *var_tmp;

	value = NULL;
	start = dollar->i;
	end = dollar->i;
	end++;
	if (ft_isdigit(tmp->str[end]) == 1)
		end++;
	else
	{
		while (tmp->str[end] && (ft_isalnum(tmp->str[end]) || tmp->str[end] == '_' || tmp->str[end] == '?'))
			end++;					
	}
	dollar->key = ft_substr(tmp->str, start + 1, end - start - 1);
	var_tmp = var_root;
	while (var_tmp)
	{
		token_value_checker(var_tmp, dollar->key, &value);
		var_tmp = var_tmp->next;
	}
	return (value);
}

void token_split_dollars(t_token **token_root, t_variables *var_root, t_state *state)
{
	t_token *tmp;
	t_dollar dollar;

	dollar.value = NULL;
	dollar.flag = -1;
	dollar.flag2 = -1;
	tmp = *token_root;
	while (tmp)
	{
		dollar.i = -1;
		while (tmp->str[++dollar.i])
		{
			toggle_single_quote(&dollar.flag, tmp->str[dollar.i], &dollar.flag2);
			if (dollar.flag == -1 && tmp->str[dollar.i] == '$')
			{
				dollar.value = token_value_finder(tmp, &dollar, var_root);
				token_replace_value(&tmp, &dollar, &dollar.i, state);
			}			
		}

		tmp = tmp->next;
	}
	token_extract_spaces(token_root);
}



void token_replace_value(t_token **str, t_dollar *dollar, int *i, t_state *state)
{
	int end;
	char *left;
	char *right;

	end = *i + ft_strlen(dollar->key);
	left = ft_substr((*str)->str, 0, *i);
	right = ft_substr((*str)->str, end + 1, ft_strlen((*str)->str) - end);
	if(!dollar->value)
	{
		if(ft_strcmp(dollar->key, "?") == 0)
			dollar->value = ft_itoa(state->status);
		else if(ft_strcmp(dollar->key, "0") == 0)
			dollar->value = ft_strdup("minishell");
		else
			dollar->value = ft_strdup("");
		*i = *i - 1; 
	}
	else
		dollar->value = ft_strdup(dollar->value);
	(*str)->str = ft_strjoin(left, dollar->value);
	(*str)->str = ft_strjoin((*str)->str, right);
	free(left);
	free(right);
}




int main()
{
	while (1)
	{
		t_state state;
		state.status = 0;
		extern char **environ;
		char **env = environ;

		char *line;
		t_token *root;
		t_variables *var_root;
		line = readline("minishell$ ");
		add_history(line);
		var_root = dup_veriables(env);
		
		// variables_list_printer(var_root);

		root = str_to_token(line);

		token_extract_all_meta(&root);
		token_split_dollars(&root, var_root, &state);

		token_list_printer(root);
	}
}