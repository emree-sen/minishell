/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:17:25 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:19:26 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	main(void)
{
	t_state		state;
	t_variables	*var_root;
	char		*line;

	initialize_state(&state, &var_root);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		add_history(line);
		process_line(line, &state, var_root);
		free(line);
	}
	ft_free_var_root(var_root);
	return (0);
}
