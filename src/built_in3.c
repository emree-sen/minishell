/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:10 by emsen             #+#    #+#             */
/*   Updated: 2024/09/19 15:25:19 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	is_just_n(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' && i == 0 && ft_strlen(str) > 1)
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

void	ft_echo(char **args)
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

void	env_node_updater(t_variables *var_root, char *key, char *value)
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

void	chdir_check(char **args, t_state *state,
			t_variables *var_root, char *oldpwd)
{
	struct stat	buf;

	stat(args[1], &buf);
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

void	ft_cd(char **args, t_state *state, t_variables *var_root)
{
	char		*home;
	char		*oldpwd;
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
		chdir_check(args, state, var_root, oldpwd);
	env_node_updater(var_root, "PWD", oldpwd);
	free(oldpwd);
}
