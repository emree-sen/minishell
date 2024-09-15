/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:23 by emsen             #+#    #+#             */
/*   Updated: 2024/09/15 18:13:19 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	pass_quote(char *str, int qi)
{
	char	q;

	q = str[qi];
	qi++;
	while (str[qi] && str[qi] != q)
		qi++;
	return (qi);
}

int	pass_str(char *str, int si)
{
	while (str[si] && str[si] != '|' && str[si] != '>' && str[si] != '<')
		si++;
	return (si);
}

void	pass_quote_or_any(char *str, int *i)
{
	while (str[*i])
	{
		if (str[*i] == '\'' || str[*i] == '\"')
			*i = pass_quote(str, *i);
		if (str[*i] && str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
}

t_token	*str_to_token(char *str)
{
	t_token	*root;
	t_token	*token;
	int		i;
	int		start;
	char	*tmp;
	
	i = 0;
	root = NULL;
	token = NULL;
	is_null(str);
	str = ft_strtrim(str, " ");
	while (str[i])
	{
		start = pass_any(str, &i, ' ');
		pass_quote_or_any(str, &i);
		tmp = ft_substr(str, start, i - start);
		token = token_new(tmp, NONE);
		token_add_last(&root, token);
	}
	return (free(str), root);
}

int	pass_str_pls(char *str, int i)
{
	int	flag;

	flag = -1;
	while (str[i])
	{
		toggle_quote(&flag, str[i]);
		if (str[i] && (flag != -1 || (flag == -1 && (str[i] != '|'
						&& str[i] != '>' && str[i] != '<'))))
			i++;
		else
			break ;
	}
	return (i);
}
