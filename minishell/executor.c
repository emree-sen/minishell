#include "mini.h"

void	state_arr_len_set(t_state *state)
{
	int	i;

	i = 0;
	while (state->token_arr[i])
		i++;
	state->arr_len = i;
}

int	*heredoc_create(t_state *state)
{
	int	i;
	int	*heredoc_fds;

	heredoc_fds = malloc(sizeof(int) * state->arr_len);
	if (!heredoc_fds)
		exit(1);
	i = 0;
	while (i < state->arr_len)
	{
		heredoc_fds[i] = -37;
		i++;
	}
	return (heredoc_fds);
}

void	heredoc_setter(t_exec *exec)
{
	int		pipe_fd[2];
	char	*input;

	input = NULL;
	if (pipe(pipe_fd) == -1)
		exit(1);
	exec->heredoc_idx = 0;
	while (exec->heredocs != NULL && exec->heredocs[exec->heredoc_idx])
	{
		if (input)
			free(input);
		input = readline("> ");
		if (!input)
			break ;
		if (!ft_strcmp(input, exec->heredocs[exec->heredoc_idx]))
		{
			if (exec->heredocs[exec->heredoc_idx + 1])
				exec->heredoc_idx++;
			else
				break ;
		}
		else
		{
			if (exec->heredocs[exec->heredoc_idx + 1])
				continue ;
			else
			{
				write(pipe_fd[1], input, ft_strlen(input));
				write(pipe_fd[1], "\n", 1);
			}	

		}
	}
	exec->in_fd = pipe_fd[0];
	close(pipe_fd[1]);
	free(input);
}

t_exec	**exec_create(t_state *state)
{
	int	i;
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
		exec[i]->path = NULL;
		exec[i]->args = NULL;
		exec[i]->input_file = NULL;
		exec[i]->output_file = NULL;
		exec[i]->in_fd = -1;
		exec[i]->out_fd = -1;
		exec[i]->output_type = -1;
		exec[i]->heredocs = NULL;
		exec[i]->heredoc_idx = 0;
		i++;
	}
	return (exec);
}

char	*ft_getenv(char *key, t_variables *var_root)
{
	t_variables	*tmp;

	tmp = var_root;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*path_finder(char *cmd, t_variables *var_root)
{
	char	*path;
	char	*tmp;
	char	**paths;
	int		i;

	paths = ft_split(ft_getenv("PATH", var_root), ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(path, F_OK))
		{
			ft_free_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

int	arg_num_finder(t_token *tmp)
{
	int	i;

	i = 0;
	while (tmp)
	{
		if (tmp->type == ARG)
			i++;
		tmp = tmp->next;
	}
	return (i);
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
				exec[j]->path = path_finder(tmp->str, var_root);
				exec[j]->args = args_filler(tmp, exec[j]->path);
			}
			else if (tmp->type == HEREDOC)
			{
				if (exec[j]->heredocs == NULL)
				{
					heredoc_num = count_heredocs(tmp);
					exec[j]->heredocs = malloc(sizeof(char *) * (heredoc_num + 1));
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
				exec[j]->input_file = ft_strdup(tmp->next->str);
				exec[j]->in_fd = open(exec[j]->input_file, O_RDONLY);
			}
			tmp = tmp->next;
		}
		i++;
	}
	return (exec);
}

void	exec_print(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	while (exec[i])
	{
		j = 0;
		if (exec[i]->path != NULL)
			printf("path: %s\n", exec[i]->path);
		while (exec[i]->args != NULL && exec[i]->args[j])
		{
			if (exec[i]->args != NULL)
				printf("args: %s\n", exec[i]->args[j]);
			j++;
		}
		j = 0;
		while (exec[i]->heredocs != NULL && exec[i]->heredocs[j])
		{
			if (exec[i]->heredocs != NULL)
				printf("heredocs: %s\n", exec[i]->heredocs[j]);
			j++;
		}
		i++;
	}
}

int **pds_filler(int **pds, t_state *state)
{
	int	i;

	i = 0;
	while (i < state->arr_len)
	{
		pds = malloc(sizeof(int *) * (state->arr_len + 1));
		if (!pds)
			exit(1);
		while (i < state->arr_len)
		{
			pds[i] = malloc(sizeof(int) * 2);
			if (!pds[i])
				exit(1);
			if (pipe(pds[i]) == -1)
				exit(1);
			i++;
		}
		pds[state->arr_len] = NULL;
	}
	return (pds);
}

void fd_closer(int **pds, int i, t_state *state)
{
	int j;

	j = 0;
	while (j < state->arr_len - 1)
	{
		if (j != i)
			close(pds[j][0]);
		if (j != i - 1)
			close(pds[j][1]);
		j++;
	}
}

void fd_setter(int **pds, int i, t_state *state)
{	
	if (i == 0)
	{
		dup2(pds[i][1], 1);
		close(pds[i][0]);
	}
	else if (i == state->arr_len - 1)
	{
		dup2(pds[i - 1][0], 0);
		close(pds[i - 1][1]);
	}
	else
	{
		dup2(pds[i - 1][0], 0);
		dup2(pds[i][1], 1);
		close(pds[i - 1][1]);
		close(pds[i][0]);
	}
	fd_closer(pds, i, state);
}


void	executor(t_state *state, t_variables *var_root)
{
	t_exec	**exec;
	int **pds;
	// int  *heredocs;
	int i;

	i = 0;
	state_arr_len_set(state);
	exec = exec_filler(state, var_root);
	exec_print(exec);
	while (exec[i])
	{
		heredoc_setter(exec[i]);
		i++;
	}

	pds = NULL;
	pds = pds_filler(pds, state);

	pid_t	pid;
	i = 0;
	while (i < state->arr_len)
	{
		pid = fork();

		if (pid < 0)
			exit(1); // hata kodu
		else if (pid == 0)
		{
			// child
			fd_setter(pds, i, state);
			if (exec[i]->in_fd != 1)
				dup2(exec[i]->in_fd, 0);
			if (exec[i]->out_fd != 1)
				dup2(exec[i]->out_fd, 1);
			execve(exec[i]->path, exec[i]->args, NULL);
		}
		else
		{
			// parent
			printf("parent\n");
		}
	}	
}


