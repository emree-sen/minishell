#include "mini.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int is_in_quote(char *str, int i)
{
	int flag;
	int j;

	flag = -1;
	j = -1;
	while (str[++j] && j < i)
	{
		if (str[j] == '\'')
			toggle_quote(&flag, str[j]);
	}
	return (flag);
}


t_variables *dup_veriables(char **environ)
{
	t_variables *root;
	t_variables *variables;
	int i;
	char *key;
	char *value;

	i = -1;
	root = NULL;
	while (environ[++i])
	{
		key = ft_substr(environ[i], 0, ft_strchr(environ[i], '=') - environ[i]);
		value = ft_strdup(ft_strchr(environ[i], '=') + 1);
		variables = variables_new(key, value);
		variables_add_last(&root, variables);
	}
	return (root);
}

void token_split_dollars(t_token **token_root, t_variables *var_root)
{
	t_token *tmp;
	t_token *new;
	t_variables *var_tmp;
	int i;
	int j;
	int start;
	int end;
	char *key;
	char *value;

	tmp = *token_root;
	while (tmp)
	{
		i = -1;
		while (tmp->str[++i])
		{	
			if(is_in_quote(tmp->str, i) != -1 && tmp->str[i] == '$')
			{
				pass_quote(tmp->str, '\'');
			}
			else
			{
				start = i;
				i++;
				while (tmp->str[i] && ft_isalnum(tmp->str[i]))
					i++;
				end = i;
				key = ft_substr(tmp->str, start + 1, end - start - 1);
				var_tmp = var_root;
				while (var_tmp)
				{
					if (ft_strcmp(var_tmp->key, key) == 0)
					{
						value = var_tmp->value;
						new = token_new(value, NONE);
						token_add_prev(token_root, tmp, new);
						i = start + ft_strlen(value);
						break;
					}
					var_tmp = var_tmp->next;
				}
			}
		}
		tmp = tmp->next;
	}

}


int main()
{
	while (1)
	{
		extern char **environ;
		char **env = environ;
		// test str_to_token func in readline
		char *line;
		t_token *root;
		t_variables *var_root;
		line = readline("minishell$ ");
		add_history(line);
		var_root = dup_veriables(env);
		//variables_list_printer(var_root);

		root = str_to_token(line);
		/*
		t_token *new;
		new = token_new("new", NONE);
		token_add_prev(&root, root, new);
		*/

		token_extract_all_meta(&root);

		token_split_dollars(&root, var_root);
		token_list_printer(root);
	}
	
}