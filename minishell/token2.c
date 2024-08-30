#include "mini.h"

void	token_add_next(t_token *token, t_token *new)
{
	t_token	*tmp;

	tmp = token->next;
	token->next = new;
	new->prev = token;
	new->next = tmp;
	if (tmp)
		tmp->prev = new;
}

void	token_list_printer(t_token *root)
{
	t_token	*tmp;

	tmp = root;
	while (tmp)
	{
		printf("String: %s, Type: %u\n", tmp->str, tmp->type);
		tmp = tmp->next;
	}
}
