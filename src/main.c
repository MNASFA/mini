/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/12 10:00:39 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void print_cmd_list(t_cmd *cmd_list)
// {
//     t_cmd *current = cmd_list;
//     while (current)
//     {
//         t_token *token = current->token;
//         while (token)
//         {
//             printf("%s ==> %d    ", token->value, token->type);
//             token = token->next;
//         }
//         printf("\n");
//         current = current->next;
//     }
// }

void	print_exec_list(t_exec **execs)
{
	int i = 0;
	
	if (!execs)
		return;
	
	while (execs[i])
	{
		printf("Command %d:\n", i + 1);
		
		printf("	cmd	: %s\n", execs[i]->cmd);
		
		printf("	args 	: ");
		int j = 0;
		while (execs[i]->args && execs[i]->args[j])
		{
			printf("%s ", execs[i]->args[j]);
			j++;
		}
		printf("\n");

		if (execs[i]->infile)
			printf("	infile : %s\n", execs[i]->infile);
		
		if (execs[i]->outfile)
			printf("	outfile	: %s (append: %d)\n", execs[i]->outfile , execs[i]->append);
		
		if (execs[i]->heredoc)
			printf("	heredoc	: << %s\n", execs[i]->delimiter);
		i++;
	}
}

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;

	char	*input;
	t_env	*env;

	env = init_env(envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		
		if (*input)
			add_history(input);

		t_exec **execs = build_exec_list(input, env);
		print_exec_list(execs);
		
		free_exec_array(execs);
		free(input);
	}
	return (0);
}
