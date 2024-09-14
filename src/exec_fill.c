/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:25 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:09:26 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_built_in(t_token *tmp)
{
	if (ft_strcmp(tmp->str, "echo") == 0)
		return (1);
	if (ft_strcmp(tmp->str, "cd") == 0)
		return (1);
	else if (ft_strcmp(tmp->str, "pwd") == 0)
		return (1);
	else if (ft_strcmp(tmp->str, "export") == 0)
		return (1);
	else if (ft_strcmp(tmp->str, "unset") == 0)
		return (1);
	else if (ft_strcmp(tmp->str, "env") == 0)
		return (1);
	else if (ft_strcmp(tmp->str, "exit") == 0)
		return (1);
	return (0);
}

void	fill_exec(t_exec *exec, t_token *tmp, t_variables *var_root)
{
	while (tmp)
	{
		if (tmp->type == CMD)
		{
			if (is_built_in(tmp) == 1)
			{
				exec->cmd_type = BUILTIN;
				exec->path = ft_strdup(tmp->str);
				exec->args = args_filler(tmp, exec->path);
			}
			if (exec->cmd_type != BUILTIN)
			{
				path_finder(tmp->str, var_root, exec);
				if (exec->path != NULL)
					exec->args = args_filler(tmp, exec->path);
			}
		}
		init_redirection(tmp, exec);
		tmp = tmp->next;
	}
}

t_exec	**exec_filler(t_state *state, t_variables *var_root)
{
	int		i;
	t_exec	**exec;

	exec = exec_create(state);
	i = 0;
	while (i < state->arr_len)
	{
		fill_exec(exec[i], state->token_arr[i], var_root);
		i++;
	}
	return (exec);
}

t_exec	**exec_create(t_state *state)
{
	int		i;
	t_exec	**exec;

	exec = malloc(sizeof(t_exec *) * (state->arr_len + 1));
	if (!exec)
		exit(1);
	exec[state->arr_len] = NULL;
	i = 0;
	while (i < state->arr_len)
	{
		exec[i] = malloc(sizeof(t_exec));
		if (!exec[i])
			exit(1);
		exec_init(exec[i]);
		i++;
	}
	return (exec);
}

void	ft_set_error(t_exec *exec, int err)
{
	exec->err_val = err;
	exec->err_str = ft_strdup(strerror(err));
}
