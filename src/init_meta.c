/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_meta.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:55 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:09:56 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	init_heredoc(t_token *tmp, t_exec *exec)
{
	int	heredoc_num;

	if (exec->heredocs == NULL)
	{
		heredoc_num = count_heredocs(tmp);
		exec->heredocs = malloc(sizeof(char *) * (heredoc_num + 1));
		exec->heredocs[heredoc_num] = NULL;
	}
	exec->heredocs[exec->heredoc_idx] = ft_strdup(tmp->str);
	exec->heredoc_idx++;
}

void	init_redr(t_token *tmp, t_exec *exec)
{
	exec->output_file = ft_strdup(tmp->next->str);
	exec->output_type = REDR;
	exec->out_fd = open(exec->output_file,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (exec->out_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redrr(t_token *tmp, t_exec *exec)
{
	exec->output_file = ft_strdup(tmp->next->str);
	exec->output_type = REDRR;
	exec->out_fd = open(exec->output_file,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (exec->out_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redll(t_token *tmp, t_exec *exec)
{
	exec->in_type = REDLL;
	exec->input_file = ft_strdup(tmp->next->str);
	exec->in_fd = open(exec->input_file, O_RDONLY);
	if (exec->in_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redirection(t_token *tmp, t_exec *exec)
{
	if (tmp->type == HEREDOC)
		init_heredoc(tmp, exec);
	else if (tmp->type == REDR && exec->err_val == 0)
		init_redr(tmp, exec);
	else if (tmp->type == REDRR && exec->err_val == 0)
		init_redrr(tmp, exec);
	else if (tmp->type == REDL && exec->err_val == 0)
		init_redll(tmp, exec);
	else if (tmp->type == REDLL && exec->err_val == 0)
		exec->in_type = REDLL;
}
