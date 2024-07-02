/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_upgrade_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:07:52 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/03 11:42:01 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	upgrade_args_cmds(t_token *current, int direction)
{
	if (direction == RIGHT)
	{
		while (current->next)
		{
			if (current->next->token == WORD)
				current->next->token = CMD_TOKEN;
			else
				break ;
			current = current->next;
		}
	}
	else
	{
		while (current->prev)
		{
			if (current->prev->token == WORD)
				current->prev->token = CMD_TOKEN;
			else
				break ;
			current = current->prev;
		}
	}
	return (TRUE);
}

static int	upgrade_operator(t_token *current)
{
	char	*new_content;

	if ((current->token >= 3 && current->token <= 5) && current->next)
	{
		if (current->next->token == WORD)
		{
			current->next->token = FILE_TOKEN;
			new_content = remove_quotes_expansion(current->next->content);
			free(current->next->content);
			current->next->content = new_content;
		}
	}
	else if (current->token == FILE_TOKEN && current->next)
	{
		if (current->next->token == WORD)
			upgrade_args_cmds(current, RIGHT);
	}
	if ((current->token >= 3 && current->token <= 5) && current->prev)
	{
		if (current->prev->token == WORD)
			upgrade_args_cmds(current, LEFT);
	}
	return (TRUE);
}

static int	upgrade_pipe(t_token *current)
{
	if (current->token == PIPE && current->next)
	{
		if (current->next->token == WORD)
			upgrade_args_cmds(current, RIGHT);
	}
	if (current->token == PIPE && current->prev)
	{
		if (current->prev->token == WORD)
			upgrade_args_cmds(current, LEFT);
	}
	return (TRUE);
}

static int	upgrade_dless(t_token *current)
{
	if (current->token == DLESS && current->next)
	{
		if (current->next->token == WORD)
			current->next->token = DELIMITER_TOKEN;
	}
	else if (current->token == DELIMITER_TOKEN && current->next)
	{
		if (current->next->token == WORD)
			upgrade_args_cmds(current, RIGHT);
	}
	if (current->token == DLESS && current->prev)
	{
		if (current->prev->token == WORD)
			upgrade_args_cmds(current, LEFT);
	}
	return (TRUE);
}

t_token	*upgrade_token_by_type(t_token *current, int *has_operator,
				t_data *data, t_env_var **envp)
{
	if (current->next == NULL)
	{
		current->token = CMD_TOKEN;
		current = upgrade_env_var(current, envp, data);
		return (reorganize_tokens(&(data->token_list)));
	}
	while (current->next)
	{
		if ((current->token >= 3 && current->token <= 5) && current->next)
			*has_operator = upgrade_operator(current);
		else if ((current->token == DLESS || current->token == DELIMITER_TOKEN)
			&& current->next)
			*has_operator = upgrade_dless(current);
		else if (current->token == PIPE && current->next)
			*has_operator = upgrade_pipe(current);
		if (((current->token >= 3 && current->token <= 5)
				|| current->token == FILE_TOKEN) && current->prev)
			*has_operator = upgrade_operator(current);
		else if (current->token == DLESS && current->prev)
			*has_operator = upgrade_dless(current);
		else if (current->token == PIPE && current->prev)
			*has_operator = upgrade_pipe(current);
		current = current->next;
	}
	return (current);
}
