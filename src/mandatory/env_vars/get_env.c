/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrefil <andrefil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:04:48 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/02 23:53:43 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static char	*get_key(char *envp, int *len_key)
{
	char	*key;

	*len_key = 0;
	while (envp[*len_key] != '=')
		(*len_key)++;
	key = ft_strndup(envp, *len_key);
	*len_key += 1;
	return (key);
}

void	get_envp(t_env_var **envp, char **environ)
{
	char	*key;
	char	*value;
	int		index;
	int		len_key;

	index = 0;
	len_key = 0;
	while (environ[index])
	{
		key = get_key(environ[index], &len_key);
		value = ft_strdup(environ[index] + len_key);
		env_lstadd_back(envp, env_lstnew(key, value));
		index++;
		free (key);
		free (value);
	}
}
