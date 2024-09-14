/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:17:38 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 15:29:23 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	ft_free_tokenn(t_token *token)
{
	if (token)
	{
		free(token->str);
		free(token);
	}
}

void	ft_free_token_arr(t_token **token_arr)
{
	int		i;
	t_token	*tmp;
	t_token	*tmp2;

	i = 0;
	while (token_arr && token_arr[i])
	{
		tmp = token_arr[i];
		while (tmp)
		{
			tmp2 = tmp;
			tmp = tmp->next;
			if (tmp2->type != PIPE)
				ft_free_tokenn(tmp2);
		}
		i++;
	}
	free(token_arr);
	token_arr = NULL;
}

void	ft_free_root(t_token *root)
{
	t_token	*tmp;

	while (root)
	{
		tmp = root;
		root = root->next;
		free(tmp);
	}
}

void	ft_free_var_root(t_variables *var_root)
{
	t_variables	*tmp;

	while (var_root)
	{
		tmp = var_root;
		var_root = var_root->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
