#include "mini.h"

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
		free(line);

	}
}
