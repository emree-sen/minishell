#include "mini.h"

t_token *token_new(char *str, t_token_type type)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	token->str = str;
	token->type = type;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void token_add_last(t_token **root, t_token *token)
{
	t_token *tmp;

	if (*root == NULL)
	{
		*root = token;
		return;
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	token->next = NULL;
}

void token_del(t_token *token)
{
	if(token->prev)
		token->prev->next = token->next;
	else
		token->prev = NULL;
	if(token->next)
		token->next->prev = token->prev;
	else
		token->next = NULL;
	free(token->str);
	free(token);
}

void token_add_start(t_token **root, t_token *token)
{
	t_token *tmp;

	if (*root == NULL)
	{
		*root = token;
		return;
	}
	tmp = *root;
	tmp->prev = token;
	token->next = tmp;
	*root = token;
}

void token_add_prev(t_token **root, t_token *token, t_token *new)
{
	t_token *tmp;

	if(token->prev)
	{
		tmp = token->prev;
		tmp->next = new;
		new->prev = tmp;
		new->next = token;
		token->prev = new;
	}
	else
		token_add_start(root, new);
}

void token_add_next(t_token *token, t_token *new)
{
	t_token *tmp;

	tmp = token->next;
	token->next = new;
	new->prev = token;
	new->next = tmp;
	if (tmp)
		tmp->prev = new;
}

void token_list_printer(t_token *root)
{
	t_token *tmp;

	tmp = root;
	while (tmp)
	{
		printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
}