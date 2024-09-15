/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:57 by emsen             #+#    #+#             */
/*   Updated: 2024/09/15 18:52:25 by emsen            ###   ########.fr       */
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

void	ft_print_token_arr(t_token **token)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (token[i])
	{
		tmp = token[i];
		while (tmp)
		{
			printf("str: %s, type: %d\n", tmp->str, tmp->type);
			printf("tmp: %p\n", tmp);
			tmp = tmp->next;
		}
		i++;
	}
}

void token_meta_type_changer_2(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp->str[i])
	{
		if (tmp->str[i] == '|')
			tmp->type = PIPE;
		else if (tmp->str[i] == '>')
		{
			if (tmp->str[i + 1] == '>')
				tmp->type = REDRR;
			else
				tmp->type = REDR;
		}
		else if (tmp->str[i] == '<')
		{
			if (tmp->str[i + 1] == '<')
				tmp->type = REDLL;
			else
				tmp->type = REDL;
		}
		else
			tmp->type = NONE;
		i++;
	}
}

void meta_type_changer(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		token_meta_type_changer_2(tmp);
		tmp = tmp->next;
	}
	
}

void	lexer(char *line, t_token **root, t_variables *var_root, t_state *state)
{
	if (check_the_syntax(line))
	{
		state->status = 258;
		return ;
	}
	*root = str_to_token(line);
	token_extract_all_meta(root);


	token_split_dollars(root, var_root, state);

	//meta_type_changer(root);
	token_del_quote(*root);
	state->token_arr = token_separate_by_pipe(*root);
	token_arr_set_type(state->token_arr);
	//ft_print_token_arr(root);
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
