/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrefil <andrefil@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 19:25:43 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 00:10:18 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int	open_redir_input(t_ast *operator)
{
	t_ast	*tmp;
	int		file_fd;

	tmp = operator->right;
	file_fd = open(tmp->content, O_RDONLY);
	if (file_fd == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	if (dup2(file_fd, STDIN_FILENO) == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	close(file_fd);
	tmp = NULL;
	return (OK);
}

int	open_redir_output(t_ast *operator)
{
	t_ast	*tmp;
	int		file_fd;

	tmp = operator->right;
	file_fd = open(tmp->content, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (file_fd == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	if (dup2(file_fd, STDOUT_FILENO) == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	close(file_fd);
	tmp = NULL;
	return (OK);
}

int	open_redir_append(t_ast *operator)
{
	t_ast	*tmp;
	int		file_fd;

	tmp = operator->right;
	file_fd = open(tmp->content, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (file_fd == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	if (dup2(file_fd, STDOUT_FILENO) == -1)
	{
		close_fd_fork(&file_fd, "minishell");
		return (KO);
	}
	close(file_fd);
	tmp = NULL;
	return (OK);
}

static int	execute_operator(t_ast *operator)
{
	if (operator->ast_type == INPUT)
		return (open_redir_input(operator));
	else if (operator->ast_type == OUTPUT)
		return (open_redir_output(operator));
	else if (operator->ast_type == APPEND)
		return (open_redir_append(operator));
	else
		return (EMPTY);
}

int	consume_redir_in_ast(t_ast *node, t_ast **cmd)
{
	t_ast	*tmp;
	int		count;
	int		index;
	int		status;

	tmp = node;
	*cmd = find_cmd(&tmp, &count);
	if (!cmd && node->right->ast_type == DELIMITER)
		status = 0;
	else
	{
		while (count > 0)
		{
			index = count;
			while (tmp->left && index-- > 1)
				tmp = tmp->left;
			count--;
			status = execute_operator(tmp);
			if (status == KO)
				break ;
			tmp = node;
		}
	}
	return (status);
}
