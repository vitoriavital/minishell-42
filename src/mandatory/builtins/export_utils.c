/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrefil <andrefil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 20:05:38 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/06 19:23:55 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	has_signal(char *str, char s, int status)
{
	int		index;

	index = 0;
	while (str[index])
		if (str[index++] == s)
			return (status + 1);
	return (status);
}

static void	has_var(t_env_var *check, char **split_var, int has_equal)
{
	if (check->value)
		free(check->value);
	if (!split_var[1] && has_equal == TRUE)
		check->value = ft_strdup("\"\"");
	else if (split_var[1] == NULL && has_equal == FALSE)
		check->value = NULL;
	else
		check->value = ft_strdup(split_var[1]);
	free_matrix(split_var);
	check = NULL;
}

static void	add_var(t_env_var *temp, char **split_var, int has_equal)
{
	if (split_var[1] == NULL && has_equal == FALSE)
		env_lstadd_back(&temp, env_lstnew(split_var[0], NULL));
	else if (split_var[1] == NULL && has_equal == TRUE)
		env_lstadd_back(&temp, env_lstnew(split_var[0], "\"\""));
	else
		env_lstadd_back(&temp, env_lstnew(split_var[0], split_var[1]));
	free_matrix(split_var);
}

static char	**split_var_env(char *input, int has_equal)
{
	char	**var;
	int		index;

	index = 0;
	var = ft_calloc(sizeof(char *), 3);
	if (!var)
		return (NULL);
	if (has_equal)
	{
		while (input[index++] != '=')
			;
		var[0] = ft_strndup(input, index - 1);
		var[1] = ft_strdup(input + index);
		return (var);
	}
	var[0] = ft_strndup(input, ft_strlen(input));
	var[1] = NULL;
	return (var);
}

void	ft_export_new_var(char *var, t_env_var **envp)
{
	t_env_var	*temp;
	t_env_var	*check;
	char		**split_var;
	int			has_equal;

	check = NULL;
	split_var = NULL;
	if (!var || !*envp)
		return ;
	has_equal = has_signal(var, '=', FALSE);
	temp = *envp;
	split_var = split_var_env(var, has_equal);
	check = env_lstsearch(&temp, split_var[0]);
	if (!check)
		add_var(temp, split_var, has_equal);
	else
		has_var(check, split_var, has_equal);
}
