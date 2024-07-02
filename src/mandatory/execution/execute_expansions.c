/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expansions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:36:28 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 15:46:37 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

static void	rebuild_quote(char *content, char **full_line, char *remove_quotes,
		int i)
{
	char	*temp;
	char	*old_line;
	int		start;

	start = ++i;
	while (content[i])
	{
		if (ft_isspace(content[i]) == 0 && content[i] != '\''
			&& content[i] != '\"')
			i++;
		else
			break ;
	}
	temp = ft_strndup(&(content[start]), i - start);
	*full_line = ft_strjoin(remove_quotes, temp);
	free(temp);
	free(remove_quotes);
	if (content[i] == '\'' || content[i] == '\"')
	{
		old_line = *full_line;
		temp = get_cmds_args_quote(&(content[i]));
		*full_line = ft_strjoin(*full_line, temp);
		free(temp);
		free(old_line);
	}
}

char	*get_cmds_args_quote(char *content)
{
	char	*remove_quotes;
	int		i;
	char	*full_line;
	char	quote;

	if (ft_strncmp(content, "\"\"", 2) == 0)
		return (NULL);
	remove_quotes = NULL;
	quote = '\0';
	i = 0;
	quote = content[i++];
	while (content[i])
	{
		if (content[i] != quote)
			i++;
		else
			break ;
	}
	remove_quotes = ft_strndup(&(content[1]), i - 1);
	if (content[i + 1] == '\0')
		return (remove_quotes);
	rebuild_quote(content, &full_line, remove_quotes, i);
	return (full_line);
}

static char	*get_cmds_args_sentence(char *content)
{
	int		i;
	char	*line;

	i = 0;
	while (content[i])
	{
		if (content[i] != '\'' && content[i] != '\"')
			i++;
		else
			break ;
	}
	line = ft_strndup(&(content[0]), i);
	return (line);
}

static void	rebuild_word(char **full_line, char *temp, int *i, int has_quote)
{
	char	*line;

	if (*full_line == NULL)
		*full_line = ft_strdup(temp);
	else if (temp != NULL)
	{
		line = *full_line;
		*full_line = ft_strjoin(line, temp);
		free(line);
	}
	if (temp == NULL)
		*i += 2;
	else
	{
		*i += (ft_strlen(temp) + has_quote);
		free(temp);
	}
}

char	*get_cmds_args_word(char *content)
{
	int		i;
	char	*full_line;
	char	*temp;
	int		has_quote;

	i = 0;
	full_line = NULL;
	while (content[i])
	{
		has_quote = 0;
		if (content[i] == '\'' || content[i] == '\"')
		{
			temp = get_cmds_args_quote(&(content[i]));
			has_quote = 2;
		}
		else
			temp = get_cmds_args_sentence(&(content[i]));
		rebuild_word(&full_line, temp, &i, has_quote);
	}
	return (full_line);
}
