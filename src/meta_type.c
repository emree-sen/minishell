/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:17:00 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:17:05 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	token_meta_type_changer_2(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp->str[i])
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
		i++;
	}
}

void	meta_type_changer(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		token_meta_type_changer_2(tmp);
		tmp = tmp->next;
	}
}
