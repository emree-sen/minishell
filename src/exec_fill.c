#include "mini.h"

int	is_built_in(t_token *tmp)
{

	if (ft_strcmp(tmp->str , "echo") == 0)
	 	return (1);
	if (ft_strcmp(tmp->str , "cd") == 0)
		return (1);
	else if (ft_strcmp(tmp->str , "pwd") == 0)
	 	return (1);
	else if (ft_strcmp(tmp->str , "export") == 0)
		return (1);
	else if (ft_strcmp(tmp->str , "unset") == 0)
	 	return (1);
	else if (ft_strcmp(tmp->str , "env") == 0)
	 	return (1);
	else if (ft_strcmp(tmp->str , "exit") == 0)
	  	return (1);
	return (0);
}

void	init_heredoc(t_token *tmp, t_exec *exec)
{
	int	heredoc_num;

	if (exec->heredocs == NULL)
		{
			heredoc_num = count_heredocs(tmp);
			exec->heredocs = malloc(sizeof(char *) * (heredoc_num
						+ 1));
			exec->heredocs[heredoc_num] = NULL;
		}
		exec->heredocs[exec->heredoc_idx] = ft_strdup(tmp->str);
		exec->heredoc_idx++;
}

void	init_redr(t_token *tmp, t_exec *exec)
{
	exec->output_file = ft_strdup(tmp->next->str);
	exec->output_type = REDR;
	exec->out_fd = open(exec->output_file,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (exec->out_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redrr(t_token *tmp, t_exec *exec)
{
	exec->output_file = ft_strdup(tmp->next->str);
	exec->output_type = REDRR;
	exec->out_fd = open(exec->output_file,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (exec->out_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redll(t_token *tmp, t_exec *exec)
{
	exec->in_type = REDLL;
	exec->input_file = ft_strdup(tmp->next->str);
	exec->in_fd = open(exec->input_file, O_RDONLY);
	if (exec->in_fd == -1)
	{
		exec->err_val = 1;
		exec->err_str = ft_strdup(strerror(errno));
	}
}

void	init_redirection(t_token *tmp, t_exec *exec)
{
	if (tmp->type == HEREDOC)
		init_heredoc(tmp, exec);
	else if (tmp->type == REDR && exec->err_val == 0)
		init_redr(tmp, exec);
	else if (tmp->type == REDRR && exec->err_val == 0)
		init_redrr(tmp, exec);
	else if (tmp->type == REDL && exec->err_val == 0)
		init_redll(tmp, exec);
	else if (tmp->type == REDLL && exec->err_val == 0)
		exec->in_type = REDLL;
}

void fill_exec(t_exec *exec, t_token *tmp, t_variables *var_root)
{
	while (tmp)
	{
		if (tmp->type == CMD)
		{
			if (is_built_in(tmp) == 1)
			{
				exec->cmd_type = BUILTIN;
				exec->path = ft_strdup(tmp->str);
				exec->args = args_filler(tmp, exec->path);
			}
			if (exec->cmd_type != BUILTIN)
			{
				path_finder(tmp->str, var_root, exec);
				if (exec->path != NULL)
					exec->args = args_filler(tmp, exec->path);
			}
		}
		init_redirection(tmp, exec);
		tmp = tmp->next;
	}
}

t_exec	**exec_filler(t_state *state, t_variables *var_root)
{
	int		i;
	int		j;
	t_exec	**exec;

	exec = exec_create(state);
	i = 0;
	j = -1;
	while (i < state->arr_len)
	{
		fill_exec(exec[i], state->token_arr[i], var_root);
		i++;
	}
	return (exec);
}

t_exec	**exec_create(t_state *state)
{
	int		i;
	t_exec	**exec;

	exec = malloc(sizeof(t_exec *) * (state->arr_len + 1));
	if (!exec)
		exit(1);
	exec[state->arr_len] = NULL;
	i = 0;
	while (i < state->arr_len)
	{
		exec[i] = malloc(sizeof(t_exec));
		if (!exec[i])
			exit(1);
		exec_init(exec[i]);
		i++;
	}
	return (exec);
}

void	ft_set_error(t_exec *exec, int err)
{
	exec->err_val = err;
	exec->err_str = ft_strdup(strerror(err));
}

char	*path_finder(char *cmd, t_variables *var_root, t_exec *exec)
{
	char	*path;
	char	*tmp;
	char	**paths;
	int		i;
	struct stat	buf;

	stat(cmd, &buf);
	paths = ft_split(ft_getenv("PATH", var_root), ':');
	if (!paths)
		return (NULL);
	i = -1;
	if (is_has_slash(cmd))
	{
		if (access(cmd, F_OK) == -1)
			return (ft_free_split(paths), ft_set_error(exec, 1271), NULL);
		else if (buf.st_mode & S_IFDIR)
			return (ft_free_split(paths), ft_set_error(exec, ERR_IS_A_DIRECTORY), NULL);
		else if (access(cmd, X_OK) == -1)
			return (ft_free_split(paths), ft_set_error(exec, 1261), NULL);
	}
	else
	{
		while (paths[++i])
		{
			tmp = ft_strjoin(paths[i], "/");
			path = ft_strjoin(tmp, cmd);
			if (ft_strcmp(tmp, path) != 0 && !access(path, F_OK))
			{
				stat(path, &buf);
				if (buf.st_mode & S_IFDIR)
					return (ft_free_split(paths), ft_set_error(exec, ERR_IS_A_DIRECTORY), NULL);
				else if ((buf.st_mode & S_IFREG) == 0)
					return (ft_free_split(paths), ft_set_error(exec, 1261), NULL);
				exec->path = path;
				return (ft_free_split(paths), free(tmp), path);
			}
			free(tmp);
			free(path);
		}
	}
	ft_set_error(exec, 1271);
	return (ft_free_split(paths) , NULL);
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
