/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:06 by emsen             #+#    #+#             */
/*   Updated: 2024/09/18 13:11:52 by emsen            ###   ########.fr       */
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

int	token_arr_len(t_token *token)
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
			if ((!have_not_alpha(token->str) || token->str[0] == '_')
				&& ft_strchr(token->str, '=') == NULL)
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
