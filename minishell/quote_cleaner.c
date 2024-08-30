#include "mini.h"

char	*token_dup_quote(t_token *tmp, int *flag, int i, int j)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (ft_strlen(tmp->str) + 1));
	while (tmp->str[i])
	{
		while (tmp->str[i] && ((tmp->str[i] != '\'' && tmp->str[i] != '\"')))
		{
			str[j++] = tmp->str[i++];
		}
		if (tmp->str[i] == '\"' || tmp->str[i] == '\'')
			toggle_quote(flag, tmp->str[i++]);
		while (*flag != -1 && tmp->str[i])
		{
			toggle_quote(flag, tmp->str[i]);
			if (*flag != -1)
				str[j++] = tmp->str[i++];
			else
				i++;
		}
	}
	str[j] = '\0';
	return (str);
}

void	token_quote_detective(t_token *tmp)
{
	char	*str;
	int		flag;

	flag = -1;
	str = token_dup_quote(tmp, &flag, 0, 0);
	tmp->str = ft_strdup(str);
}

void	token_del_quote(t_token *token_root)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = token_root;
	while (tmp)
	{
		if (is_has_quote(tmp->str))
			token_quote_detective(tmp);
		tmp = tmp->next;
	}
}

int	is_has_quote(char *str)
{
	int	i;
	int	flag;

	flag = -1;
	i = -1;
	while (str[++i])
	{
		toggle_quote(&flag, str[i]);
		if (flag == -1 && (str[i] == '\'' || str[i] == '\"'))
			return (1);
	}
	return (0);
}

int	is_only_quote(char *str)
{
	char	c;
	int		i;

	i = 0;
	c = str[0];
	while (str[i])
	{
		if (str[i] != c)
			return (0);
		i++;
	}
	return (1);
}
