/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:30 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:07:15 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	exec_init(t_exec *exec)
{
	exec->path = NULL;
	exec->args = NULL;
	exec->input_file = NULL;
	exec->output_file = NULL;
	exec->in_fd = -1;
	exec->out_fd = -1;
	exec->output_type = -1;
	exec->heredocs = NULL;
	exec->heredoc_idx = 0;
	exec->err_str = NULL;
	exec->err_val = 0;
	exec->in_type = -1;
	exec->cmd_type = -1;
}

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	arg_num_finder(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp->type == ARG)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

int	is_has_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_getenv(char *key, t_variables *var_root)
{
	t_variables	*tmp;

	tmp = var_root;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
