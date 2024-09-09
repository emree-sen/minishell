#include "mini.h"

void	token_value_checker(t_variables *var_tmp, char *key, char **value)
{
	if (!ft_strcmp(var_tmp->key, key))
	{
		*value = ft_strdup(var_tmp->value);
		return ;
	}
}

t_variables	*dup_veriables(char **environ)
{
	t_variables	*root;
	t_variables	*variables;
	int			i;
	char		*key;
	char		*value;

	i = -1;
	root = NULL;
	while (environ[++i])
	{
		key = ft_substr(environ[i], 0, ft_strchr(environ[i], '=') - environ[i]);
		value = ft_strdup(ft_strchr(environ[i], '=') + 1);
		variables = variables_new(key, value);
		variables->line = ft_strdup(environ[i]);
		variables_add_last(&root, variables);
	}
	return (root);
}

char	*token_value_finder(t_token *tmp,
	t_dollar *dollar, t_variables *var_root)
{
	int			start;
	int			end;
	char		*value;
	t_variables	*var_tmp;

	value = NULL;
	start = dollar->i;
	end = dollar->i;
	end++;
	if (ft_isdigit(tmp->str[end]) == 1)
		end++;
	else
	{
		while (tmp->str[end] && (ft_isalnum(tmp->str[end]) || \
			tmp->str[end] == '_' || tmp->str[end] == '?'))
			end++;
	}
	dollar->key = ft_substr(tmp->str, start + 1, end - start - 1);
	var_tmp = var_root;
	while (var_tmp)
	{
		token_value_checker(var_tmp, dollar->key, &value);
		var_tmp = var_tmp->next;
	}
	return (value);
}

void	token_split_dollars(t_token **token_root,
		t_variables *var_root, t_state *state)
{
	t_token		*tmp;
	t_dollar	dllr;

	dllr.value = NULL;
	dllr.flag = -1;
	dllr.flag2 = -1;
	tmp = *token_root;
	while (tmp)
	{
		dllr.i = -1;
		while (tmp->str[++dllr.i])
		{
			toggle_single_quote(&dllr.flag, tmp->str[dllr.i], &dllr.flag2);
			if (dllr.flag == -1 && tmp->str[dllr.i] == '$')
			{
				dllr.value = token_value_finder(tmp, &dllr, var_root);
				token_replace_value(&tmp, &dllr, &dllr.i, state);
			}
		}
		tmp = tmp->next;
	}
	token_extract_spaces(token_root);
}

void	token_replace_value(t_token **str, t_dollar *dollar,
		int *i, t_state *state)
{
	int		end;
	char	*left;
	char	*right;

	end = *i + ft_strlen(dollar->key);
	left = ft_substr((*str)->str, 0, *i);
	right = ft_substr((*str)->str, end + 1, ft_strlen((*str)->str) - end);
	if (!dollar->value)
	{
		if (ft_strcmp(dollar->key, "?") == 0)
			dollar->value = ft_itoa(state->status);
		else if (ft_strcmp(dollar->key, "0") == 0)
			dollar->value = ft_strdup("minishell");
		else
			dollar->value = ft_strdup("");
		*i = *i - 1;
	}
	else
		dollar->value = ft_strdup(dollar->value);
	(*str)->str = ft_strjoin(left, dollar->value);
	(*str)->str = ft_strjoin((*str)->str, right);
	free(left);
	free(right);
}
