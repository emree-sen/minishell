/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_space_cont.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:08:57 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:08:58 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_has_space(char *str)
{
	int	i;
	int	flag;

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

void	toggle_single_quote(int *quote, char c, int *fl)
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

void	token_extract_spaces(t_token **token_root)
{
	t_token	*tmp;
	t_token	*new;

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

void	token_extract_sp_creator(t_token *tmp,
		t_token **token_root, t_token *new, int i)
{
	int	start;
	int	flag;

	flag = -1;
	while (tmp->str[i])
	{
		toggle_quote(&flag, tmp->str[i]);
		if (flag == -1 && tmp->str[i] == ' ')
			i++;
		else
		{
			start = i;
			if (tmp->str[i] == '\"' || tmp->str[i] == '\'')
				i = pass_quote(tmp->str, i);
			while (tmp->str[i] && tmp->str[i] != ' ')
				i++;
			new = token_new(ft_substr(tmp->str, start, i - start), NONE);
			token_add_prev(token_root, tmp, new);
		}
	}
}
