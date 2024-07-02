/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_token_upgrade.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:07:48 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 15:54:31 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static void	expand_status_env_var(char *env_key, char **final_line, \
									t_data *data)
{
	char	*temp;
	char	*status;

	status = ft_itoa(data->last_status);
	temp = ft_strjoin(*final_line, status);
	free (*final_line);
	free (status);
	free (env_key);
	*final_line = temp;
	return ;
}

void	search_env_key(char *env_key, char **final_line, t_env_var **envp)
{
	t_env_var	*env;
	char		*temp;

	temp = NULL;
	env = env_lstsearch(envp, env_key);
	free (env_key);
	if (env != NULL)
	{
		temp = ft_strjoin(*final_line, env->value);
		free (*final_line);
		*final_line = temp;
	}
	else
	{
		if (*final_line)
			free (*final_line);
		*final_line = NULL;
	}
}

char	*concat_env_var(int *i, char *content, char **final_line, t_data *data)
{
	int			start;
	char		*env_key;

	start = *i;
	(*i)++;
	data->has_env = TRUE;
	while (content[*i])
	{
		if (ft_isspace(content[*i]) == 0 && content[*i] != '$' && \
			content[*i] != '\'' && content[*i] != '\"')
		{
			if (content[++(*i) - 1] == '?')
				break ;
		}
		else
			break ;
	}
	env_key = ft_strndup(&content[start + 1], (*i) - start - 1);
	if (env_key[0] == '?')
	{
		expand_status_env_var(env_key, final_line, data);
		return (NULL);
	}
	return (env_key);
}

void	concat_word(int *i, char *content, char **final_line, int has_s_quote)
{
	char	*line;
	char	*temp;
	int		start;

	start = *i;
	while (content[*i])
	{
		if (content[*i] == '$' && has_s_quote == FALSE && \
			content[(*i) + 1] && ft_isspace(content[(*i) + 1]) == 0 \
			&& content[(*i) + 1] != '\'' && content[(*i) + 1] != '\"')
			break ;
		(*i)++;
	}
	line = ft_strndup(&(content[start]), (*i) - start);
	temp = ft_strjoin(*final_line, line);
	free (*final_line);
	free (line);
	*final_line = temp;
}

t_token	*rebuild_env_token(t_token *token, char **final_line, t_data *data)
{
	t_token	*next;

	free (token->content);
	if (*final_line != NULL)
	{
		token->content = *final_line;
		return (token);
	}
	else
	{
		next = token->next;
		if (token->prev)
			token->prev->next = token->next;
		if (token->next)
			token->next->prev = token->prev;
		if (next)
			data->token_list = next;
		else
			data->token_list = token->prev;
		free (token);
		return (next);
	}
}
