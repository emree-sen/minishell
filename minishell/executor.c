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
// char ** heredocs = {"a", "b"};
// int in_fd = pipe_fd[0];
// if input == heredoc?
//    if heredoc == last_heredoc? -> not
//        continue ;
//    else
//        break ;
// else
//    if curren_heredoc == last_heredoc -> not
//        continue ;
//    else
//        write(pipe_fd[1], input, ft_strlen(input));
//


void	heredoc_setter(t_exec *exec)
{
	int		pipe_fd[2];
	char	*input;

	input = NULL;
	if (pipe(pipe_fd) == -1)
		exit(1);
	exec->heredoc_idx = 0;
	while (exec->heredocs[exec->heredoc_idx])
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
				write(pipe_fd[1], input, ft_strlen(input));
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
		exec[i]->input_type = -1;
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
	// t_variables *var_tmp;	
	exec = exec_create(state);
	i = 0;
	j = -1;
	while (i < state->arr_len)
	{
		tmp = state->token_arr[i];
		printf("j: %d\n", j);
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
			// else if (tmp->type == ARG)
			// {
			// 	//exec[j]->args = args_filler(tmp);
			// }
			// else if (tmp->type == REDR)
			// {
			// 	exec[j]->output_file = ft_strdup(tmp->next->str);
			// 	exec[j]->out_fd = open(exec[j]->output_file,
			//			O_WRONLY | O_CREAT | O_TRUNC, 0644);
			// }
			// else if (tmp->type == REDRR)
			// {
			// 	exec[j]->output_file = ft_strdup(tmp->next->str);
			// 	exec[j]->out_fd = open(exec[j]->output_file,
			//			O_WRONLY | O_CREAT | O_APPEND, 0644);
			// }
			// else if (tmp->type == REDL)
			// {
			// 	exec[j]->input_file = ft_strdup(tmp->next->str);
			// 	exec[j]->in_fd = open(exec[j]->input_file, O_RDONLY);
			// }
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
		printf("path: %s\n", exec[i]->path);
		while (exec[i]->args[j])
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

void	executor(t_state *state, t_variables *var_root)
{
	t_exec	**exec;

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
	execve(exec[0]->path, exec[0]->args, NULL);
	// heredocs = heredoc_create(state);
	// heredoc_setter(state, heredocs);
	// i = 0;
	// if (state->arr_len == 1)
	// 	//single_command_executor(state, heredocs);
	// else
	// {
	// 	//set_pipes(state, heredocs);
	// 	//multi_command_executor(state, heredocs);
	// }
}
