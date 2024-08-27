#include "mini.h"

int	is_pipe_first(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] == ' ')
	{
		i++;
	}
	if (input[i] == '|')
	{
		return (1);
	}
	return (0);
}

int	is_pipe_last(char *input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	i--;
	while (input[i] == ' ')
		i--;
	if (input[i] == '|')
		return (1);
	return (0);
}

int	double_pipe(char *input)
{
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if ((input[i] == '|' && input[i + 1] == '|') && quote == 0)
			return (1);
		i++;
	}
	return (0);
}

int	redir_plus_pipe_two(char *input)
{
	int	i;
	int	quote;
	
	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<')) && quote == 0)
		{
			i += 2;
			i = pass_the_spaces(input, i);
			if (input[i] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int	redir_plus_pipe(char *input)
{
	int	i;
	int	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '>' || input[i] == '<')
		{
			i++;
			i = pass_the_spaces(input, i);
			if (input[i] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int	print_syntax_error_quote()
{
	printf("Syntax error. Unclosed quote.\n");
	return (0);
}

int	print_syntax_error_pipe()
{
	printf("Syntax error. Invalid pipe using.\n");
	return (0);
}

int	print_unexpected_char_error()
{
	printf("Unexpected character using.\n");
	return (0);
}

int	print_syntax_error_redir()
{
	printf("Mixed redirect using.\n");
	return (0);
}


int	mixed_redir(char *input)
{
	int	i;
	int	quote;
	
	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '<' && quote == 0)
		{
			i++;
			i = pass_the_spaces(input, i);
			if (input[i] == '>')
				return (1);
		}
		i++;
	}
	return (0);
}

int	mixed_redir_three(char *input)
{
	int	i;
	int	quote;
	
	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '<' && input[i + 1] == '<' && quote == 0)
		{
			i += 2;
			i = pass_the_spaces(input, i);
			if (input[i] == '>' || input[i] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

int	mixed_redir_four(char *input)
{
	int	i;
	int	quote;
	
	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '>' && input[i + 1] == '>' && quote == 0)
		{
			i += 2;
			i = pass_the_spaces(input, i);
			if (input[i] == '>' || input[i] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

int	mixed_redir_two(char *input)
{
	int	i;
	int	quote;
	
	quote = 0;
	i = 0;
	while (input[i])
	{
		quote = pass_the_quotes(input[i], quote);
		if (input[i] == '>' && quote == 0)
		{
			i++;
			i = pass_the_spaces(input, i);
			if (input[i] == '<')
				return (1);
		}
		i++;
	}
	return (0);
}

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
	int quote;

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
		if(input[i] == '\\' && quote == 0)
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

int	redirect_check(char *input)
{
	if (mixed_redir(input) != 0 || mixed_redir_two(input) != 0 || mixed_redir_three(input) != 0 || mixed_redir_four(input) != 0)
		return (1);
	if (redir_plus_pipe(input) != 0 || redir_plus_pipe_two(input) || last_arg_is_redir(input))
		return (1);
	return (0);
}

int	check_the_syntax(char *input)
{
	if (all_closed_quotes(input) != 0)
		return (print_syntax_error_quote());
	if (is_pipe_first(input) != 0 || is_pipe_last(input) != 0 || double_pipe(input) != 0)
		return (print_syntax_error_pipe());
	if (backslash_check(input) == 1 || semicolon_check(input) == 1)
		return (print_unexpected_char_error());
	if (redirect_check(input) != 0)
		return (print_syntax_error_redir());
	return (1);
}

int	pass_the_quotes(char c, int quote)
{
	if ((c == '\'' || c == '\"') && quote == 0)
		quote = c;
	else if ((quote == c) && quote != 0)
		quote = 0;
	return (quote);
}

int	pass_the_spaces(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	return (i);
}