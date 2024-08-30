#include "mini.h"

void	set_token_type(t_token *token)
{
	if (ft_strcmp(token->str, "|") == 0)
		token->type = PIPE;
	else if (ft_strcmp(token->str, ">") == 0)
		token->type = REDR;
	else if (ft_strcmp(token->str, "<") == 0)
		token->type = REDL;
	else if (ft_strcmp(token->str, ">>") == 0)
		token->type = REDRR;
	else if (ft_strcmp(token->str, "<<") == 0)
		token->type = REDLL;
	else
		token->type = ARG;
}

void	handle_redirection(t_token **token)
{
	t_token	*tmp;

	tmp = *token;
	if (tmp->type == REDR || tmp->type == REDL || tmp->type == REDRR)
	{
		if (tmp->next && tmp->next->str)
		{
			tmp = tmp->next;
			tmp->type = FILEE;
		}
	}
	else if (tmp->type == REDLL)
	{
		if (tmp->next && tmp->next->str)
		{
			tmp = tmp->next;
			tmp->type = HEREDOC;
		}
	}
	*token = tmp;
}

void	token_arr_set_type(t_token **token_arr)
{
	int		i;
	t_token	*token_root;

	i = 0;
	while (token_arr[i])
	{
		token_root = token_arr[i];
		token_root->type = CMD;
		token_root = token_root->next;
		while (token_root)
		{
			set_token_type(token_root);
			handle_redirection(&token_root);
			token_root = token_root->next;
		}
		i++;
	}
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
