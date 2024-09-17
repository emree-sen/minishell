/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_dollar3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:11:49 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:12:56 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	token_value_checker(t_variables *var_tmp, char *key, char **value)
{
	if (!ft_strcmp(var_tmp->key, key))
	{
		*value = ft_strdup(var_tmp->value);
		return ;
	}
}

t_variables	*dup_veriables(char **environ)
{
	t_variables	*root;
	t_variables	*variables;
	int			i;
	char		*key;
	char		*value;

	i = -1;
	root = NULL;
	while (environ[++i])
	{
		key = ft_substr(environ[i], 0, ft_strchr(environ[i], '=') - environ[i]);
		value = ft_strdup(ft_strchr(environ[i], '=') + 1);
		variables = variables_new(key, value);
		variables_add_last(&root, variables);
		free(key);
		free(value);
	}
	return (root);
}
