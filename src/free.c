/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:17:38 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 12:18:08 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	ft_free_token_arr(t_token **token_arr)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (token_arr[i])
	{
		tmp = token_arr[i];
		free(tmp->str);
		free(tmp);
		i++;
	}
	free(token_arr);
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
