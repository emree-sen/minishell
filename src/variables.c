/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:43 by emsen             #+#    #+#             */
/*   Updated: 2024/09/19 19:06:44 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_variables	*variables_new(char *key, char *value)
{
	t_variables	*variables;
	char		*line;

	variables = malloc(sizeof(t_variables));
	if (!variables)
		exit(1);
	variables->key = ft_strdup(key);
	variables->value = ft_strdup(value);
	variables->prev = NULL;
	variables->next = NULL;
	line = ft_strjoin(key, "=");
	variables->line = ft_strjoin(line, value);
	free(line);
	return (variables);
}

void	variables_add_last(t_variables **root, t_variables *variables)
{
	t_variables	*tmp;

	if (!*root)
	{
		*root = variables;
		return ;
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = variables;
	variables->prev = tmp;
	variables->next = NULL;
}

void	variables_list_printer(t_variables *root)
{
	t_variables	*tmp;

	tmp = root;
	while (tmp)
	{
		printf("key: %s\n", tmp->key);
		printf("value: %s\n", tmp->value);
		tmp = tmp->next;
	}
}

void	variables_del(t_variables *var)
{
	if (var->prev)
		var->prev->next = var->next;
	else
		var->prev = NULL;
	if (var->next)
		var->next->prev = var->prev;
	else
		var->next = NULL;
	free(var->line);
	free(var->key);
	free(var->value);
	free(var);
}
