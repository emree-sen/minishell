/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sep_by_pipe2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:34 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:10:35 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

t_token	**finalize_token_array(t_token **list_array, int size)
{
	t_token	**final_list;
	int		i;

	final_list = malloc(sizeof(t_token *) * (size + 1));
	if (!final_list)
		return (NULL);
	i = 0;
	while (i < size)
	{
		final_list[i] = list_array[i];
		i++;
	}
	final_list[size] = NULL;
	free(list_array);
	return (final_list);
}

void	process_tokens(t_token *start, t_token *current,
	t_token ***sep_list, int *size)
{
	t_token	*new_list;

	new_list = copy_token_list(start, current);
	if (new_list)
		*sep_list = add_to_token_list_array(*sep_list, new_list, size);
}

t_token	**token_separate_by_pipe(t_token *token_root)
{
	t_token	**separated_lists;
	t_token	*current;
	t_token	*start;
	int		size;

	separated_lists = NULL;
	current = token_root;
	start = token_root;
	size = 0;
	while (current)
	{
		if (current->type == PIPE)
		{
			process_tokens(start, current, &separated_lists, &size);
			start = current->next;
		}
		current = current->next;
	}
	if (start)
		process_tokens(start, NULL, &separated_lists, &size);
	return (finalize_token_array(separated_lists, size));
}
