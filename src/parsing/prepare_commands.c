/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:44:14 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/12 11:29:11 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int count_args(t_token *tokens)
{
	int count;
	t_token	*current;
	t_token	*prev;
	
	count = 0;
	current = tokens;
	prev = NULL;

	while (current)
	{
		if (current->type == WORD)
		{
			if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT && prev->type != APPEND && prev->type != HEREDOC && prev->type != REDIR_INOUT))
				count++;
		}
		prev = current;
		current = current->next;
	}
	return (count);
}

t_exec	*parse_command(t_cmd *cmd)
{
	t_exec *exec;
	t_token	*current;
	t_token	*prev;
	int		i;
	int		arg_count;

	exec = malloc(sizeof(t_exec));
	current = cmd->token;
	prev = NULL;
	i = 0;
	arg_count = count_args(current);


	exec->args = malloc(sizeof(char *) * (arg_count + 1));
	exec->infile = NULL;
	exec->outfile = NULL;
	exec->append = 0;
	exec->heredoc = 0;
	exec->delimiter = NULL;
	exec->cmd = NULL;

	while (current)
	{

		if(current->type == WORD)
		{
			if (!prev || (prev->type != REDIR_IN && prev->type != REDIR_OUT && prev->type != APPEND && prev->type != HEREDOC && prev->type != REDIR_INOUT))
			{
				exec->args[i++] = ft_strdup(current->value);
				if (!exec->cmd)
					exec->cmd = exec->args[0];
			}
		}
		else if (current->type == REDIR_IN && current->next)
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
		else if (current->type == REDIR_INOUT && current->next)
		{
			exec->infile = ft_strdup(current->next->value);
			exec->outfile = ft_strdup(current->next->value);
		}
		else if (current->type == HEREDOC && current->next)
		{
			exec->delimiter = ft_strdup(current->next->value);
			exec->heredoc = 1;
		}
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
	input = handle_pipe_end(input);
	if (!input)
		return (NULL);
	
	input = check_unclosed_quotes(input);
	if (!input)
		return (NULL);
	
	tokens = tokenizer(input);
	if (!tokens)
		return (NULL);

	if (check_redirection_err(tokens) == 1)
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
t_exec	**build_exec_list(char *input, t_env *env)
{
	t_cmd	*cmds;
	t_cmd	*tmp;
	t_exec	**exec_list;
	int		count;
	int 	i;
	
	cmds = prepare_commands(input, env);
	tmp = cmds;
	count = 0;
	i = 0;
	
	if (!cmds)
		return (NULL);
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	
	exec_list = malloc(sizeof(t_exec *) * (count + 1));
	if (!exec_list)
	{
		free_cmd_list(cmds);
		return (NULL);
	}
	
	// Parse each cmd !!
	tmp = cmds; 
    i = 0;
	while (tmp)
	{
		exec_list[i++] = parse_command(tmp);
		tmp = tmp->next;
	}
	exec_list[i] = NULL;
	
	free_cmd_list(cmds);
	return (exec_list);
}
