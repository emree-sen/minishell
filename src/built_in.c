#include "mini.h"

void new_variable_adder(t_variables *var_root, char *key, char *value)
{
	t_variables	*new;

	new = variables_new(key, value);
	variables_add_last(&var_root, new);
}

int	is_alphanumericorunderscore(char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && str[i] != '=')
		{
			if (!ft_isalnum(str[i]) && str[i] != '_')
				return (1);
			i++;
		}
		return (0);
	}
	return (1);
}

void	ft_export(t_state *state, t_variables *var_root, int i)
{
	t_token	*tmp;

	tmp = state->token_arr[i];
	while (tmp)
	{
		if (tmp->type == ARG)
		{
			if (!ft_isdigit(tmp->str[0]) && is_alphanumericorunderscore(tmp->str) == 0)
			{
				if (ft_strchr(tmp->str, '='))
					new_variable_adder(var_root, ft_substr(tmp->str, 0,
							ft_strchr(tmp->str, '=') - tmp->str),
						ft_substr(tmp->str,
							ft_strchr(tmp->str, '=') - tmp->str + 1,
							ft_strlen(tmp->str)));
			}
			else
			{
				write(2, "minishell: export: `", 20);
				write(2, tmp->str, ft_strlen(tmp->str));
				write(2, "': not a valid identifier\n", 26);
				state->status = 1;
			}
		}
		tmp = tmp->next;
	}
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
			if (ft_isalpha(token->str[0]) || token->str[0] == '_' || ft_strchr(token->str, '=') == NULL)
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

int is_just_n(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' && i == 0)
		{
			i++;
			continue ;
		}
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void ft_echo(char **args)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (args[i] && is_just_n(args[i]) == 1)
	{
		flag = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (flag == 0)
		write(1, "\n", 1);

}

void env_node_updater(t_variables *var_root, char *key, char *value)
{
	t_variables	*tmp;
	char		*tmp_line;

	tmp = var_root;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			free(tmp->line);
			tmp->line = ft_strjoin(key, "=");
			tmp_line = tmp->line;
			tmp->line = ft_strjoin(tmp->line, value);
			free(tmp_line);
			break ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
	{
		tmp = variables_new(key, value);
		variables_add_last(&var_root, tmp);
	}
}

void ft_cd(char **args, t_state *state, t_variables *var_root)
{
	char	*home;
	char	*oldpwd;
	struct stat	buf;

	stat(args[1], &buf);
	oldpwd = getcwd(NULL, 0);
	home = ft_getenv("HOME", var_root);
	if (!args[1])
	{
		if (chdir(home) == -1)
		{
			printf("minishell: cd: %s: %s\n", home, "HOME not set");
			state->status = ERR_NOT_A_DIRECTORY;
			return ;
		}
	}
	if (args[1])
	{
		if (chdir(args[1]) == -1)
		{
			if (access(args[1], F_OK) == -1)
				printf_spesific_error(ENOENT, args[1]);
			else if (buf.st_mode & S_IFDIR)
				printf_spesific_error(EACCES, args[1]);
			else if (buf.st_mode & S_IFREG)
				printf_spesific_error(ENOTDIR, args[1]);
			state->status = 1;
		}
		else
			env_node_updater(var_root, "OLDPWD", oldpwd);
	}
	env_node_updater(var_root, "PWD", getcwd(NULL, 0));
}

int ft_arr_len(t_token *tmp)
{
	int	len;

	len = 0;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (1);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void ft_exit(t_state *state, int i)
{
	t_token	*tmp;
	int		len;

	tmp = state->token_arr[i];
	len = ft_arr_len(tmp);
	if (len > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		state->status = 1;
		exit(state->status);
	}
	else if (len == 2)
	{
		if (tmp->next->type == ARG)
		{
			if (is_numeric(tmp->next->str) == 0)
			{
				state->status = ft_atoi(tmp->next->str);
				write(2, "exit\n", 5);
			}
			else
			{
				printf_spesific_error(ERR_NUMERIC_ARG, tmp->next->str);
				state->status = ERR_NUMERIC_ARG;
			}
			exit(state->status);
		}
	}
	else
	{
		state->status = 0;
		exit(state->status);
	}
}
