/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:10:29 by emsen             #+#    #+#             */
/*   Updated: 2024/09/14 15:49:41 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	print_syntax_error_quote(void)
{
	printf("Syntax error. Unclosed quote.\n");
	return (1);
}

int	print_syntax_error_pipe(void)
{
	printf("Syntax error. Invalid pipe using.\n");
	return (1);
}

int	print_unexpected_char_error(void)
{
	printf("Unexpected character using.\n");
	return (1);
}

int	print_syntax_error_redir(void)
{
	printf("Mixed redirect using.\n");
	return (1);
}
