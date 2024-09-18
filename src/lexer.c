/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:57 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 18:20:16 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	initialize_state(t_state *state, t_variables **var_root)
{
	extern char	**environ;

	*var_root = dup_veriables(environ);
	state->token_arr = NULL;
}

void	cleanup(t_state *state, t_token *root, char *line)
{
	if (state->token_arr)
		ft_free_token_arr(state->token_arr);
	if (root)
		ft_free_root(root);
	if (line)
		free(line);
}

int	just_spaces_check(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	lexer(char *line, t_token **root, t_variables *var_root, t_state *state)
{
	if (ft_strlen(line) != 0 && just_spaces_check(line) == 0)
	{
		if (check_the_syntax(line))
		{
			state->status = 258;
			return ;
		}
	}
	*root = str_to_token(line);
	token_extract_all_meta(root);
	token_split_dollars(root, var_root, state);
	token_del_quote(*root);
	state->token_arr = token_separate_by_pipe(*root, 0);
	token_arr_set_type(state->token_arr);
}

void	process_line(char *line, t_state *state, t_variables *var_root)
{
	t_token	*root;

	state->token_arr = NULL;
	lexer(line, &root, var_root, state);
	if (state->token_arr)
		executor(state, var_root);
	if (state->token_arr)
		cleanup(state, root, NULL);
}
