/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnasfa <hmnasfa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:23:31 by hmnasfa           #+#    #+#             */
/*   Updated: 2025/03/03 01:03:53 by hmnasfa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// initialize the environment

t_env	*create_env_var(char *env_start)
{
	t_env	*new_var;
	char	*equal;
	
	equal = ft_strchr(env_start, '=');
	if (!equal)
		return (NULL);	
	
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
		
	new_var->key = ft_strndup(env_start, equal - env_start);
	new_var->value = ft_strdup(equal + 1);
	new_var->next = NULL;
	return (new_var);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_var;
	int		i;
		
	head = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_var = create_env_var(envp[i]);
		if (!head)
			head = new_var;
		else 
			current->next = new_var;
		current = new_var;
		i++;
	}
	return (head);
}

char	*get_env_value(t_env *env, char *key)
{
	if (!key || *key == '\0')
		return ("");
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

int	extract_var_name(char *str, int i, char *var_name)
{
	int	j;
	
	j = 0;
	if (str[i] == '$')
		i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		var_name[j] = str[i];
		i++;
		j++;
	}
	var_name[j] = '\0';
	return (i);
}

size_t expanded_length(char *str, t_env *env)
{
	size_t	lenght;
	int		i;
	char	var_name[100];

	i = 0;
	lenght = 0;
	
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i = extract_var_name(str, i, var_name);
			lenght += ft_strlen(get_env_value(env, var_name));
		}
		else
		{
			lenght++;
			i++;
		}
	}
	return (lenght);
}

char	*expand_variables(char	*str, t_env *env)
{
	size_t	expanded_len;
	char	*result;
	int		i;
	int		j;
	char	var_name[100];
	char	*value;

	expanded_len = expanded_length(str, env);
	result = malloc(expanded_len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if(str[i] == '$' && str[i + 1])
		{
			i = extract_var_name(str, i, var_name);
			value = get_env_value(env, var_name);
			ft_strcpy(&result[j], value);
			j += ft_strlen(value);
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
