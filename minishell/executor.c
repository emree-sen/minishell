#include "mini.h"

void state_arr_len_set(t_state *state)
{
	int i;

	i = 0;
	while (state->token_arr[i])
		i++;
	state->arr_len = i;
}

int *heredoc_create(t_state *state)
{
	int i;
	int *heredoc_fds;

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

void heredoc_setter(t_state *state, int *heredoc_fds)
{
	int i;
	int pipe_fd[2];
	char *line;

	i = 0;
	while (i < state->arr_len)
	{
		while (state->token_arr[i])
		{
			if (state->token_arr[i]->type == HEREDOC)
			{
				pipe(pipe_fd);
				line = readline("> ");
				while (ft_strncmp(line, state->token_arr[i]->str, ft_strlen(state->token_arr[i]->str) + 1))
				{
					write(pipe_fd[1], line, ft_strlen(line));
					write(pipe_fd[1], "\n", 1);
					free(line);
					line = readline("> ");
				}
				heredoc_fds[i] = pipe_fd[0];
				close(pipe_fd[1]);
			}
			state->token_arr[i] = state->token_arr[i]->next;
		}
		i++;
	}	
}


/*void set_pipes(t_state *state, int *heredocs)
{
	int i;
	int *pipe_fd;
	
	while (i < state->arr_len - 1)
	{

	}

}*/

// void single_command_executor(t_state *state, int *heredocs)
// {
	
// }

void executor(t_state *state)
{
	int  *heredocs;
	//int i;

	state_arr_len_set(state);
	heredocs = heredoc_create(state);
	heredoc_setter(state, heredocs);
	//i = 0;

	// if (state->arr_len == 1)
	// 	//single_command_executor(state, heredocs);
	// else
	// {
	// 	//set_pipes(state, heredocs);
	// 	//multi_command_executor(state, heredocs);
	// }
}
