/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:52:42 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/04/11 16:06:29 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
	WORD, //(COMMANDS, ARGUMENTS, FILENAMES)
	PIPE, // "|"
	REDIR_IN, // "<"
	REDIR_OUT, // ">"
	HEREDOC, // "<<"
	APPEND // ">>"
}	t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type type;
	struct s_token *next;
}	t_token;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct  s_env	*next;
}	t_env;

typedef struct s_cmd
{
	t_token	*token;
	struct s_cmd *next;
}	t_cmd;

// Structure for the execution phase

typedef struct  s_exec
{
	char	**args;	// ["ls" , "-l" , NULL]
	char	*infile; // for '< input.txt'
	char	*outfile;	// for '> output.txt' or '>>'
	int		append;	// 0 for '>' , 1 for '>>'
	int 	heredoc; // 1 if it's a herdoc
	char	*delimiter;
	char	*cmd; // original command name 
}	t_exec;


void	*ft_memcpy(void *dest, const void *src, size_t n);
char	**ft_split(char *input);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strdup(const char *str);
char	*ft_strndup(const char *str, size_t n);
char	*ft_strchr(const char *s, int c);
int		ft_isalnum(int c);
char    *ft_strcpy(char *s1, char *s2);
size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
t_token	*tokenizer(char *input);
t_token *create_token(char *content);
t_env	*init_env(char **envp);

char	*expand_variables(char	*str, t_env *env);
t_cmd	*split_by_pipe(t_token *tokens);
void	remove_pipe_node(t_cmd	*cmd_list);
char	*check_unclosed_quotes(char *input);
int check_two_pipes(char *input);
int		check_redirection_err(t_token *tokens);
char *handle_pipe_end(char *input);
int	is_pipe_at_start(char *input);

void	free_token(t_token *tokens);

t_cmd	*prepare_commands(char *input, t_env *env);
void	free_cmd_list(t_cmd *cmd);
t_exec	**build_exec_list(char *input, t_env *env);

#endif
