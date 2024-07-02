/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:57:52 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 15:46:54 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

static int	child_process(t_ast *node, t_data *data, t_env_var **envp)
{
	int	status;

	status = 0;
	status = execute_command(node, data, envp, LEFT);
	ast_lstclear(&node);
	data->envp = NULL;
	free(data);
	data = NULL;
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	return (status);
}

static int	parent_process(t_data *data, pid_t pid, int status)
{
	data->envp = NULL;
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

static int	check_builtins(t_ast *node, t_data *data, t_env_var **envp)
{
	char	**cmd_args;
	int		status;

	status = 0;
	cmd_args = check_exit_execute(node, LEFT);
	if (cmd_args != NULL)
		return (terminate_program(data, envp, cmd_args));
	cmd_args = get_cmd_args(node, LEFT);
	status = call_builtins(cmd_args, data, envp);
	if (status != -1)
	{
		free_matrix(cmd_args);
		return (status);
	}
	free_matrix(cmd_args);
	return (status);
}

int	execute_single_multi_cmd(t_ast *node, t_data *data, t_env_var **envp)
{
	pid_t	pid;
	int		status;

	status = check_builtins(node, data, envp);
	if (status != -1)
		return (status);
	status = 0;
	signal(SIGINT, handle_signal);
	pid = fork();
	if (pid == -1)
	{
		perror("Failed to fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		exit(child_process(node, data, envp));
	else
		return (parent_process(data, pid, status));
}
