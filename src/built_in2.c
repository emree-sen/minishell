/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:06 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 17:30:39 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_alporund(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && str[i] != '=')
		{
			if (!ft_isalnum(str[i]) && str[i] != '_')
				return (1);
			i++;
		}
		return (0);
	}
	return (1);
}

int token_arr_len(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		i++;
		token = token->next;
	}
	return (i);
}

void new_export_variable(t_variables *var_root, char *str)
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

void	variable_deler(t_variables **var_root, char *key)
{
	t_variables	*tmp;

	tmp = *var_root;
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

void	ft_unset(t_variables *var_root, t_state *state, int i)
{
	t_token		*token;

	token = state->token_arr[i];
	while (token)
	{
		if (token->type == ARG)
		{
			if (ft_isalpha(token->str[0]) || token->str[0] == '_'
				|| ft_strchr(token->str, '=') == NULL)
				variable_deler(&var_root, token->str);
			else
			{
				printf("minishell: unset: `%s': not a valid identifier\n",
					token->str);
				state->status = ERR_NOT_VALID_IDFR;
			}
		}
		token = token->next;
	}
}

void	ft_env(t_variables *var_root)
{
	t_variables	*tmp;

	tmp = var_root;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
}
