#include "mini.h"

int pass_quote(char *str, int qi)
{
	char q;
	q = str[qi];
	qi++;
	while (str[qi] && str[qi] != q)
		qi++;
	return (qi);
}

int pass_str(char *str, int si)
{
	while (str[si] && str[si] != '|' && str[si] != '>' && str[si] != '<')
		si++;
	return (si);
}

int is_has_meta(char *str)
{
	int i;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|' || str[i] == '>' || str[i] == '<')
			return (1);
	}
	return (0);
}


t_token *str_to_token(char *str)
{
	t_token *root;
	t_token *token;
	int i;
	int start;

	i = -1;
	root = NULL;
	while (str[++i])
	{
		while(str[i] == ' ')
			i++;
		start = i;
		while(str[i])
		{
			if(str[i] == '\'' || str[i] == '\"')
				i = pass_quote(str,i);
			if(str[i] && str[i] != ' ')
				i++;
			else
				break;
		}
		token = token_new(ft_substr(str, start, i - start), NONE);
		token_add_last(&root, token);
	}
	return (root);
}

int token_extract_metas(t_token *tmp, t_token **token_root,t_token *new,int i)
{
	if (tmp->str[i + 1] == '>')
	{
		new = token_new(ft_substr(tmp->str, i, 2), REDRR);
		token_add_prev(token_root, tmp, new);
		i += 2;
	}
	else if (tmp->str[i + 1] == '<')
	{
		new = token_new(ft_substr(tmp->str, i, 2), REDLL);
		token_add_prev(token_root, tmp, new);
		i += 2;
	}
	else
	{
		new = token_new(ft_substr(tmp->str, i++, 1), PIPE);
		token_add_prev(token_root, tmp, new);
	}
	return (i);
}

void token_extract_creator(t_token *tmp, t_token **token_root,t_token *new,int i)
{
	int start;

	while(tmp->str[i])
	{
		if(tmp->str[i] == '|' || tmp->str[i] == '>' || tmp->str[i] == '<')
		{
			i = token_extract_metas(tmp, token_root, new, i);
		}
		else
		{
			start = i;
			i = pass_str(tmp->str, i);
			new = token_new(ft_substr(tmp->str, start, i - start), NONE);
			token_add_prev(token_root, tmp, new);
		}
	}
}

void token_extract_all_meta(t_token **token_root)
{
	t_token *tmp;
	t_token *new;
	int i;

	i = 0;
	tmp = *token_root;
	new = NULL;
	while (tmp)
	{
		if(is_has_meta(tmp->str) == 1)
		{
			token_extract_creator(tmp, token_root, new, i);
			i = 0;
			if(tmp->prev)
			{
				tmp = tmp->prev;
				token_del(tmp->next);
			}
		}
		else
			tmp = tmp->next;
	}
}

int main()
{
	// test str_to_token func in readline
	char *line;
	t_token *root;
	line = readline("minishell$ ");
	root = str_to_token(line);
	/*
	t_token *new;
	new = token_new("new", NONE);
	token_add_prev(&root, root, new);
	*/

	token_extract_all_meta(&root);
	token_list_printer(root);
	
}