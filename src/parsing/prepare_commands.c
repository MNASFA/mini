/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/17 13:20:12 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int count_args(t_token *tokens)
{
	int count;
	t_token	*current;
	t_token	*prev;
	int		cmd_found;
	
	cmd_found = 0;
	count = 0;
	current = tokens;
	prev = NULL;

	while (current)
	{
		if (current->type == WORD)
		{
			if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT 
				&& prev->type != APPEND && prev->type != HEREDOC))
			{
				if (!cmd_found)
					cmd_found = 1;
				else
					count++;		
			}
		}
		prev = current;
		current = current->next;
	}
	return (count);
}

static t_exec *init_exec(int arg_count)
{
	t_exec *exec;
	
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (NULL);
	
	exec->args = malloc(sizeof(char *) * (arg_count + 1));
	exec->infile = NULL;
	exec->outfile = NULL;
	exec->append = 0;
	exec->heredoc = 0;
	exec->delimiter = NULL;
	exec->cmd = NULL;

	return (exec);
}

static void	handle_redirections(t_exec *exec, t_token *current)
{
	if (current->type == REDIR_IN && current->next)
		exec->infile = ft_strdup(current->next->value);
	else if (current->type == REDIR_OUT && current->next)
	{
		exec->outfile = ft_strdup(current->next->value);
		exec->append = 0;
	}
	else if	(current->type == APPEND && current->next)
	{
		exec->outfile = ft_strdup(current->next->value);
		exec->append = 1;
	}
	else if (current->type == HEREDOC && current->next)
	{
		exec->delimiter = ft_strdup(current->next->value);
		exec->heredoc = 1;
	}
}

static void	handle_word(t_exec *exec, t_token *current, t_token *prev
	, int *i, int *cmd_found)
{
	if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT && prev->type != APPEND 
		&& prev->type != HEREDOC))
	{
		if (!(*cmd_found))
		{
			exec->cmd = ft_strdup(current->value);
			*cmd_found = 1;
		}
		else
		{
			exec->args[*i] = ft_strdup(current->value);
			(*i)++;
		}
	}
}

t_exec	*parse_command(t_cmd *cmd, int i, int cmd_found)
{
	t_exec *exec;
	t_token	*current;
	t_token	*prev;
	int		arg_count;

	current = cmd->token;
	prev = NULL;
	arg_count = count_args(current);
	exec = init_exec(arg_count);
	if (!exec)
		return (NULL);
	while (current)
	{
		if(current->type == WORD)
			handle_word(exec, current, prev, &i, &cmd_found);
		else
			handle_redirections(exec, current);
		prev = current;
		current = current->next;
	}
	exec->args[i] = NULL;
	return (exec);
}
 
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
	
	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	
	tokens = tokenizer(input);
	if (!tokens)
		return (NULL);
	
	if (check_redirection_err(tokens) == 1)
		return (NULL);
	
	input = handle_pipe_end(input);
	if (!input)
		return (NULL);
	
	// Expand environment variables
	current = tokens;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_variables(current->value, env, 0, 0, 0, 0);
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
t_exec	*build_exec_list(char *input, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*tmp;
	t_exec	*exec_list;
	t_exec	*current;
	t_exec	*new_node; // TEMPORARY POINTER FOR NEW NODES
	
	current = NULL;
	exec_list = NULL;
	new_node = NULL;
	cmds = prepare_commands(input, env);
	if (!cmds)
		return (NULL);
	
	tmp = cmds;
	while (tmp)
	{
		//Create a new t_exec node
		new_node = parse_command(tmp, 0, 0);
		if (!new_node)
		{
			free_cmd_list(cmds);
			free_exec_list(exec_list);
			return (NULL);
		}

		// Link the new node to the list
		if (!exec_list)
		{
			exec_list = new_node;
			current = exec_list;
		}
		else
		{
			current->next = new_node;
			current = new_node;
		}
		tmp = tmp->next;
	}
	free_cmd_list(cmds);
	return (exec_list);
}
