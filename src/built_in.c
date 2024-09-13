#include "mini.h"

void ft_pwd()
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
}

int ft_arr_len(t_token *tmp)
{
	int	len;

	len = 0;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (1);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void arg_check(t_token *tmp, t_state *state)
{
	if (tmp->type == ARG)
	{
		if (is_numeric(tmp->str) == 0)
		{
			state->status = ft_atoi(tmp->str);
			write(1, "exit\n", 5);
		}
		else
		{
			printf_spesific_error(ERR_NUMERIC_ARG, tmp->str);
			state->status = ERR_NUMERIC_ARG;
		}
		exit(state->status);
	}
}

void ft_exit(t_state *state, int i)
{
	t_token	*tmp;
	int		len;

	tmp = state->token_arr[i];
	len = ft_arr_len(tmp);
	if (len > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		state->status = 1;
		exit(state->status);
	}
	else if (len == 2)
		arg_check(tmp->next, state);
	else
	{
		state->status = 0;
		exit(state->status);
	}
}
