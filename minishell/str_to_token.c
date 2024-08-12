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

t_token *str_to_token(char *str)
{
	t_token *root;
	t_token *token;
	int i;
	int start;

	i = -1;
	root = NULL;
	while (str[++i])
	{
		while(str[i] == ' ')
			i++;
		start = i;
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