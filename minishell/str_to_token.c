#include "mini.h"

int pass_quote(char *str, int qi)
{
	char q;
	q = str[qi];
	qi++;
	while (str[qi] && str[qi] != q)
		qi++;
	return (qi);
}

int pass_str(char *str, int si)
{
	while (str[si] && str[si] != '|' && str[si] != '>' && str[si] != '<')
		si++;
	return (si);
}

int pass_any(char *str, int *ai, char any)
{
	while (str[*ai] && str[*ai] == any)
		(*ai)++;
	return (*ai);
}


t_token *str_to_token(char *str)
{
	t_token *root;
	t_token *token;
	int i;
	int start;

	i = -1;
	root = NULL;
	str = ft_strtrim(str, " ");
	while (str[++i])
	{
		start = pass_any(str, &i, ' ');
		while(str[i])
		{
			if(str[i] == '\'' || str[i] == '\"')
				i = pass_quote(str,i);
			if(str[i] && str[i] != ' ')
				i++;
			else
				break;
		}
		token = token_new(ft_substr(str, start, i - start), NONE);
		token_add_last(&root, token);
	}
	return (root);
}
