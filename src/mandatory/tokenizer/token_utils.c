/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:36:44 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/03 11:38:24 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_token	*create_redirect_token(int *i, char *cmd_line, char c)
{
	t_token	*token;

	if (cmd_line[(*i) + 1] == c)
	{
		if (c == '<')
			token = token_lstnew(ft_strndup(&cmd_line[(*i)++], 2), DLESS);
		else
			token = token_lstnew(ft_strndup(&cmd_line[(*i)++], 2), DGREAT);
	}
	else
	{
		if (c == '<')
			token = token_lstnew(ft_strndup(&cmd_line[*i], 1), LESS);
		else
			token = token_lstnew(ft_strndup(&cmd_line[*i], 1), GREAT);
	}
	return (token);
}

void	create_quote_token(char *cmd_line, int *size)
{
	int	inside_quote;

	inside_quote = FALSE;
	while (cmd_line[*size])
	{
		if (ft_isspace(cmd_line[*size]) == 0)
		{
			if (cmd_line[*size] == '\'' || cmd_line[*size] == '\"')
				inside_quote = !inside_quote;
			(*size)++;
		}
		else if (inside_quote == TRUE && ft_isspace(cmd_line[*size]) == 1)
			(*size)++;
		else
			break ;
	}
}

t_token	*find_last_cmd(t_token *current)
{
	while (current->next)
	{
		if (current->next->token == CMD_TOKEN)
			current = current->next;
		else
			return (current);
	}
	return (current);
}

void	relink_tokens(t_token *target, t_token *first_cmd, t_token *last_cmd,
				t_token *input)
{
	t_token	*operator;

	if (target->token == CMD_TOKEN)
	{
		operator = target->next;
		target->next = first_cmd;
		first_cmd->prev = target;
		last_cmd->next = operator;
		if (operator != NULL)
			operator->prev = last_cmd;
		input->prev = target;
	}
	else
	{
		operator = target;
		if (target->prev != NULL)
			target->prev->next = first_cmd;
		first_cmd->prev = target->prev;
		last_cmd->next = operator;
		input->prev = target->prev;
		if (operator != NULL)
			operator->prev = last_cmd;
	}
}

t_token	*build_redirect_token(int *i, char *cmd_line)
{
	char	c;

	if (cmd_line[*i] == '<' || cmd_line[*i] == '>')
	{
		c = cmd_line[*i];
		if (cmd_line[(*i) + 1] == c && cmd_line[(*i) + 2] == c)
		{
			ft_putstr_fd("Syntax error near unexpected token \'", 1);
			ft_putchar_fd(c, 1);
			ft_putendl_fd("\'", 1);
			return (NULL);
		}
		return (create_redirect_token(i, cmd_line, c));
	}
	return (NULL);
}
