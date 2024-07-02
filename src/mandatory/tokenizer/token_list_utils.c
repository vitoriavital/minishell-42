/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 13:08:31 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 18:20:27 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_token	*token_lstnew(char *content, t_tokens type)
{
	t_token	*node;

	node = (t_token *)malloc(sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->content = content;
	node->token = type;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	token_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (*lst == NULL && new != NULL)
	{
		*lst = new;
		return ;
	}
	if (new != NULL)
	{
		current = *lst;
		if (current != NULL)
		{
			while (current->next != NULL)
				current = current->next;
			current->next = new;
			new->prev = current;
		}
		else
			*lst = new;
	}
}

t_token	*token_lstlast(t_token *lst)
{
	if (lst)
	{
		while (lst->next && lst->next->token != NONE)
			lst = lst->next;
		return (lst);
	}
	return (NULL);
}

void	token_lstclear(t_token **lst)
{
	t_token	*current;
	t_token	*next;

	if (*lst == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	*lst = NULL;
}
