/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin_executing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:53:23 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/07 18:22:38 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

t_ast	*find_cmd(t_ast **root, int *count)
{
	t_ast	*tmp;
	t_ast	*cmd;

	if (!(*root) || !root)
		return (NULL);
	tmp = *root;
	cmd = NULL;
	(*count) = 0;
	while (tmp && tmp->ast_type != COMMAND)
	{
		(*count)++;
		tmp = tmp->left;
	}
	if (!tmp)
		return (NULL);
	cmd = tmp;
	tmp = NULL;
	return (cmd);
}

int	exec_all_ast(t_ast *node, t_data *data, t_ast *cmd, int *fd)
{
	int	status;
	int	fork_cmd;

	status = consume_redir_in_ast(node, &cmd);
	if (cmd && status != KO && data->has_att == FALSE)
	{
		fork_cmd = fork();
		if (fork_cmd == 0)
		{
			status = execute_command(cmd, data, &data->envp, LEFT);
			free_data(&data);
			close_fd_fork(fd, NULL);
			close (STDIN_FILENO);
			close (STDOUT_FILENO);
			exit (status);
		}
		waitpid(fork_cmd, &status, 0);
		status = finish_pipe(&status, data);
	}
	else if (cmd && status != KO)
		status = execute_command(cmd, data, &data->envp, LEFT);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close_fd_fork(fd, NULL);
	return (status);
}

int	execute_all_command(t_data *data, t_ast *node)
{
	int		status;
	char	**cmd_args;

	status = 0;
	if (node->left && node->left->ast_type == COMMAND)
		data->direction = LEFT;
	else
		data->direction = RIGHT;
	cmd_args = check_exit_execute(node, data->direction);
	if (cmd_args != NULL)
		return (terminate_program(data, &data->envp, cmd_args));
	status = execute_command(node, data, &data->envp, data->direction);
	return (status);
}

int	execute_ast(t_ast *node, t_data *data)
{
	t_ast	*cmd;
	int		fd[2];

	if (node == NULL)
		return (EMPTY);
	cmd = NULL;
	if (node->ast_type == 18)
		return (execute_pipe(node, data));
	else if (node->ast_type >= 14 && node->ast_type < 18)
	{
		fd[0] = dup(STDIN_FILENO);
		fd[1] = dup(STDOUT_FILENO);
		return (exec_all_ast(node, data, cmd, fd));
	}
	else
		return (execute_all_command(data, node));
}

int	begin_executing(t_data *data, t_env_var **envp)
{
	t_ast	*root;
	int		status;

	status = 0;
	if (data->ast_list == NULL || data->ast_list == NULL)
		return (EMPTY);
	root = data->ast_list;
	status = search_here_doc(root, data, envp);
	if (status != 0)
	{
		if (status != 130)
		{
			ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
			ft_putstr_fd("near unexpected token `newline'", STDERR_FILENO);
		}
		return (status);
	}
	data->has_att = FALSE;
	data->direction = LEFT;
	if (root->right == NULL && (root->left == NULL || root->left != NULL))
		status = execute_single_multi_cmd(root, data, envp);
	else
		status = execute_ast(root, data);
	return (status);
}
