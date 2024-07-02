/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:42:18 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/03 15:33:16 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>

static void	join_content(char *content, int i, int start, char **final_line)
{
	char	*temp;
	char	*line;

	line = ft_strndup(&(content[start]), i - start);
	temp = ft_strjoin(*final_line, line);
	free(*final_line);
	free(line);
	*final_line = temp;
}

static void	manage_quotes(int *inside_quote, int *quote, int *i, char *content)
{
	if (*inside_quote == FALSE)
	{
		*quote = content[(*i)++];
		*inside_quote = TRUE;
	}
	else
	{
		*inside_quote = FALSE;
		(*i)++;
	}
}

static int	manage_content(int inside_quote, int quote, int *i, char *content)
{
	int	start;

	if (inside_quote == TRUE)
	{
		start = *i;
		while (content[*i] && content[*i] != quote)
			(*i)++;
	}
	else
	{
		start = *i;
		while (content[*i] && content[*i] != '\'' && content[*i] != '\"')
			(*i)++;
	}
	return (start);
}

char	*remove_quotes_expansion(char *content)
{
	int		i;
	char	*final_line;
	int		inside_quote;
	int		quote;
	int		start;

	i = 0;
	final_line = NULL;
	inside_quote = FALSE;
	quote = '\0';
	while (content[i])
	{
		if (((content[i] == '\'' || content[i] == '\"')
				&& inside_quote == FALSE) || (inside_quote == TRUE
				&& content[i] == quote))
			manage_quotes(&inside_quote, &quote, &i, content);
		else if ((inside_quote == TRUE && content[i] != quote)
			|| (inside_quote == FALSE && content[i] != '\''
				&& content[i] != '\"'))
		{
			start = manage_content(inside_quote, quote, &i, content);
			join_content(content, i, start, &final_line);
		}
	}
	return (final_line);
}
