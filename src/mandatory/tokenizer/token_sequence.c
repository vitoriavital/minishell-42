/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:43 by mavitori          #+#    #+#             */
/*   Updated: 2024/05/30 15:30:24 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static t_token	*swap_tokens(t_token *current, t_token **token_list)
{
	t_token	*operator;
	t_token	*data;
	t_token	*first_cmd;
	t_token	*last_cmd;
	t_token	*next_last_cmd;

	operator = current->prev;
	data = current;
	first_cmd = current->next;
	last_cmd = find_last_cmd(current);
	next_last_cmd = last_cmd->next;
	first_cmd->prev = operator->prev;
	last_cmd->next = operator;
	if (operator->prev)
		operator->prev->next = first_cmd;
	operator->prev = last_cmd;
	operator->next = data;
	data->prev = operator;
	data->next = next_last_cmd;
	if (next_last_cmd && next_last_cmd->prev)
			next_last_cmd->prev = data;
	if (operator == *token_list)
		return (first_cmd);
	else
		return (NULL);
}

static void	swap_multi(t_token *target, t_token *input)
{
	t_token	*first_cmd;
	t_token	*last_cmd;

	if (target == NULL || input == NULL)
		return ;
	first_cmd = input;
	last_cmd = find_last_cmd(input);
	if (first_cmd->prev != NULL)
		first_cmd->prev->next = last_cmd->next;
	if (last_cmd->next != NULL)
		last_cmd->next->prev = first_cmd->prev;
	relink_tokens(target, first_cmd, last_cmd, input);
}

static void	reorganize_multi_redirects(t_token *start, t_token *end)
{
	while (start)
	{
		if (start->token != CMD_TOKEN)
		{
			if (start->prev)
				start = start->prev;
			break ;
		}
		else
			start = start->next;
	}
	while (end && end != start)
	{
		if (end->token == CMD_TOKEN)
			swap_multi(start, end);
		end = end->prev;
	}
}

static t_token	*multiple_redirect_cmds(t_token *token_root)
{
	t_token	*start;
	t_token	*end;
	t_token	*last_end;

	end = token_root;
	while (end->next)
	{
		if (end->next->token == PIPE)
			break ;
		else
			end = end->next;
	}
	last_end = end;
	start = token_root;
	reorganize_multi_redirects(start, end);
	if (last_end->token == PIPE && last_end->next != NULL)
		multiple_redirect_cmds(last_end->next);
	while (token_root->prev != NULL)
		token_root = token_root->prev;
	return (token_root);
}

t_token	*reorganize_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*initial;

	current = *token_list;
	initial = NULL;
	if (!current || (current->next == NULL && current->content == NULL))
		return (NULL);
	else if (current->next == NULL)
		return (*token_list);
	while (current)
	{
		if ((current->token == DELIMITER_TOKEN || current->token == FILE_TOKEN)
			&& current->next)
		{
			if (current->next->token == CMD_TOKEN)
				initial = swap_tokens(current, token_list);
		}
		current = current->next;
	}
	if (initial != NULL)
		return (multiple_redirect_cmds(initial));
	else
		return (multiple_redirect_cmds(*token_list));
}
