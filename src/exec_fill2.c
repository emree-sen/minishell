/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fill2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emsen <emsen@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 10:09:27 by emsen             #+#    #+#             */
/*   Updated: 2024/09/17 12:46:19 by emsen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	ft_absoulte_path(char *cmd, t_exec *exec)
{
	struct stat	buf;

	stat(cmd, &buf);
	if (access(cmd, F_OK) == -1)
		return (ft_set_error(exec, 1271));
	else if (buf.st_mode & S_IFDIR)
		return (ft_set_error(exec, ERR_IS_A_DIRECTORY));
	else if ((buf.st_mode & S_IFREG) == 0)
		return (ft_set_error(exec, 1261));
	else if (access(cmd, X_OK) == -1)
		return (ft_set_error(exec, 1261));
	exec->path = ft_strdup(cmd);
}

void	ft_relavite_path(char *cmd, char **paths, t_exec *exec)
{
	char		*path;
	char		*tmp;
	int			i;
	struct stat	buf;

	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		if (ft_strcmp(tmp, path) != 0 && !access(path, F_OK))
		{
			stat(path, &buf);
			if (buf.st_mode & S_IFDIR)
				return (ft_free_split(paths), \
							ft_set_error(exec, ERR_IS_A_DIRECTORY));
			else if ((buf.st_mode & S_IFREG) == 0)
				return (ft_free_split(paths), ft_set_error(exec, 1261));
			exec->path = path;
			return (ft_free_split(paths), free(tmp));
		}
		free(tmp);
		free(path);
	}
	return (ft_free_split(paths), ft_set_error(exec, 1271));
}

void	path_finder(char *cmd, t_variables *var_root, t_exec *exec)
{
	char	**paths;
	char	*path;
	
	path = ft_getenv("PATH", var_root);
	paths = ft_split(path, ':');
	if (!paths)
		return ;
	if (is_has_slash(cmd))
	{
		ft_absoulte_path(cmd, exec);
		ft_free_split(paths);
	}
	else
		ft_relavite_path(cmd, paths, exec);
}

char	**args_filler(t_token *tmp, char *path)
{
	char	**args;
	int		i;
	int		arg_num;

	arg_num = arg_num_finder(tmp);
	args = malloc(sizeof(char *) * (arg_num + 2));
	if (!args)
		exit(1);
	i = 0;
	args[i] = ft_strdup(path);
	i++;
	while (tmp)
	{
		if (tmp->type == ARG)
		{
			args[i] = ft_strdup(tmp->str);
			i++;
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

int	count_heredocs(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			i++;
		tmp = tmp->next;
	}
	return (i);
}
