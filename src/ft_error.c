/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:46 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 13:04:55 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	printf_spesific_error2(int err_type, char *str)
{
	if (err_type == ERR_NOT_VALID_IDFR)
	{
		write(2, "minishell: unset: `", 19);
		write(2, str, ft_strlen(str));
		write(2, "': not a valid identifier\n", 26);
	}
	else if (err_type == ERR_NUMERIC_ARG)
	{
		write(2, "minishell: exit: ", 17);
		write(2, str, ft_strlen(str));
		write(2, ": numeric argument required\n", 29);
	}
	else if (err_type == 1)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": No such file or directory\n", 29);
	}
	else if (err_type == 1271)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": command not found\n", 20);
	}
}

void	print_error(int err_type, char *str)
{
	write(2, "minishell: ", 11);
	write(2, str, ft_strlen(str));
	write(2, ": ", 2);
	write(2, strerror(err_type), ft_strlen(strerror(err_type)));
	write(2, "\n", 1);
}

void	printf_spesific_error(int err_type, char *str)
{
	if (err_type == ENOTDIR)
		print_error(err_type, str);
	else if (err_type == ENOENT)
		print_error(err_type, str);
	else if (err_type == EACCES)
		print_error(err_type, str);
	else
		printf_spesific_error2(err_type, str);
}

void	ft_print_exec_errors2(t_exec **exec, t_state *state, int i)
{
	if (exec[i]->err_val == 1271)
	{
		write(2, state->token_arr[i]->str, ft_strlen(state->token_arr[i]->str));
		write(2, ": command not found\n", 20);
	}
	else if (exec[i]->err_val == ERR_IS_A_DIRECTORY)
	{
		write(2, state->token_arr[i]->str, ft_strlen(state->token_arr[i]->str));
		write(2, ": Is a directory\n", 18);
	}
}

void	ft_print_exec_errors(t_exec **exec, t_state *state)
{
	int	i;

	i = 0;
	while (exec[i])
	{
		if (exec[i]->err_val != 0)
		{
			write(2, "minishell: ", 11);
			if (exec[i]->err_val == 1)
			{
				write(2, exec[i]->err_str, ft_strlen(exec[i]->err_str));
				write(2, "\n", 1);
			}
			else if (exec[i]->err_val == 1261)
			{
				write(2, "Permission denied\n", 19);
			}
			else
				ft_print_exec_errors2(exec, state, i);
		}
		i++;
	}
}
