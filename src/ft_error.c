#include "mini.h"

void	printf_spesific_error(int err_type, char *str)
{
	if (err_type == ERR_NOT_VALID_IDFR)
	{
		write(2, "minishell: unset: `", 19);
		write(2, str, ft_strlen(str));
		write(2, "': not a valid identifier\n", 26);
	}
	else if (err_type == ENOENT)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
		write(2, strerror(err_type), ft_strlen(strerror(err_type)));
		write(2, "\n", 1);
	}
	else if (err_type == EACCES)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
		write(2, strerror(err_type), ft_strlen(strerror(err_type)));
		write(2, "\n", 1);
	}
	else if (err_type == ENOTDIR)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
		write(2, strerror(err_type), ft_strlen(strerror(err_type)));
		write(2, "\n", 1);
	}
	else if (err_type == ERR_NUMERIC_ARG)
	{
		write(2, "minishell: exit: ", 17);
		write(2, str, ft_strlen(str));
		write(2, ": numeric argument required\n", 29);
	}
	else if (err_type == 1)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": No such file or directory\n", 29);
	}
	else if (err_type == 1271)
	{
		write(2, "minishell: ", 11);
		write(2, str, ft_strlen(str));
		write(2, ": command not found\n", 20);
	}
}