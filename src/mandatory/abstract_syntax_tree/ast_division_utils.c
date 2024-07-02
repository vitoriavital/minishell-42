/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_division_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:06:54 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 18:19:34 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	add_ast_node(t_token *current, t_ast *current_ast_node)
{
	if (current->prev != NULL)
	{
		if (current->prev->prev == NULL || current->prev->prev->content == NULL)
			build_ast_node(current->prev, &current_ast_node, LEFT);
		else
			current_ast_node->left_token = current->prev;
		if (current_ast_node->left_token)
			check_multi_cmd_left(current_ast_node, current_ast_node);
	}
	if (current->next != NULL)
	{
		if (current->next->next == NULL || current->next->next->content == NULL)
			build_ast_node(current->next, &current_ast_node, RIGHT);
		else
			current_ast_node->right_token = current->next;
		if (current_ast_node->right_token)
			check_multi_cmd_right(current_ast_node, current_ast_node);
	}
}

t_ast	*break_in_two(t_token *current, t_ast **ast_list, int direction)
{
	t_ast	*current_ast_node;

	current_ast_node = ast_lstnew(ft_strdup(current->content), current->token);
	if (direction == RIGHT)
		ast_lstadd_right(ast_list, current_ast_node);
	else
		ast_lstadd_left(ast_list, current_ast_node);
	free(current->content);
	current->content = NULL;
	current->token = NONE;
	add_ast_node(current, current_ast_node);
	return (current_ast_node);
}

void	continue_division(t_tokens operator, t_ast **ast_list,
		t_ast *ast_current_node, int direction)
{
	t_ast	*node_ast;
	t_token	*current;

	node_ast = NULL;
	if (direction == RIGHT)
		current = token_lstlast(ast_current_node->right_token);
	else
		current = ast_current_node->left_token;
	while (current != NULL && current->content != NULL)
	{
		if (current->token == operator)
		{
			node_ast = break_in_two(current, &ast_current_node, direction);
			continue_division_left(operator, node_ast, ast_list, TRUE);
			continue_division_right(operator, node_ast, ast_list, TRUE);
		}
		else if (operator == 6 && current->token > 2)
		{
			node_ast = break_in_two(current, &ast_current_node, direction);
			continue_division_left(operator, node_ast, ast_list, TRUE);
		}
		else
			current = current->prev;
	}
}

void	continue_division_left(t_tokens operator, t_ast *ast_current_node,
		t_ast **ast_list, int status)
{
	if (status == FALSE)
	{
		while (ast_current_node && ast_current_node->left != NULL)
			ast_current_node = ast_current_node->left;
	}
	if (ast_current_node->left != NULL || ast_current_node->left_token == NULL)
		return ;
	else if (ast_current_node->left_token)
	{
		if (ast_current_node->left_token->content == NULL)
			return ;
	}
	continue_division(operator, ast_list, ast_current_node, LEFT);
	if (ast_current_node->right_token)
		continue_division(operator, ast_list, ast_current_node, RIGHT);
}

void	continue_division_right(t_tokens operator, t_ast *ast_current_node,
		t_ast **ast_list, int status)
{
	if (status == FALSE)
	{
		while (ast_current_node && ast_current_node->right != NULL)
			ast_current_node = ast_current_node->right;
	}
	if (ast_current_node->right != NULL
		|| ast_current_node->right_token == NULL)
		return ;
	else if (ast_current_node->right_token)
	{
		if (ast_current_node->right_token->content == NULL)
			return ;
	}
	continue_division(operator, ast_list, ast_current_node, RIGHT);
	if (ast_current_node->left_token)
		continue_division(operator, ast_list, ast_current_node, LEFT);
}
