/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:33 by emsen             #+#    #+#             */
/*   Updated: 2024/09/18 13:22:12 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

char	**env_list_creator(t_variables *var_root)
{
	t_variables	*tmp;
	char		**env;
	int			i;

	tmp = var_root;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		exit(1);
	i = 0;
	tmp = var_root;
	while (tmp)
	{
		env[i] = ft_strdup(tmp->line);
		i++;
		tmp = tmp->next;
	}
	env[i] = NULL;
	return (env);
}

void	single_command_built_in(t_exec **exec,
	t_state *state, t_variables *var_root, int i)
{
	if (exec[i]->cmd_type == BUILTIN)
	{
		if (ft_strcmp(exec[i]->args[0], "echo") == 0)
			ft_echo(exec[i]->args);
		else if (ft_strcmp(exec[i]->args[0], "cd") == 0)
			ft_cd(exec[i]->args, state, var_root);
		else if (ft_strcmp(exec[i]->args[0], "pwd") == 0)
			ft_pwd();
		else if (ft_strcmp(exec[i]->args[0], "export") == 0)
			ft_export(state, var_root, i);
		else if (ft_strcmp(exec[i]->args[0], "unset") == 0)
			ft_unset(var_root, state, i);
		else if (ft_strcmp(exec[i]->args[0], "env") == 0)
			ft_env(var_root);
		else if (ft_strcmp(exec[i]->args[0], "exit") == 0)
			ft_exit(state, i);
	}
}

void	exit_num(int ex_num)
{
	if (ex_num == 127)
		exit(127);
	else if (ex_num == 126 || ex_num == 1261)
		exit(126);
	else if (ex_num == 1271)
		exit(127);
	else if (ex_num == 258)
		exit(258);
	else
		exit(1);
}

void	ft_free_exec(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	while (exec[i])
	{
		j = 0;
		while (exec[i]->args && exec[i]->args[j])
			free(exec[i]->args[j++]);
		if (exec[i]->args)
			free(exec[i]->args);
		j = 0;
		while (exec[i]->heredocs && exec[i]->heredocs[j])
			free(exec[i]->heredocs[j++]);
		if (exec[i]->path)
			free(exec[i]->path);
		if (exec[i]->heredocs)
			free(exec[i]->heredocs);
		free(exec[i]);
		i++;
	}
	free(exec);
}
