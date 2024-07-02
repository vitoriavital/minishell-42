/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:51:11 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/03 12:42:11 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

t_ast	*ast_lstnew(char *content, t_tokens token_type)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->content = content;
	node->ast_type = type_check(token_type);
	node->left = NULL;
	node->right = NULL;
	node->left_token = NULL;
	node->right_token = NULL;
	return (node);
}

void	ast_lstclear(t_ast **lst)
{
	t_ast	*current;
	t_ast	*right;

	if (*lst == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		right = current->right;
		if (current->left != NULL)
			ast_lstclear(&(current->left));
		if (current->content)
			free(current->content);
		free(current);
		current = right;
	}
	*lst = NULL;
}

void	ast_lstadd_left(t_ast **lst, t_ast *new)
{
	t_ast	*last;

	last = NULL;
	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->left != NULL)
		last = last->left;
	last->left = new;
	last->left_token = NULL;
}

void	ast_lstadd_right(t_ast **lst, t_ast *new)
{
	t_ast	*last;

	last = NULL;
	if (new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->right != NULL)
		last = last->right;
	last->right = new;
	last->right_token = NULL;
}

void	build_ast_node(t_token *new_node, t_ast **ast_list, int direction)
{
	t_ast	*new;

	new = ast_lstnew(ft_strdup(new_node->content), new_node->token);
	if (direction == LEFT)
		ast_lstadd_left(ast_list, new);
	else
		ast_lstadd_right(ast_list, new);
	free (new_node->content);
	new_node->content = NULL;
	new_node->token = NONE;
	return ;
}
