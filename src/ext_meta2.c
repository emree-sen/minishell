#include "mini.h"

int	real_quote_count(char *str)
{
	int	i;
	int	count;
	int	flag;

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

int	quote_count(char *str)
{
	int	i;
	int	count;
	int	flag;

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

void	token_extract_creator(t_token *tmp,
	t_token **token_root, t_token *new, int i)
{
	int	start;

	while (tmp->str[i])
	{
		if (tmp->str[i] == '|' || tmp->str[i] == '>' || tmp->str[i] == '<')
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

void	token_meta_type_changer(t_token *tmp, int i)
{
	if (tmp->str[i] == '|')
		tmp->type = PIPE;
	else if (tmp->str[i] == '>')
	{
		if (tmp->str[i + 1] == '>')
			tmp->type = REDRR;
		else
			tmp->type = REDR;
	}
	else if (tmp->str[i] == '<')
	{
		if (tmp->str[i + 1] == '<')
			tmp->type = REDLL;
		else
			tmp->type = REDL;
	}
	else
		tmp->type = NONE;
}

void	token_extract_all_meta(t_token **token_root)
{
	t_token	*tmp;
	t_token	*new;
	int		i;

	i = 0;
	tmp = *token_root;
	new = NULL;
	while (tmp)
	{
		if ((is_has_meta(tmp->str) && !is_only_meta(tmp->str)))
		{
			token_extract_creator(tmp, token_root, new, i);
			i = 0;
			if (tmp->prev)
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
