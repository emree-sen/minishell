#include "mini.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void toggle_single_quote(int *quote, char c, int *fl)
{
	if (*fl == -1 && c == '\"')
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
		return;
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

int is_has_quote(char *str)
{
	int i;
	int flag;

	flag = -1;
	i = -1;
	while (str[++i])
	{
		toggle_quote(&flag, str[i]);
		if (flag == -1 && (str[i] == '\'' || str[i] == '\"'))
			return (1);
	}
	return (0);
}

void token_extract_sp_creator(t_token *tmp, t_token **token_root, t_token *new, int i)
{
	int start;
	int flag;

	flag = -1;
	while (tmp->str[i])
	{
		toggle_quote(&flag, tmp->str[i]);
		if (flag == -1 && tmp->str[i] == ' ')
			i++;
		else
		{
			start = i;
			if(tmp->str[i] == '\"' || tmp->str[i] == '\'')
				i = pass_quote(tmp->str, i);
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
		if (is_has_space(tmp->str))
		{
			token_extract_sp_creator(tmp, token_root, new, 0);
			if (tmp->prev)
			{
				tmp = tmp->prev;
				token_del(tmp->next);
			}
		}
		else
			tmp = tmp->next;
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
	if (!dollar->value)
	{
		if (ft_strcmp(dollar->key, "?") == 0)
			dollar->value = ft_itoa(state->status);
		else if (ft_strcmp(dollar->key, "0") == 0)
			dollar->value = ft_strdup("minishell");
		else
			dollar->value = ft_strdup("");
		*i = *i - 1;
	}
	else
		dollar->value = ft_strdup(dollar->value); // bu gereksiz mi emin ol
	(*str)->str = ft_strjoin(left, dollar->value);
	(*str)->str = ft_strjoin((*str)->str, right);
	free(left);
	free(right);
}

int is_only_quote(char *str)
{
	char c;
	int i;

	i = 0;
	c = str[0];
	while (str[i])
	{
		if (str[i] != c)
			return (0);
		i++;
	}
	return (1);
}

int real_quote_count(char *str)
{
	int i;
	int count;
	int flag;

	i = 0;
	count = 0;
	flag = -1;
	while (str[i])
	{
		toggle_quote(&flag, str[i]);
		if (flag != -1)
			count++;
		i++;
	}
	return (count);
}

int quote_count(char *str)
{
	int i;
	int count;
	int flag;

	i = 0;
	count = 0;
	flag = -1;
	while (str[i])
	{
		toggle_quote(&flag, str[i]);
		if (flag != -1)
			count++;
		i++;
	}
	return (count);
}

char  *token_dup_quote(t_token *tmp, int *flag, int i, int j)
{
	char *str;

    str = (char *)malloc(sizeof(char) * (ft_strlen(tmp->str) + 1));
	while (tmp->str[i])
    {
		while(tmp->str[i] && ((tmp->str[i] != '\'' && tmp->str[i] != '\"')))
		{
			str[j++] = tmp->str[i++];
		}
		if(tmp->str[i] == '\"' || tmp->str[i] == '\'')
			toggle_quote(flag, tmp->str[i++]);
		while(*flag != -1 && tmp->str[i])
		{				
			toggle_quote(flag, tmp->str[i]);
			if(*flag != -1)
				str[j++] = tmp->str[i++];	
			else
				i++;
		}
    }	
	str[j] = '\0';

	return (str);
}

void token_quote_detective(t_token *tmp)
{
	char *str;
    int flag;

    flag = -1;
	str = token_dup_quote(tmp, &flag, 0 , 0);
	tmp->str = ft_strdup(str);
}


void token_del_quote(t_token *token_root)
{
	t_token *tmp;
	int i;

	i = 0;
	tmp = token_root;
	while (tmp)
	{
		if (is_has_quote(tmp->str))
			token_quote_detective(tmp);
		tmp = tmp->next;
	}
}

void print_state(t_state *state)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (state->token_arr[j][i].str)
	{
		printf("%s\n", state->token_arr[j][i].str);
		i++;
	}
}

t_token *copy_token_list(t_token *start, t_token *end)
{
    t_token *new_list;
    t_token *current;
    t_token *last;
	t_token *new_token;

	current = start;
	new_list = NULL;
	last = NULL;
    while (current != end) 
	{
        new_token = token_new(current->str, current->type);
        if (!new_token)
            return NULL;
        if (!new_list)
            new_list = new_token;
        else 
		{
            last->next = new_token;
            new_token->prev = last;
        }
        last = new_token;
        current = current->next;
    }
    return new_list;
}

t_token **add_to_token_list_array(t_token **token_list_array, 
	t_token *new_list, int *size)
{
    t_token **new_array;
    int i;
	
	new_array = malloc(sizeof(t_token *) * (*size + 1));
	i = 0;
	if (!new_array) 
        return NULL;

    // Eski listeyi yeniye kopyala
    while (i < *size) 
	{
        new_array[i] = token_list_array[i];
		i++;
	}

    // Yeni listeyi ekle
    new_array[*size] = new_list;
    (*size)++;

    // Eski listeyi serbest bırak
    free(token_list_array);

    return new_array;
}

t_token **finalize_token_list_array(t_token **list_array, int size)
{
	int i;

	i = 0; 
    t_token **final_list;
	final_list = malloc(sizeof(t_token *) * (size + 1));
    if (!final_list)
        return NULL;
    
    while (i < size) 
	{
        final_list[i] = list_array[i];
		i++;
	}
    final_list[size] = NULL;
    free(list_array);
    return final_list;
}

t_token	**finalize_token_array(t_token **list_array, int size)
{
	t_token	**final_list;
	int		i;

	final_list = malloc(sizeof(t_token *) * (size + 1));
	if (!final_list)
		return (NULL);
	i = 0;
	while (i < size)
	{
		final_list[i] = list_array[i];
		i++;
	}
	final_list[size] = NULL;
	free(list_array);
	return (final_list);
}

void	process_tokens(t_token *start, t_token *current, 
	t_token ***separated_lists, int *size)
{
	t_token	*new_list;

	new_list = copy_token_list(start, current);
	if (new_list)
		*separated_lists = add_to_token_list_array(*separated_lists, new_list, size);
}

t_token	**token_separate_by_pipe(t_token *token_root)
{
	t_token	**separated_lists;
	t_token	*current;
	t_token	*start;
	int		size;

	separated_lists = NULL;
	current = token_root;
	start = token_root;
	size = 0;
	while (current)
	{
		if (current->type == PIPE)
		{
			process_tokens(start, current, &separated_lists, &size);
			start = current->next;
		}
		current = current->next;
	}
	if (start)
		process_tokens(start, NULL, &separated_lists, &size);
	return (finalize_token_array(separated_lists, size));
}

void	set_token_type(t_token *token)
{
	if (ft_strcmp(token->str, "|") == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ">") == 0)
		token->type = REDR;
	else if (ft_strcmp(token->str, "<") == 0)
		token->type = REDL;
	else if (ft_strcmp(token->str, ">>") == 0)
		token->type = REDRR;
	else if (ft_strcmp(token->str, "<<") == 0)
		token->type = REDLL;
	else
		token->type = ARG;
}

void	handle_redirection(t_token **token)
{
	t_token	*tmp;

	tmp = *token;
	if (tmp->type == REDR || tmp->type == REDL || tmp->type == REDRR)
	{
		if (tmp->next && tmp->next->str)
		{
			tmp = tmp->next;
			tmp->type = FILEE;
		}
	}
	else if (tmp->type == REDLL)
	{
		if (tmp->next && tmp->next->str)
		{
			tmp = tmp->next;
			tmp->type = HEREDOC;
		}
	}
	*token = tmp;
}

void	token_arr_set_type(t_token **token_arr)
{
	int		i;
	t_token	*token_root;

	i = 0;
	while (token_arr[i])
	{
		token_root = token_arr[i];
		token_root->type = CMD;
		token_root = token_root->next;
		while (token_root)
		{
			set_token_type(token_root);
			handle_redirection(&token_root);
			token_root = token_root->next;
		}
		i++;
	}
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

		if(ft_strlen(line) > 0)
			check_the_syntax(line);
		var_root = dup_veriables(env);

		root = str_to_token(line);

		token_extract_all_meta(&root);
		token_split_dollars(&root, var_root, &state);
		token_del_quote(root);
		state.token_arr = token_separate_by_pipe(root);
		token_arr_set_type(state.token_arr);

		int i = 0;
		printf("\n");
		while(state.token_arr[i])
		{
			token_list_printer(state.token_arr[i]);
			printf("\n");
			i++;
		}
	}
}
