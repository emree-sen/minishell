/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:44 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:27:47 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	**fds_filler(int **fds, t_state *state)
{
	int	i;

	i = 0;
	if (state->arr_len <= 1)
		return (NULL);
	fds = malloc(sizeof(int *) * (state->arr_len - 1));
	if (!fds)
		exit(1);
	while (i < state->arr_len - 1)
	{
		fds[i] = malloc(sizeof(int) * 2);
		if (!fds[i])
			exit(1);
		if (pipe(fds[i]) == -1)
		{
			exit(1);
		}
		i++;
	}
	return (fds);
}

void	fd_closer(int **fds, int i, t_state *state)
{
	int	j;

	j = 0;
	while (j < state->arr_len - 1)
	{
		if (j != i - 1 && j != i)
		{
			close(fds[j][0]);
			close(fds[j][1]);
		}
		j++;
	}
}

void	close_all_fd(int **fds, t_state *state)
{
	int	i;

	i = 0;
	while (i < state->arr_len - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

int	**prepare_fds(t_state *state)
{
	if (state->arr_len > 1)
		return (fds_filler(NULL, state));
	return (NULL);
}
