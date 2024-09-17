/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:11:39 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:29:14 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	key_searcher(t_variables *var_root, char *key)
{
	t_variables	*tmp;

	tmp = var_root;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			variables_del(tmp);
			break ;
		}
		tmp = tmp->next;
	}
}

void	new_variable_adder(t_variables *var_root, char *key, char *value)
{
	t_variables	*new;

	key_searcher(var_root, key);
	new = variables_new(key, value);
	free(key);
	free(value);
	variables_add_last(&var_root, new);
}

void	new_export_variable(t_variables *var_root, char *str)
{
	if (!ft_isdigit(str[0]) && is_alporund(str) == 0)
	{
		if (ft_strchr(str, '='))
			new_variable_adder(var_root, ft_substr(str, 0,
					ft_strchr(str, '=') - str),
				ft_substr(str, ft_strchr(str, '=') \
				- str + 1, ft_strlen(str)));
	}
	else
	{
		write(2, "minishell: export: `", 20);
		write(2, str, ft_strlen(str));
		write(2, "': not a valid identifier\n", 26);
	}
}

void	ft_export(t_state *state, t_variables *var_root, int i)
{
	t_token	*tmp;

	if (token_arr_len(state->token_arr[i]) == 1)
	{
		ft_env(var_root);
		return ;
	}
	tmp = state->token_arr[i];
	while (tmp)
	{
		if (tmp->type == ARG)
			new_export_variable(var_root, tmp->str);
		tmp = tmp->next;
	}
}
