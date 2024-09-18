/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:24:16 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:27:36 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	state_arr_len_set(t_state *state)
{
	int	i;

	i = 0;
	while (state->token_arr[i])
		i++;
	state->arr_len = i;
}

void	execute_heredocs(t_exec **exec)
{
	int	i;

	i = 0;
	while (exec[i])
	{
		heredoc_setter(exec[i]);
		i++;
	}
}

void	execute_single_builtin(t_exec **exec, t_state *state,
		t_variables *var_root, int i)
{
	int	tmp_fd;

	if (exec[i]->err_val != 0)
	{
		state->status = 1;
		return ;
	}
	tmp_fd = dup(1);
	if (exec[i]->out_fd != -1)
		dup2(exec[i]->out_fd, 1);
	single_command_built_in(exec, state, var_root, i);
	if (exec[i]->out_fd != -1)
		dup2(tmp_fd, 1);
}

void	execute_commands(t_exec_params *params, pid_t *pid)
{
	int	i;

	i = 0;
	while (i < params->state->arr_len)
	{
		if (params->state->arr_len == 1 && params->exec[i]->cmd_type == BUILTIN)
		{
			execute_single_builtin(params->exec, params->state,
				params->var_root, i);
			break ;
		}
		pid[i] = fork_process(params, i);
		i++;
	}
}

pid_t	fork_process(t_exec_params *params, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit(1);
	else if (pid == 0)
	{
		handle_child_process(params, i);
	}
	return (pid);
}
