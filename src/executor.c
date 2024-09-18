/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:36 by emsen             #+#    #+#             */
/*   Updated: 2024/09/18 18:27:32 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	handle_child_process(t_exec_params *params, int i)
{
	char	**env;

	if (params->exec[i]->err_val != 0)
	{
		close_all_fd(params->fds, params->state);
		exit_num(params->exec[i]->err_val);
	}
	if (params->state->arr_len == 1)
		single_command(params->exec, i);
	else
		execute_multi_command(params, i);
	if (params->exec[i]->path == NULL)
		exit(127);
	env = env_list_creator(params->var_root);
	execve(params->exec[i]->path, params->exec[i]->args, env);
	exit(127);
}

void	execute_multi_command(t_exec_params *params, int i)
{
	if (params->exec[i]->in_fd != -1 || params->exec[i]->out_fd != -1)
	{
		multi_command_with_redr(params->exec, i, params->fds, params->state);
	}
	else
	{
		multi_command_without_redr(params->fds, i, params->state);
	}
	if (params->exec[i]->cmd_type == BUILTIN)
	{
		single_command_built_in(params->exec, params->state, params->var_root,
			i);
		exit(0);
	}
}

void	wait_for_children(pid_t *pid, t_state *state)
{
	int	i;

	i = 0;
	while (i < state->arr_len)
	{
		if (i == state->arr_len - 1)
		{
			waitpid(pid[i], &state->status, 0);
			if (WIFEXITED(state->status))
				state->status = WEXITSTATUS(state->status);
		}
		waitpid(pid[i], NULL, 0);
		i++;
	}
}

void	free_resources(t_exec **exec, pid_t *pid, int **fds, t_state *state)
{
	int	i;

	i = 0;
	ft_free_exec(exec);
	free(pid);
	if (fds)
	{
		while (i < state->arr_len - 1)
		{
			free(fds[i]);
			i++;
		}
		free(fds);
	}
}

void	executor(t_state *state, t_variables *var_root)
{
	t_exec			**exec;
	int				**fds;
	pid_t			*pid;
	t_exec_params	params;

	state_arr_len_set(state);
	exec = exec_filler(state, var_root);
	execute_heredocs(exec);
	ft_print_exec_errors(exec, state);
	g_sig = IN_CMD;
	fds = prepare_fds(state);
	pid = malloc(sizeof(pid_t) * state->arr_len);
	params.exec = exec;
	params.state = state;
	params.var_root = var_root;
	params.fds = fds;
	execute_commands(&params, pid);
	close_all_fd(fds, state);
	wait_for_children(pid, state);
	g_sig = AFTER_CMD;
	free_resources(exec, pid, fds, state);
}
