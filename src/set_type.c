/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:14 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:14:25 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	set_token_type(t_token *token, int *flag)
{
	if (*flag == 0 && token->type == NONE)
	{
		token->type = CMD;
		*flag = 1;
	}
	else if (*flag == 1 && token->type == NONE)
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
	int		flag;

	flag = 0;
	i = 0;
	while (token_arr[i])
	{
		token_root = token_arr[i];
		while (token_root)
		{
			set_token_type(token_root, &flag);
			handle_redirection(&token_root);
			token_root = token_root->next;
		}
		flag = 0;
		i++;
	}
}

int	pass_the_quotes(char c, int quote)
{
	if ((c == '\'' || c == '\"') && quote == 0)
		quote = c;
	else if ((quote == c) && quote != 0)
		quote = 0;
	return (quote);
}

int	pass_the_spaces(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	return (i);
}
