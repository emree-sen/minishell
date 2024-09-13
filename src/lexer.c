#include "mini.h"

void	ft_free_token_arr(t_token **token_arr)
{
	int i;
	t_token *tmp;

	i = 0;
	while (token_arr[i])
	{
		tmp = token_arr[i];
		free(tmp->str);
		free(tmp);
		i++;
	}
	free(token_arr);
}

void	ft_free_root(t_token *root)
{
	t_token *tmp;

	while (root)
	{
		tmp = root;
		root = root->next;
		// free(tmp->str);
		free(tmp);
	}
}

int main()
{
	t_state state;
	extern char **environ;
	char **env = environ;

	char *line;
	t_token *root;
	t_variables *var_root;
	var_root = dup_veriables(env);
	while (1)
	{
		
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		add_history(line);
		root = str_to_token(line);
		token_extract_all_meta(&root);
		token_split_dollars(&root, var_root, &state);
		token_del_quote(root);
		state.token_arr = token_separate_by_pipe(root);
		token_arr_set_type(state.token_arr);
		// executor
		executor(&state, var_root);
		ft_free_token_arr(state.token_arr);
		ft_free_root(root);
		free(line);
	}
}
