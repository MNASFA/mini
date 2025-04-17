/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:58:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/17 09:47:35 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_split(char	**split)
{
	int i;
	
	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_token(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;

		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
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

void	free_exec_list(t_exec *exec_list)
{
	t_exec	*current;
	t_exec	*next;

	current = exec_list;
	while (current)
	{
		next = current->next;
		if (current->args)
			free_split(current->args);
		free(current->infile);
		free(current->outfile);
		free(current->delimiter);
		free(current->cmd);

		free(current);
		current = next;
	}
}
