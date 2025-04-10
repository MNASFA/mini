/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:04:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/10 14:55:37 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void print_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    while (current)
    {
        t_token *token = current->token;
        while (token)
        {
            printf("%s ==> %d    ", token->value, token->type);
            token = token->next;
        }
        printf("\n");
        current = current->next;
    }
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;
	while (cmd)
	{
		free_token(cmd->token);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;

	char	*input;
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	t_env	*env;
	t_token	*token;

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

		cmds = prepare_commands(input, env);
		if (!cmds)
		{
			free(input);
			continue;
		}
		
		int i = 1;
		current_cmd = cmds;
		while (current_cmd)
		{
			printf("Command %d:\n", i++);
			token = current_cmd->token;
			while (token)
			{
				printf("	token:%s	type:%d\n", token->value, token->type);
				token = token->next;
			}
			current_cmd = current_cmd->next;
		}
		
		free_cmd_list(cmds);
		free(input);
	}
	return (0);
}
