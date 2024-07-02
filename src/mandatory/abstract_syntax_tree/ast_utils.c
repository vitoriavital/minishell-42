/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 23:57:55 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/03 12:40:19 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>

t_type_ast	type_check(t_tokens token)
{
	t_type_ast	ast_type;

	if (token == WORD)
		ast_type = STRING;
	else if (token == CMD_TOKEN)
		ast_type = COMMAND;
	else if (token == FILE_TOKEN)
		ast_type = FILENAME;
	else if (token == DELIMITER_TOKEN)
		ast_type = DELIMITER;
	else
		ast_type = token + 11;
	return (ast_type);
}

void	check_multi_cmd_right(t_ast *node, t_ast *current_ast_node)
{
	t_token	*current;
	t_token	*token;
	int		has_operator;

	current = node->right_token;
	has_operator = TRUE;
	token = node->right_token;
	while (current->next != NULL && current->content != NULL)
	{
		if (current->token != CMD_TOKEN)
			break ;
		current = current->next;
	}
	if (current->next == NULL || current->content == NULL)
		has_operator = FALSE;
	if (has_operator == FALSE)
	{
		while (token && token->content != NULL)
		{
			build_ast_node(token, &current_ast_node, RIGHT);
			token = token->next;
		}
	}
}

void	check_multi_cmd_left(t_ast *node, t_ast *current_ast_node)
{
	t_token	*current;
	t_token	*token;
	int		has_operator;

	current = node->left_token;
	has_operator = TRUE;
	token = node->left_token;
	while (current->prev != NULL && current->content != NULL)
	{
		if (current->token != CMD_TOKEN)
			break ;
		current = current->prev;
	}
	if (current->prev == NULL || current->content == NULL)
		has_operator = FALSE;
	if (has_operator == FALSE)
	{
		while (token && token->content != NULL)
		{
			build_ast_node(token, &current_ast_node, LEFT);
			token = token->prev;
		}
	}
}
