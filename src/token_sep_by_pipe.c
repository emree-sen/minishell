/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sep_by_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:32 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 16:41:19 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	ft_free_token(t_token *token)
{
	if (token)
	{
		free(token->str);
		free(token);
	}
}

t_token	*copy_token_list(t_token *start, t_token *end)
{
	t_token	*new_list;
	t_token	*current;
	t_token	*last;
	t_token	*new_token;

	current = start;
	new_list = NULL;
	last = NULL;
	while (current != end)
	{
		new_token = token_new(current->str, current->type);
		if (!new_token)
			return (NULL);
		if (!new_list)
			new_list = new_token;
		else
		{
			last->next = new_token;
			new_token->prev = last;
		}
		last = new_token;
		current = current->next;
	}
	return (new_list);
}

void	ft_free_token_array(t_token **token_arr, int size)
{
	int		i;
	t_token	*tmp;
	t_token	*tmp2;

	i = 0;
	while (token_arr && i < size - 1)
	{
		tmp = token_arr[i];
		while (tmp)
		{
			tmp2 = tmp;
			tmp = tmp->next;
			if (strcmp(tmp2->str, "|") != 0)
				ft_free_token(tmp2);
		}
		i++;
	}
	if (token_arr)
		free(token_arr);
}

t_token	**add_to_token_list_array(t_token **token_list_array,
	t_token *new_list, int *size)
{
	t_token	**new_array;
	int		i;

	new_array = malloc(sizeof(t_token *) * (*size + 1));
	i = 0;
	if (!new_array)
		return (NULL);
	while (i < *size)
	{
		new_array[i] = token_list_array[i];
		i++;
	}
	new_array[*size] = new_list;
	(*size)++;
	free(token_list_array);
	return (new_array);
}

t_token	**finalize_token_list_array(t_token **list_array, int size)
{
	int		i;
	t_token	**final_list;

	i = 0;
	final_list = malloc(sizeof(t_token *) * (size + 1));
	if (!final_list)
		return (NULL);
	while (i < size)
	{
		final_list[i] = list_array[i];
		i++;
	}
	final_list[size] = NULL;
	free(list_array);
	return (final_list);
}
