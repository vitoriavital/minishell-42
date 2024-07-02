/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:25:19 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 15:44:48 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>

static int	validate_export_var(char *str)
{
	int	i;
	int	after_equal;

	i = 0;
	after_equal = FALSE;
	if (ft_isalpha(str[0]) == 0 && str[0] != '_')
		return (FALSE);
	while (str[i])
	{
		if ((ft_isalnum(str[i]) != 0 || str[i] == '_') && after_equal == FALSE)
			i++;
		else if (str[i] == '=')
		{
			i++;
			after_equal = TRUE;
		}
		else if (after_equal == TRUE)
			i++;
		else
			return (FALSE);
	}
	return (TRUE);
}

static int	ft_export(char **arg, t_env_var **envp, t_env_var *env)
{
	int		i;
	int		has_error;

	i = 1;
	has_error = 0;
	if (!arg[1])
		return (print_env_order(env));
	while (arg[i])
	{
		if (validate_export_var(arg[i]) == TRUE)
			ft_export_new_var(arg[i++], envp);
		else
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(arg[i++], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			has_error++;
		}
	}
	if (has_error != 0)
		return (1);
	return (0);
}

int	call_builtins(char **arg, t_data *data, t_env_var **envp)
{
	int			status;

	status = change_dir(arg, envp);
	if (status != -1)
		return (status);
	if (ft_clear(arg[0]) || ft_echo(arg, data) || call_pwd(arg[0]))
		return (0);
	else if (ft_strncmp(arg[0], "env", 4) == 0)
	{
		if (arg[1] == NULL)
			return (ft_env(envp, FALSE));
		printf("env: ‘%s’: No such file or directory\n", arg[1]);
		return (127);
	}
	else if (ft_strncmp(arg[0], "unset", 6) == 0)
	{
		if (arg[1])
			return (ft_unset(envp, arg[1]));
		else
			return (0);
	}
	else if (ft_strncmp(arg[0], "export", 7) == 0)
		return (ft_export(arg, envp, data->envp));
	return (-1);
}
