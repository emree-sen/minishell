/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_dollar2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:17 by emsen             #+#    #+#             */
/*   Updated: 2024/09/19 18:35:02 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	dollar_value_checker(t_state *state, t_dollar *dollar)
{
	if (ft_strcmp(dollar->key, "?") == 0)
		dollar->value = ft_itoa(state->status);
	else if (ft_strcmp(dollar->key, "0") == 0)
		dollar->value = ft_strdup("minishell");
	else
		dollar->value = ft_strdup(" ");
}

void	token_replace_value(t_token **str, t_dollar *dollar, int *i,
		t_state *state)
{
	int		end;
	char	*left;
	char	*right;
	char	*tmp;

	end = *i + ft_strlen(dollar->key);
	left = ft_substr((*str)->str, 0, *i);
	right = ft_substr((*str)->str, end + 1, ft_strlen((*str)->str) - end);
	if (!dollar->value)
	{
		dollar_value_checker(state, dollar);
		*i = *i - 1;
	}
	free((*str)->str);
	(*str)->str = ft_strjoin(left, dollar->value);
	tmp = (*str)->str;
	free((*str)->str);
	(*str)->str = ft_strjoin(tmp, right);
	free(left);
	free(right);
}

void	is_null(char *str)
{
	if (!str)
	{
		str = ft_strdup("");
	}
}

int	pass_any(char *str, int *ai, char any)
{
	while (str[*ai] && str[*ai] == any)
		(*ai)++;
	return (*ai);
}
