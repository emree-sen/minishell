/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:36 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 13:08:36 by emsen            ###   ########.fr       */
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

void	printer(t_exec *exec)
{
	printf("input_file: %s\n", exec->input_file);
	printf("in_fd: %d\n", exec->in_fd);
	printf("output_file: %s\n", exec->output_file);
	printf("out_fd: %d\n", exec->out_fd);
}

void	exec_print(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	while (exec[i])
	{
		j = 0;
		if (exec[i]->path != NULL)
			printf("path: %s\n", exec[i]->path);
		while (exec[i]->args != NULL && exec[i]->args[j])
		{
			if (exec[i]->args != NULL)
				printf("args: %s\n", exec[i]->args[j]);
			j++;
		}
		j = 0;
		while (exec[i]->heredocs != NULL && exec[i]->heredocs[j])
		{
			if (exec[i]->heredocs != NULL)
				printf("heredocs: %s\n", exec[i]->heredocs[j]);
			j++;
		}
		printer(exec[i]);
		i++;
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
	ft_print_exec_errors(exec, state);
	execute_heredocs(exec);
	fds = prepare_fds(state);
	pid = malloc(sizeof(pid_t) * state->arr_len);
	params.exec = exec;
	params.state = state;
	params.var_root = var_root;
	params.fds = fds;
	execute_commands(&params, pid);
	close_all_fd(fds, state);
	wait_for_children(pid, state);
	free_resources(exec, pid);
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

int	**prepare_fds(t_state *state)
{
	if (state->arr_len > 1)
		return (fds_filler(NULL, state));
	return (NULL);
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

void	free_resources(t_exec **exec, pid_t *pid)
{
	ft_free_exec(exec);
	free(pid);
}
