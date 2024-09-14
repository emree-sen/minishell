/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:26 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 10:10:27 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	last_arg_is_redir(char *input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	i--;
	while (input[i] == ' ')
		i--;
	if (input[i] == '>' || input[i] == '<')
		return (1);
	return (0);
}

int	all_closed_quotes(const char *input)
{
	int	i;
	int	quote;

	quote = 0;
	i = -1;
	while (input[++i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && quote == 0)
			quote = input[i];
		else if ((quote == input[i]) && (quote != 0))
			quote = 0;
	}
	return (quote);
}

int	backslash_check(char *input)
{
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '\\' && quote == 0)
			return (1);
		i++;
	}
	return (0);
}

int	semicolon_check(char *input)
{
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == ';' && quote == 0)
			return (1);
		i++;
	}
	return (0);
}

int	check_the_syntax(char *input)
{
	if (all_closed_quotes(input) != 0)
		return (print_syntax_error_quote());
	if (is_pipe_first(input) != 0 || \
		is_pipe_last(input) != 0 || double_pipe(input) != 0)
		return (print_syntax_error_pipe());
	if (backslash_check(input) == 1 || semicolon_check(input) == 1)
		return (print_unexpected_char_error());
	if (redirect_check(input) != 0)
		return (print_syntax_error_redir());
	return (1);
}
