/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:39:33 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 18:22:22 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>
#include <signal.h>

static void	exec_child_one(t_data *data, t_ast *node)
{
	int	status;

	status = 0;
	if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close_fd_fork(data->pipe_fd, "pipe");
		exit(EXIT_FAILURE);
	}
	close(data->pipe_fd[1]);
	close(data->pipe_fd[0]);
	rl_clear_history();
	data->has_att = TRUE;
	data->direction = LEFT;
	status = execute_ast(node->left, data);
	ast_lstclear(&data->ast_list);
	env_lstclear(&data->envp);
	free (data);
	close (STDOUT_FILENO);
	close (STDIN_FILENO);
	if (status == KO)
		exit (EXIT_FAILURE);
	exit(status);
}

static void	exec_child_two(t_data *data, t_ast *node)
{
	int	status;

	status = 0;
	if (dup2(data->pipe_fd[0], STDIN_FILENO) == -1)
	{
		close_fd_fork(data->pipe_fd, "pipe");
		exit(EXIT_FAILURE);
	}
	close_fd_fork(data->pipe_fd, NULL);
	rl_clear_history();
	data->has_att = TRUE;
	data->direction = RIGHT;
	status = execute_ast(node->right, data);
	ast_lstclear(&data->ast_list);
	env_lstclear(&data->envp);
	free (data);
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	if (status == KO)
		exit (EXIT_FAILURE);
	exit(status);
}

int	finish_pipe(int *status, t_data *data)
{
	if (data->ast_list)
		ast_lstclear(&data->ast_list);
	if (data->token_list)
		token_lstclear(&data->token_list);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	return (*status);
}

int	execute_pipe(t_ast *node, t_data *data)
{
	int			status;
	char		**cmd_args;

	status = 0;
	signal(SIGINT, handle_signal);
	cmd_args = NULL;
	if (pipe(data->pipe_fd) == 0)
	{
		data->fork[0] = fork();
		if (data->fork[0] == 0)
			exec_child_one(data, node);
		data->fork[1] = fork();
		if (data->fork[1] == 0)
			exec_child_two(data, node);
		close_fd_fork(data->pipe_fd, NULL);
		waitpid(data->fork[0], &status, 0);
		waitpid(data->fork[1], &status, 0);
	}
	return (finish_pipe(&status, data));
}
