/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:54:21 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/02 23:30:11 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

t_env_var	*env_lstnew(char *key, char *value)
{
	t_env_var	*node;

	node = (t_env_var *)malloc(sizeof(t_env_var));
	if (node == NULL)
		return (NULL);
	if (key)
		node->key = ft_strdup(key);
	else
		node->key = NULL;
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	env_lstadd_back(t_env_var **lst, t_env_var *new)
{
	t_env_var	*current;

	if (*lst == NULL && new != NULL)
	{
		*lst = new;
		return ;
	}
	if (new != NULL)
	{
		current = *lst;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void	env_lstclear(t_env_var **lst)
{
	t_env_var	*current;
	t_env_var	*next;

	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		free (current->value);
		free (current->key);
		free (current);
			current = next;
	}
	*lst = NULL;
}

t_env_var	*env_lstsearch(t_env_var **lst, char *key)
{
	t_env_var	*current;
	int			size;

	size = ft_strlen(key) + 1;
	current = *lst;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, size) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	env_size(t_env_var **env)
{
	t_env_var	*temp;
	int			len;

	if (!env || !*env)
		return (0);
	len = 0;
	temp = *env;
	while (temp)
	{
		len++;
		temp = temp->next;
	}
	return (len);
}
