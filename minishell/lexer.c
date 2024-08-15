#include "mini.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void toggle_single_quote(int *quote, char c)
{
	if (*quote == -1 && c == '\'')
		*quote = (int)c;
	else if (*quote == c)
		*quote = -1;
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
	t_variables *var_tmp;
	int i;
	int start;
	int end;
	char *key;
	char *value;
	int flag;

	key = NULL;
	value = NULL;
	flag = -1;
	tmp = *token_root;
	while (tmp)
	{
		i = 0;
		while (tmp->str[i])
		{
			toggle_single_quote(&flag, tmp->str[i]);
			if (flag == -1 && tmp->str[i] == '$')
			{
				start = i;
				end = i;
				end++;
				if (ft_isdigit(tmp->str[end]) == 1)
					end++;
				else
				{
					while (tmp->str[end] && (ft_isalnum(tmp->str[end]) || tmp->str[end] == '_'))
						end++;					
				}
				key = ft_substr(tmp->str, start + 1, end - start - 1);
				var_tmp = var_root;
				while (var_tmp)
				{
					if (!ft_strcmp(var_tmp->key, key))
					{
						value = var_tmp->value;
						break;
					}
					var_tmp = var_tmp->next;
				}
				
				token_replace_dollars(&tmp->str, key, value, start);
				i = start + ft_strlen(value);
				printf("%s", tmp->str);
				printf("key: %s\n", key);
				printf("value: %s\n", value);
			}
			i++;
		}
		tmp = tmp->next;
	}
}

// void token_replace_dollars(char **token, char *key, char *value, int start)
// {

// }

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
		// variables_list_printer(var_root);

		root = str_to_token(line);
		/*
		t_token *new;
		new = token_new("new", NONE);
		token_add_prev(&root, root, new);
		*/

		token_extract_all_meta(&root);
		token_split_dollars(&root, var_root);

		// token_split_dollars(&root, var_root);
		token_list_printer(root);
	}
}