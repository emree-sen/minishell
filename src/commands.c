/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:13 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:09:14 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	check_middle_command(t_exec **exec, int **fds, int i)
{
	if (exec[i]->in_fd != -1 && exec[i]->out_fd != -1)
	{
		dup_exec_in_out(exec, i);
		close(fds[i - 1][1]);
		close(fds[i - 1][0]);
	}
	else if (exec[i]->in_fd != -1)
	{
		dup2(exec[i]->in_fd, 0);
		close(exec[i]->in_fd);
		close(fds[i - 1][1]);
		close(fds[i - 1][0]);
	}
	else if (exec[i]->out_fd != -1)
	{
		dup2(exec[i]->out_fd, 1);
		close(exec[i]->out_fd);
		dup2(fds[i - 1][0], 0);
		close(fds[i - 1][1]);
	}
	else
	{
		dup2(fds[i - 1][0], 0);
		close(fds[i - 1][1]);
	}
}

void	check_last_command_in(t_exec **exec, int **fds, int i)
{
	dup2(exec[i]->in_fd, 0);
	close(exec[i]->in_fd);
	close(fds[i - 1][1]);
	close(fds[i - 1][0]);
	dup2(fds[i][1], 1);
	close(fds[i][0]);
}

void	check_last_command_out(t_exec **exec, int **fds, int i)
{
	dup2(exec[i]->out_fd, 1);
	close(exec[i]->out_fd);
	dup2(fds[i - 1][0], 0);
	close(fds[i - 1][1]);
	close(fds[i][0]);
	close(fds[i][1]);
}

void	check_last_command(t_exec **exec, int **fds, int i)
{
	if (exec[i]->in_fd != -1 && exec[i]->out_fd != -1)
	{
		dup_exec_in_out(exec, i);
		close(fds[i - 1][1]);
		close(fds[i - 1][0]);
		close(fds[i][0]);
		close(fds[i][1]);
	}
	else if (exec[i]->in_fd != -1)
		check_last_command_in(exec, fds, i);
	else if (exec[i]->out_fd != -1)
		check_last_command_out(exec, fds, i);
	else
	{
		dup2(fds[i - 1][0], 0);
		dup2(fds[i][1], 1);
		close(fds[i - 1][1]);
		close(fds[i][0]);
	}
}

void	fd_setter_with_redr(t_exec **exec, int **fds, int i, t_state *state)
{
	if (i == 0)
		check_first_command(exec, fds, i);
	else if (i == state->arr_len - 1)
		check_middle_command(exec, fds, i);
	else
		check_last_command(exec, fds, i);
	fd_closer(fds, i, state);
}
