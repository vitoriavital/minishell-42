/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_constructor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:51:11 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/06 19:51:52 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static void	division_args_cmds(t_token **token_list, t_ast **ast_list)
{
	t_token	*current_token;
	t_ast	*current_ast;

	current_token = token_lstlast(*token_list);
	current_ast = *ast_list;
	if (current_ast == NULL)
	{
		build_ast_node(current_token, ast_list, LEFT);
		current_token = current_token->prev;
		current_ast = *ast_list;
	}
	while (current_token)
	{
		build_ast_node(current_token, &current_ast, LEFT);
		current_token = current_token->prev;
		current_ast = current_ast->left;
	}
}

static void	start_ast(t_token **token_list, t_tokens operator, t_ast **ast_list)
{
	t_token	*current;

	while (operator > 5)
	{
		current = token_lstlast(*token_list);
		while (current != NULL)
		{
			if (current->token == operator
				|| (operator == 6 && current->token > 2))
			{
				break_in_two(current, ast_list, 0);
				return ;
			}
			else
				current = current->prev;
		}
		operator--;
	}
	return ;
}

static t_token	*validate_token_list(t_token **token_list, t_ast **ast_list)
{
	t_token	*current;

	if (!*token_list || !(*token_list)->content)
		return (NULL);
	current = token_lstlast(*token_list);
	if (current->prev == NULL)
	{
		build_ast_node(current, ast_list, LEFT);
		return (NULL);
	}
	return (current);
}

void	scan_token_list(t_token **token_list, t_ast **ast_list)
{
	t_token		*current;
	int			operator;

	operator = PIPE;
	current = validate_token_list(token_list, ast_list);
	if (current == NULL)
		return ;
	start_ast(token_list, operator, ast_list);
	if (*ast_list != NULL)
	{
		if ((*ast_list)->ast_type != PIPE_LINE)
			continue_division_left(6, *ast_list, ast_list, FALSE);
		else
		{
			while (operator > 5)
			{
				continue_division_left(operator, *ast_list, ast_list, FALSE);
				continue_division_right(operator, *ast_list, ast_list, FALSE);
				operator--;
			}
		}
	}
	else if (*ast_list == NULL)
		division_args_cmds(token_list, ast_list);
}
