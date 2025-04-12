/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 09:58:22 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/12 10:48:35 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void free_exec_array(t_exec **execs)
{
	int i = 0;
	int j;

	if (!execs)
		return;

	while (execs[i])
	{
		// Free args array
		if (execs[i]->args)
		{
			j = 0;
			while (execs[i]->args[j])
			{
				free(execs[i]->args[j]);
				j++;
			}
			free(execs[i]->args);
		}
		// Free other members
		if (execs[i]->infile)
			free(execs[i]->infile);
		if (execs[i]->outfile)
			free(execs[i]->outfile);
		if (execs[i]->delimiter)
			free(execs[i]->delimiter);

		// Free the struct itself
		free(execs[i]);
		i++;
	}
	free(execs);
}
