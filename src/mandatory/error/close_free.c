/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 04:41:49 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/06 13:45:39 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	close_fd_fork(int *fd, char *error)
{
	if (error)
		perror(error);
	if (*fd)
	{
		close(fd[0]);
		if (fd[1])
			close(fd[1]);
	}
}

void	free_matrix(char **matrix)
{
	int	index;

	if (!matrix)
		return ;
	index = 0;
	while (matrix[index])
	{
		if (matrix[index])
			free(matrix[index]);
		index++;
	}
	free(matrix);
}

void	free_data(t_data **data)
{
	if ((*data)->ast_list)
		ast_lstclear(&(*data)->ast_list);
	if ((*data)->token_list)
		token_lstclear(&(*data)->token_list);
	(*data)->ast_list = NULL;
	(*data)->envp = NULL;
	(*data)->token_list = NULL;
	if (*data)
		free(*data);
	*data = NULL;
}

void	free_envp(t_env_var **envp)
{
	t_env_var	*current;
	t_env_var	*next;

	if (!*envp)
		return ;
	current = *envp;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*envp = NULL;
}

void	free_all_exec_args(char **path_args, char **args)
{
	if (path_args)
		free_matrix(path_args);
	if (args)
		free_matrix(args);
}
