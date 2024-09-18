/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:20 by emsen             #+#    #+#             */
/*   Updated: 2024/09/18 12:04:03 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	all_token_value_checker(t_variables *var_tmp, t_dollar *dollar,
		char **value)
{
	while (var_tmp)
	{
		token_value_checker(var_tmp, dollar->key, value);
		var_tmp = var_tmp->next;
	}
}

char	*token_value_finder(t_token *tmp, t_dollar *dollar,
		t_variables *var_root)
{
	int		start;
	int		end;
	char	*value;

	value = NULL;
	start = dollar->i;
	end = dollar->i;
	end++;
	if (ft_isdigit(tmp->str[end]) == 1)
		end++;
	else
	{
		while (tmp->str[end] && (ft_isalnum(tmp->str[end])
				|| tmp->str[end] == '_'))
			end++;
	}
	if (tmp->str[end] != '?')
		dollar->key = ft_substr(tmp->str, start + 1, end - start - 1);
	else
		dollar->key = ft_strdup("?");
	all_token_value_checker(var_root, dollar, &value);
	return (value);
}

void	free_dollar(t_dollar *d)
{
	if (d->value)
		free(d->value);
	if (d->key)
		free(d->key);
}

void	token_dollar_value_finder(t_dollar *d, t_token *tmp,
		t_variables *var_root, t_state *state)
{
	if (d->flag == -1 && tmp->str[d->i] == '$' && tmp->str[d->i + 1] != '\"'
		&& tmp->str[d->i + 1] != '\'' && tmp->str[d->i + 1] != '\0'
		&& tmp->str[d->i + 1] != '$' && tmp->str[d->i + 1] != ' ')
	{
		d->value = token_value_finder(tmp, d, var_root);
		token_replace_value(&tmp, d, &d->i, state);
	}
}

void	token_split_dollars(t_token **token_root, t_variables *var_root,
		t_state *state)
{
	t_token		*tmp;
	t_dollar	d;

	tmp = *token_root;
	while (tmp)
	{
		d.i = -1;
		while (tmp->str[++d.i])
		{
			d.key = NULL;
			d.value = NULL;
			d.flag = -1;
			d.flag2 = -1;
			toggle_single_quote(&d.flag, tmp->str[d.i], &d.flag2);
			token_dollar_value_finder(&d, tmp, var_root, state);
			free_dollar(&d);
		}
		tmp = tmp->next;
	}
	token_extract_spaces(token_root);
}
