#include "mini.h"

int main()
{
	while (1)
	{
		t_state state;
		state.status = 0;
		extern char **environ;
		char **env = environ;

		char *line;
		t_token *root;
		t_variables *var_root;
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		add_history(line);
		var_root = dup_veriables(env);

		// if(ft_strlen(line) > 0)
		// 	check_the_syntax(line);
		var_root = dup_veriables(env);

		root = str_to_token(line);

		token_extract_all_meta(&root);
		token_split_dollars(&root, var_root, &state);
		token_del_quote(root);
		state.token_arr = token_separate_by_pipe(root);
		token_arr_set_type(state.token_arr);
		//token_list_printer(root);
		// executor
		executor(&state, var_root);
		free(line);

	}
}
