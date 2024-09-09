#include "mini.h"

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
	if (exec->heredocs != NULL && exec->in_type == REDLL)
		exec->in_fd = pipe_fd[0];
	else
		close(pipe_fd[0]);
	close(pipe_fd[1]);
	free(input);
}
