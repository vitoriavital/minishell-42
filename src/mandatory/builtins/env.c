/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 15:46:27 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 15:45:24 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*var_join(char *key, char *value, int status)
{
	char	*temp;
	char	*var;

	temp = NULL;
	if (value && status)
	{
		temp = ft_strjoin(key, "=");
		var = ft_strjoin(temp, "\"");
		free (temp);
		temp = ft_strjoin(var, value);
		free (var);
		var = ft_strjoin(temp, "\"");
	}
	else if (value && !status)
	{
		temp = ft_strjoin(key, "=");
		var = ft_strjoin(temp, value);
	}
	else
		return (ft_strdup(key));
	free (temp);
	return (var);
}

int	ft_env(t_env_var **envp, int status)
{
	char		*p_var;
	t_env_var	*temp;

	if (!*envp)
		return (1);
	temp = *envp;
	p_var = NULL;
	while (temp)
	{
		if (temp->value || (!temp->value && status == TRUE))
		{
			if (status == TRUE)
				ft_putstr_fd("declare -x ", 1);
			p_var = var_join(temp->key, temp->value, status);
			ft_putendl_fd(p_var, 1);
			free(p_var);
		}
		temp = temp->next;
	}
	return (0);
}

int	ft_unset(t_env_var **head, char *key)
{
	t_env_var	*del_node;
	size_t		len;

	if (!*head || !head || !key)
		return (0);
	len = ft_strlen(key);
	if (ft_strncmp((*head)->key, key, len) == 0)
	{
		del_node = *head;
		*head = (*head)->next;
		free (del_node->value);
		free (del_node->key);
		free(del_node);
		return (0);
	}
	else
		return (ft_unset(&((*head)->next), key));
}
