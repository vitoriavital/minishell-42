/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 20:02:56 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/06 16:40:17 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static void	insert_sorted(t_env_var **sorted, t_env_var *new_node)
{
	t_env_var	*current;

	if (*sorted == NULL || ft_strncmp((*sorted)->key, new_node->key, \
		ft_strlen((*sorted)->key)) >= 0)
	{
		new_node->next = *sorted;
		*sorted = new_node;
	}
	else
	{
		current = *sorted;
		while (current->next && ft_strncmp(current->next->key, new_node->key, \
			ft_strlen(current->next->key)) < 0)
			current = current->next;
		new_node->next = current->next;
		current->next = new_node;
	}
}

static void	sort_env(t_env_var **envp)
{
	t_env_var	*sorted;
	t_env_var	*current;
	t_env_var	*next;

	current = *envp;
	sorted = NULL;
	while (current)
	{
		next = current->next;
		insert_sorted(&sorted, current);
		current = next;
	}
	*envp = sorted;
}

static t_env_var	*copy_env(t_env_var *origin)
{
	t_env_var	*current;
	char		*key;
	char		*value;
	t_env_var	*envp;

	current = origin;
	envp = NULL;
	while (current)
	{
		key = ft_strdup(current->key);
		if (current->value)
		{
			value = ft_strdup(current->value);
			env_lstadd_back(&envp, env_lstnew(key, value));
			free (value);
		}
		else
			env_lstadd_back(&envp, env_lstnew(key, NULL));
		free (key);
		current = current->next;
	}
	return (envp);
}

int	print_env_order(t_env_var *env)
{
	t_env_var	*copy;

	copy = copy_env(env);
	sort_env(&copy);
	ft_env(&copy, TRUE);
	free_envp(&copy);
	return (0);
}
