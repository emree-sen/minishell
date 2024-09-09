#include "mini.h"

// export 

void	ft_export(t_state *state, t_variables *var_root, int i)
{
	t_variables	*new;
	int		flag;
	t_token	*tmp;

	tmp = state->token_arr[i];
	flag = 0;
	while (tmp)
	{
		if (tmp->type == ARG)
		{
			if (ft_isalpha(tmp->str[0]) || tmp->str[0] == '_')
			{
				if (ft_strchr(tmp->str, '='))
				{
					new = variables_new(ft_substr(tmp->str, 0, ft_strchr(tmp->str, '=') - tmp->str),
						ft_substr(tmp->str, ft_strchr(tmp->str, '=') - tmp->str + 1, ft_strlen(tmp->str)));
					new->line = ft_strdup(tmp->str);
					variables_add_last(&var_root, new);
				}
			}
			else
			{
				printf("minishell: export: `%s': not a valid identifier\n", tmp->str);
				state->status = ERR_NOT_VALID_IDFR;
			}
		}
		tmp = tmp->next;
	}
	//variables_list_printer(var_root);
}

void ft_unset(t_variables *var_root, t_state *state, int i)
{
	t_variables	*tmp;
	t_token		*token;

	token = state->token_arr[i];
	while (token)
	{
		if (token->type == ARG)
		{
			if (ft_isalpha(token->str[0]) || token->str[0] == '_')
			{
				tmp = var_root;
				while (tmp)
				{
					if (!ft_strcmp(tmp->key, token->str))
					{
						variables_del(tmp);
						break ;
					}
					tmp = tmp->next;
				}
			}
			else
			{
				printf("minishell: unset: `%s': not a valid identifier\n", token->str);
				state->status = ERR_NOT_VALID_IDFR;
			}
		}
		token = token->next;
	}
}

void ft_env(t_variables *var_root)
{
	t_variables	*tmp;

	tmp = var_root;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
}

void ft_pwd()
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
}
