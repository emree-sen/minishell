#include "mini.h"

void toggle_quote(int *quote, char c)
{
	if (*quote == -1 && (c == '\'' || c == '\"'))
		*quote = (int)c;
	else if (*quote == c)
		*quote = -1;
}

int is_has_meta(char *str)
{
	int i;
	int flag;

	flag = -1;
	i = -1;
	while (str[++i])
	{
		toggle_quote(&flag, str[i]);
		if (flag == -1 && (str[i] == '|' || str[i] == '>' || str[i] == '<'))
			return (1);
	}
	return (0);
}
int is_only_meta(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != '|' && str[i] != '>' && str[i] != '<')
			return (0);
	}
	return (1);
}

void token_new_add_prev(t_token **token_root, t_token *tmp, t_token *new, int *i)
{
	if(tmp->str[*i]== '|')
	{
		new = token_new(ft_substr(tmp->str, *i, 1), PIPE);
		token_add_prev(token_root, tmp, new);
		*i += 1;
	}
	else if(tmp->str[*i + 1] == '>')
	{
		new = token_new(ft_substr(tmp->str, *i, 2), REDRR);
		token_add_prev(token_root, tmp, new);
		*i += 2;
	}
	else
	{
		new = token_new(ft_substr(tmp->str, *i, 2), REDLL);
		token_add_prev(token_root, tmp, new);
		*i += 2;
	}
}

int token_extract_metas(t_token *tmp, t_token **token_root,t_token *new, int i)
{
	if(tmp->str[i] == '<')
	{
		if(tmp->str[i + 1] == '<')
			token_new_add_prev(token_root, tmp, new, &i);
		else
		{
			new = token_new(ft_substr(tmp->str, i++, 1), REDL);
			token_add_prev(token_root, tmp, new);
		}
	}
	else if(tmp->str[i] == '>')
	{

		if(tmp->str[i + 1] == '>')
			token_new_add_prev(token_root, tmp, new, &i);
		else
		{
			new = token_new(ft_substr(tmp->str, i++, 1), REDR);
			token_add_prev(token_root, tmp, new);
		}
	}
	else
		token_new_add_prev(token_root, tmp, new, &i);
	return (i);
}

int pass_str_pls(char *str, int i)
{
	int flag;

	flag = -1;
	while (str[i])
	{
		toggle_quote(&flag, str[i]);
		if (str[i] && (flag != -1 || (flag == -1 && (str[i] != '|' && str[i] != '>' && str[i] != '<'))))
			i++;
		else
			break;
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
			i = pass_str_pls(tmp->str, i);
			new = token_new(ft_substr(tmp->str, start, i - start), NONE);
			token_add_prev(token_root, tmp, new);
		}
	}
}

void token_meta_type_changer(t_token *tmp, int i)
{
	if(tmp->str[i] == '|')
		tmp->type = PIPE;
	else if(tmp->str[i] == '>')
	{
		if(tmp->str[i + 1] == '>')
			tmp->type = REDRR;
		else
			tmp->type = REDR;
	}
	else if(tmp->str[i] == '<')
	{
		if(tmp->str[i + 1] == '<')
			tmp->type = REDLL;
		else
			tmp->type = REDL;
	}
	else
		tmp->type = NONE;

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
		if((is_has_meta(tmp->str) && !is_only_meta(tmp->str)))
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
		{
			token_meta_type_changer(tmp, i);
			tmp = tmp->next;
		}
	}
}
