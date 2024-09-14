/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:52 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:22:51 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	*heredoc_create(t_state *state)
{
	int	i;
	int	*heredoc_fds;

	heredoc_fds = malloc(sizeof(int) * state->arr_len);
	if (!heredoc_fds)
		exit(1);
	i = 0;
	while (i < state->arr_len)
	{
		heredoc_fds[i] = -37;
		i++;
	}
	return (heredoc_fds);
}

void	heredoc_writer(t_exec *exec, int *pipe_fd, char *input)
{
	if (!ft_strcmp(input, exec->heredocs[exec->heredoc_idx]))
	{
		if (exec->heredocs[exec->heredoc_idx + 1])
			exec->heredoc_idx++;
		else
			return ;
	}
	else
	{
		if (exec->heredocs[exec->heredoc_idx + 1])
			return ;
		else
		{
			write(pipe_fd[1], input, ft_strlen(input));
			write(pipe_fd[1], "\n", 1);
		}
	}
}

void	heredoc_setter(t_exec *exec)
{
	int		pipe_fd[2];
	char	*input;

	input = NULL;
	if (pipe(pipe_fd) == -1)
		exit(1);
	exec->heredoc_idx = 0;
	while (exec->heredocs != NULL && exec->heredocs[exec->heredoc_idx])
	{
		if (input)
			free(input);
		input = readline("> ");
		if (!input)
			break ;
		heredoc_writer(exec, pipe_fd, input);
	}
	if (exec->heredocs != NULL && exec->in_type == REDLL)
		exec->in_fd = pipe_fd[0];
	else
		close(pipe_fd[0]);
	close(pipe_fd[1]);
	free(input);
}
