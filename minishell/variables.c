#include "mini.h"

t_variables	*variables_new(char *key, char *value)
{
	t_variables	*variables;

	variables = (t_variables *)malloc(sizeof(t_variables));
	variables->key = key;
	variables->value = value;
	variables->next = NULL;
	return (variables);
}

void	variables_add_last(t_variables **root, t_variables *variables)
{
	t_variables	*tmp;

	if (*root == NULL)
	{
		*root = variables;
		return ;
	}
	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = variables;
	variables->next = NULL;
}

void	variables_list_printer(t_variables *root)
{
	t_variables	*tmp;

	tmp = root;
	while (tmp)
	{
		printf("key: %s\n", tmp->key);
		printf("value: %s\n", tmp->value);
		tmp = tmp->next;
	}
}
