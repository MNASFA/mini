/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/10 11:55:54 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*prepare_commands(char *input, t_env *env)
{
	t_token	*tokens;
	t_token	*current;
	char	*expanded_value;
	t_cmd	*cmds;

	if (is_pipe_at_start(input) || !check_two_pipes(input))
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		return (NULL);
	}
	input = handle_pipe_end(input);
	if (!input)
		return (NULL);
	
	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	
	tokens = tokenizer(input);
	if (!tokens)
		return (NULL);
	
	// Expand environment variables
	current = tokens;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_variables(current->value, env);
			if (expanded_value)
			{
				free(current->value);
				current->value = expanded_value;
			}
		}
		current = current->next;
	}
	
	cmds = split_by_pipe(tokens);
	remove_pipe_node(cmds);
	
	free_token(tokens);
	return(cmds);
}
