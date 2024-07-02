/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_constructor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:22:57 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/06 15:14:34 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>

static t_token	*build_word_token(int *i, char *cmd_line)
{
	t_token	*token;
	int		size;
	int		has_quote;

	size = *i;
	has_quote = FALSE;
	while (cmd_line[size])
	{
		if (cmd_line[size] == '\'' || cmd_line[size] == '\"')
		{
			size++;
			has_quote = !has_quote;
		}
		else if (ft_isspace(cmd_line[size]) == 0)
			size++;
		else if (ft_isspace(cmd_line[size]) == 1 && has_quote == TRUE)
			size++;
		else
			break ;
		if (cmd_line[size] == '|' && ft_isspace(cmd_line[--size]) == 0)
			break ;
	}
	token = token_lstnew(ft_strndup(&cmd_line[*i], size - *i), WORD);
	*i = size;
	return (token);
}

static t_token	*build_expansion_token(int *i, char *cmd_line)
{
	t_token	*token;
	int		size;

	size = *i;
	size++;
	if (ft_isdigit(cmd_line[size]) != 0 || cmd_line[size] == '?')
		size++;
	if (cmd_line[size])
	{
		while (ft_isalpha(cmd_line[size]) != 0 || cmd_line[size] == '_')
			size++;
	}
	token = token_lstnew(ft_strndup(&cmd_line[*i], size - *i), EXPANSION);
	*i = size - 1;
	return (token);
}

static t_token	*build_quote_token(int *i, char *cmd_line)
{
	t_token	*token;
	int		size;
	char	quote;

	size = *i;
	quote = cmd_line[size];
	size++;
	while (cmd_line[size] && cmd_line[size] != quote)
		size++;
	if (!cmd_line[size])
		return (NULL);
	else
		size++;
	create_quote_token(cmd_line, &size);
	token = token_lstnew(ft_strndup(&cmd_line[*i], size - *i), WORD);
	*i = size - 1;
	return (token);
}

static int	build_token_type(t_token **token, int *i, char *cmd)
{
	if (cmd[*i] == '<' || cmd[*i] == '>')
		*token = build_redirect_token(i, cmd);
	else if (cmd[*i] == '\'' || cmd[*i] == '\"')
		*token = build_quote_token(i, cmd);
	else if (cmd[*i] == '|')
		*token = token_lstnew(ft_strndup(&cmd[*i], 1), PIPE);
	else if (cmd[*i] == '$')
		*token = build_expansion_token(&(*i), cmd);
	else if (ft_isspace(cmd[*i]) == 0)
		*token = build_word_token(&(*i), cmd);
	else if (cmd[*i] == '\'' || cmd[*i] == '\"')
		*token = build_quote_token(&(*i), cmd);
	if (*token == NULL)
		return (ERROR);
	else
		return (OK);
}

int	build_token(char *cmd, t_token **token_list)
{
	t_token	*token;
	int		i;

	i = 0;
	while (cmd[i])
	{
		token = NULL;
		if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|' || cmd[i] == '$'
			|| ft_isspace(cmd[i]) == 0 || cmd[i] == '\'' || cmd[i] == '\"')
		{
			build_token_type(&token, &i, cmd);
			if (token == NULL)
				return (ERROR);
		}
		if (token != NULL)
			token_lstadd_back(token_list, token);
		if (cmd[i])
			i++;
	}
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
	return (TRUE);
}
