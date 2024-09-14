/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:22 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:09:23 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	check_first_command(t_exec **exec, int **fds, int i)
{
	if (exec[i]->in_fd != -1 && exec[i]->out_fd != -1)
	{
		dup_exec_in_out(exec, i);
		close(fds[i][0]);
		close(fds[i][1]);
	}
	else if (exec[i]->in_fd != -1)
	{
		dup2(exec[i]->in_fd, 0);
		close(exec[i]->in_fd);
		dup2(fds[i][1], 1);
		close(fds[i][0]);
	}
	else if (exec[i]->out_fd != -1)
	{
		dup2(exec[i]->out_fd, 1);
		close(exec[i]->out_fd);
		close(fds[i][1]);
		close(fds[i][0]);
	}
	else
	{
		dup2(fds[i][1], 1);
		close(fds[i][0]);
	}
}
