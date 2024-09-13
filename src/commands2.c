#include "mini.h"

void single_command(t_exec **exec, int i)
{
	if (exec[i]->in_fd != -1)
		dup2(exec[i]->in_fd, 0);
	if (exec[i]->out_fd != -1)
		dup2(exec[i]->out_fd, 1);
}

void multi_command_without_redr(int **fds, int i, t_state *state)
{
	fd_setter_without_redr(fds, i, state);
}

void multi_command_with_redr(t_exec **exec, int i, int **fds, t_state *state)
{
	if (exec[i]->in_fd != -1)
	{
		dup2(exec[i]->in_fd, 0);
		close(exec[i]->in_fd);
	}
	if (exec[i]->out_fd != -1)
	{
		dup2(exec[i]->out_fd, 1);
		close(exec[i]->out_fd);
	}
	fd_setter_with_redr(exec, fds, i, state);
}

void	fd_setter_without_redr(int **fds, int i, t_state *state)
{
	if (i == 0)
	{
		dup2(fds[i][1], 1);
		close(fds[i][0]);
	}
	else if (i == state->arr_len - 1)
	{
		dup2(fds[i - 1][0], 0);
		close(fds[i - 1][1]);
	}
	else
	{
		dup2(fds[i - 1][0], 0);
		dup2(fds[i][1], 1);
		close(fds[i - 1][1]);
		close(fds[i][0]);
	}
	fd_closer(fds, i, state);
}

void dup_exec_in_out(t_exec **exec, int i)
{
	dup2(exec[i]->in_fd, 0);
	close(exec[i]->in_fd);
	dup2(exec[i]->out_fd, 1);
	close(exec[i]->out_fd);
}
