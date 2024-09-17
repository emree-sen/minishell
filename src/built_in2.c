/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:06 by emsen             #+#    #+#             */
/*   Updated: 2024/09/16 18:49:23 by emsen            ###   ########.fr       */
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

void	ft_export(t_state *state, t_variables *var_root, int i)
{
	t_token	*tmp;

	tmp = state->token_arr[i];
	while (tmp)
	{
		if (tmp->type == ARG)
		{
			if (!ft_isdigit(tmp->str[0]) && is_alporund(tmp->str) == 0)
			{
				if (ft_strchr(tmp->str, '='))
					new_variable_adder(var_root, ft_substr(tmp->str, 0,
							ft_strchr(tmp->str, '=') - tmp->str),
						ft_substr(tmp->str, ft_strchr(tmp->str, '=') \
							- tmp->str + 1, ft_strlen(tmp->str)));
			}
			else
			{
				write(2, "minishell: export: `", 20);
				write(2, tmp->str, ft_strlen(tmp->str));
				write(2, "': not a valid identifier\n", 26);
				state->status = 1;
			}
		}
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
