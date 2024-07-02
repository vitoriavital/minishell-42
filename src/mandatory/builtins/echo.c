/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 19:02:26 by andrefil          #+#    #+#             */
/*   Updated: 2024/07/02 18:30:31 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static int	echo_env(char *content, t_data *data)
{
	char	**split;
	int		status;

	split = ft_split(content, ' ');
	if (split[1])
		status = ft_echo(split, data);
	else
		status = 0;
	free_matrix(split);
	return (status);
}

static int	check_multi_n(char *str)
{
	int	i;

	if (str[0] != '-')
		return (1);
	i = 1;
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (0);
	return (1);
}

int	ft_echo(char **arg, t_data *data)
{
	int	index;

	if (ft_strncmp(arg[0], "echo", 5) == 0 && arg[1] != NULL)
	{
		if (check_multi_n(arg[1]) == 0)
			index = 2;
		else
			index = 1;
		while (arg[index])
		{
			if (ft_strncmp(arg[index], "\"\"", 3) == 0)
				index++;
			else if (arg[index] != NULL)
				ft_putstr_fd(arg[index++], 1);
			if (arg[index] != NULL)
				ft_putchar_fd(' ', 1);
		}
		if (ft_strncmp(arg[1], "-n", 3) != 0)
			ft_putchar_fd('\n', 1);
		return (1);
	}
	else if (data->has_env == TRUE)
		return (echo_env(arg[0], data));
	return (0);
}
