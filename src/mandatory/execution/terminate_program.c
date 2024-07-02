/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate_program.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:57:36 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/06 19:31:52 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

static int	terminate_program_status(char *input)
{
	int		i;
	char	*temp;
	char	*final;

	i = 0;
	if (input[0] == '-')
		return (156);
	else if (input[0] == '+')
		i++;
	while (input[i])
	{
		if (!ft_isdigit(input[i]))
		{
			temp = ft_strjoin("minishell: exit: ", input);
			final = ft_strjoin(temp, ": numeric argument required");
			free(temp);
			ft_putendl_fd(final, STDERR_FILENO);
			free(final);
			return (2);
		}
		i++;
	}
	return (ft_atoi(input));
}

void	terminate_program_error(char **all_args)
{
	ft_putendl_fd("exit", 1);
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	free_matrix(all_args);
	return ;
}

int	terminate_program(t_data *data, t_env_var **envp, char **cmd_args)
{
	int	status;

	status = 0;
	if (ft_strncmp(cmd_args[0], "exit", 5) == 0 && cmd_args[1] == NULL)
		status = 0;
	else if (ft_strncmp(cmd_args[0], "exit", 5) == 0 && cmd_args[1] != NULL)
		status = terminate_program_status(cmd_args[1]);
	if (ft_strncmp(cmd_args[0], "exit", 5) == 0 && cmd_args[1] != NULL
		&& cmd_args[2] != NULL && status != 2)
	{
		terminate_program_error(cmd_args);
		return (1);
	}
	free_matrix(cmd_args);
	ft_putendl_fd("exit", 1);
	if (data->has_att == FALSE)
	{
		free_envp(envp);
		rl_clear_history();
		free_data(&data);
		exit(status);
	}
	return (status);
}

int	check_exit(char *input)
{
	int	i;

	i = 0;
	while (ft_isspace(input[i]) == 1)
		i++;
	if (ft_strncmp(&(input[i]), "exit", 4) == 0)
	{
		while (input[i + 4])
		{
			if (ft_isalnum(input[i + 4]) == 0 && ft_isspace(input[i + 4]) == 0)
				return (KO);
			i++;
		}
		return (OK);
	}
	else
		return (KO);
}

char	**check_exit_execute(t_ast *node, int direction)
{
	char	**cmd_args;

	if (node->ast_type != COMMAND)
		node = node->right;
	cmd_args = get_cmd_args(node, direction);
	if (ft_strncmp(cmd_args[0], "exit", 5))
	{
		free_matrix(cmd_args);
		return (NULL);
	}
	return (cmd_args);
}
