#include "mini.h"

int	is_built_in(t_token *tmp)
{

	// if (ft_strcmp(tmp->str , "echo") == 0)
	// 	return (1);
	// if (ft_strcmp(tmp->str , "cd") == 0)
	// 	return (1);
	if (ft_strcmp(tmp->str , "pwd") == 0)
	 	return (1);
	else if (ft_strcmp(tmp->str , "export") == 0)
		return (1);
	else if (ft_strcmp(tmp->str , "unset") == 0)
	 	return (1);
	else if (ft_strcmp(tmp->str , "env") == 0)
	 	return (1);
	// else if (ft_strcmp(tmp->str , "exit") == 0)
	// 	return (1);
	return (0);
}

t_exec	**exec_filler(t_state *state, t_variables *var_root)
{
	int		i;
	int		j;
	t_exec	**exec;
	t_token	*tmp;
	int		heredoc_num;

	exec = exec_create(state);
	i = 0;
	j = -1;
	while (i < state->arr_len)
	{
		tmp = state->token_arr[i];
		j++;
		while (tmp)
		{
			if (tmp->type == CMD)
			{
				if (is_built_in(tmp) == 1)
				{
					exec[j]->cmd_type = BUILTIN;
					exec[j]->path = ft_strdup(tmp->str);
					exec[j]->args = args_filler(tmp, exec[j]->path);
				}
				if (exec[j]->cmd_type != BUILTIN)
				{
					exec[j]->path = path_finder(tmp->str, var_root);
					if (!exec[j]->path)
					{
						exec[j]->err_val = 127;
						exec[j]->err_str = ft_strdup("command not found");
					}
					else
						exec[j]->args = args_filler(tmp, exec[j]->path);
				}
			}
			else if (tmp->type == HEREDOC)
			{
				if (exec[j]->heredocs == NULL)
				{
					heredoc_num = count_heredocs(tmp);
					exec[j]->heredocs = malloc(sizeof(char *) * (heredoc_num
								+ 1));
					exec[j]->heredocs[heredoc_num] = NULL;
				}
				exec[j]->heredocs[exec[j]->heredoc_idx] = ft_strdup(tmp->str);
				exec[j]->heredoc_idx++;
			}
			else if (tmp->type == REDR)
			{
				exec[j]->output_file = ft_strdup(tmp->next->str);
				exec[j]->output_type = REDR;
				exec[j]->out_fd = open(exec[j]->output_file,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
			}
			else if (tmp->type == REDRR)
			{
				exec[j]->output_file = ft_strdup(tmp->next->str);
				exec[j]->output_type = REDRR;
				exec[j]->out_fd = open(exec[j]->output_file,
						O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
			else if (tmp->type == REDL)
			{
				exec[j]->in_type = REDL;
				exec[j]->input_file = ft_strdup(tmp->next->str);
				exec[j]->in_fd = open(exec[j]->input_file, O_RDONLY);
			}
			else if (tmp->type == REDLL)
				exec[j]->in_type = REDLL;
			tmp = tmp->next;
		}
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

char	*path_finder(char *cmd, t_variables *var_root)
{
	char	*path;
	char	*tmp;
	char	**paths;
	int		i;

	paths = ft_split(ft_getenv("PATH", var_root), ':');
	if (!paths)
		return (NULL);
	i = -1;
	if (is_has_slash(cmd))
	{
		if (!access(cmd, F_OK))
			return (ft_free_split(paths), ft_strdup(cmd));
	}
	else
	{
		while (paths[++i])
		{
			tmp = ft_strjoin(paths[i], "/");
			path = ft_strjoin(tmp, cmd);
			if (ft_strcmp(tmp, path) != 0 && !access(path, F_OK))
				return (ft_free_split(paths) , free(tmp), path);
			free(tmp);
			free(path);
		}
	}
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
