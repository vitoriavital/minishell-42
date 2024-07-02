/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrefil <andrefil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 22:20:45 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/02 23:50:01 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	call_pwd(char *input)
{
	char	*pwd;

	if (ft_strncmp(input, "pwd", 4))
		return (0);
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (1);
}

static void	chand_env_pwd(t_env_var **envp)
{
	t_env_var	*temp;
	t_env_var	*pwd;

	temp = env_lstsearch(envp, ("OLDPWD"));
	pwd = env_lstsearch(envp, ("PWD"));
	free(temp->value);
	temp->value = ft_strdup(pwd->value);
	temp = NULL;
	free(pwd->value);
	pwd->value = getcwd(NULL, 0);
	pwd = NULL;
}

static int	valid_cd(t_env_var **envp, char *input)
{
	t_env_var	*path;
	int			status;

	path = env_lstsearch(envp, "HOME");
	if (input)
		status = chdir(input);
	else
		status = chdir(path->value);
	if (status == -1)
	{
		path = NULL;
		perror("cd");
		return (1);
	}
	path = NULL;
	chand_env_pwd(envp);
	return (0);
}

int	change_dir(char **input, t_env_var **envp)
{
	if (ft_strncmp(input[0], "cd", 3))
		return (-1);
	if (!input[1])
		return (valid_cd(envp, NULL));
	else if (input[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (valid_cd(envp, input[1]));
}

int	ft_clear(char *input)
{
	if (ft_strncmp(input, "clear", 6) == 0)
	{
		ft_putendl_fd("\e[2J\e[H", 1);
		return (1);
	}
	return (0);
}
