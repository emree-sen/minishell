#include "mini.h"

void	state_arr_len_set(t_state *state)
{
	int	i;

	i = 0;
	while (state->token_arr[i])
		i++;
	state->arr_len = i;
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
		printf("input_file: %s\n", exec[i]->input_file);
		printf("in_fd: %d\n", exec[i]->in_fd);
		printf("output_file: %s\n", exec[i]->output_file);
		printf("out_fd: %d\n", exec[i]->out_fd);
		i++;
	}
}

void	wait_all_children(int arr_len)
{
	int	i;
	int	status;

	i = 0;
	while (i < arr_len)
	{
		waitpid(-1, &status, 0);
		i++;
	}
}

void	ft_print_exec_errors(t_exec **exec)
{
	int	i;

	i = 0;
	while (exec[i])
	{
		if (exec[i]->err_val != 0)
			printf("%s\n", exec[i]->err_str);
		i++;
	}
}

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

void single_command_built_in(t_exec **exec, t_state *state, t_variables *var_root, int i)
{
	if (exec[i]->cmd_type == BUILTIN)
	{
		// if (ft_strcmp(exec[i]->args[0], "echo") == 0)
		// 	ft_echo(exec[i]->args);
		// else if (ft_strcmp(exec[i]->args[0], "cd") == 0)
		// 	ft_cd(exec[i]->args);
		if (ft_strcmp(exec[i]->args[0], "pwd") == 0)
		 	ft_pwd();
		else if (ft_strcmp(exec[i]->args[0], "export") == 0)
			ft_export(state, var_root, i);
		else if (ft_strcmp(exec[i]->args[0], "unset") == 0)
		 	ft_unset(var_root, state, i);
		else if (ft_strcmp(exec[i]->args[0], "env") == 0)
			ft_env(var_root);
		// else if (ft_strcmp(exec[i]->args[0], "exit") == 0)
		// 	ft_exit(exec[i]->args);
	}
}

void	executor(t_state *state, t_variables *var_root)
{
	t_exec	**exec;
	int		**fds;
	char	**env;
	pid_t	*pid;
	int		i;

	i = 0;
	state_arr_len_set(state);
	exec = exec_filler(state, var_root);
	//exec_print(exec);
	while (exec[i])
	{
		heredoc_setter(exec[i]);
		i++;
	}
	ft_print_exec_errors(exec);
	fds = NULL;
	if (state->arr_len > 1)
		fds = fds_filler(fds, state);
	pid = malloc(sizeof(pid_t) * state->arr_len);
	i = 0;
	while (i < state->arr_len)
	{
		if (state->arr_len == 1 && exec[i]->cmd_type == BUILTIN)
			single_command_built_in(exec, state, var_root, i);
		pid[i] = fork();
		if (pid[i] < 0)
			exit(1); // hata kodu
		else if (pid[i] == 0)
		{
			if (state->arr_len == 1)
			{
				single_command(exec, i);
			}
			else if (state->arr_len > 1)
			{
				if (exec[i]->in_fd != -1 || exec[i]->out_fd != -1)
					multi_command_with_redr(exec, i, fds, state);
				else
					multi_command_without_redr(fds, i, state);
			}
			if (exec[i]->path == NULL)
				exit(127);
			env = env_list_creator(var_root);
			execve(exec[i]->path, exec[i]->args, env);
			exit(127);
		}
		i++;
	}
	i = 0;
	close_all_fd(fds, state);
	while (i < state->arr_len)
	{
		if (i == state->arr_len - 1)
		{
			waitpid(pid[i], &state->status, 0);
			if (WIFEXITED(state->status))
				state->status = WEXITSTATUS(state->status);
		}
		waitpid(pid[i], NULL, 0);
		i++;
	}
	// parent
}
