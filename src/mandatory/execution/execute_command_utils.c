/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:39:09 by mavitori          #+#    #+#             */
/*   Updated: 2024/06/03 13:56:06 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void	validate_content(char **cmd_args, int i, t_ast *current)
{
	if ((current->content)[0] == '\'' || (current->content)[0] == '\"')
		cmd_args[i] = get_cmds_args_quote(current->content);
	else
		cmd_args[i] = get_cmds_args_word(current->content);
}

static char	**get_cmd_args_left(char **cmd_args, int size, t_ast *node)
{
	t_ast	*current;

	current = node;
	while (current->left)
	{
		if (current->ast_type == COMMAND)
			size++;
		else
			return (NULL);
		current = current->left;
	}
	cmd_args = ft_calloc(sizeof(char *), (size + 1));
	current = node;
	size--;
	while (size >= 0)
	{
		validate_content(cmd_args, size, current);
		current = current->left;
		size--;
	}
	return (cmd_args);
}

static char	**get_cmd_args_right(char **cmd_args, int size, t_ast *node)
{
	t_ast	*current;
	int		i;

	current = node;
	i = 0;
	while (current->right)
	{
		if (current->ast_type == COMMAND)
			size++;
		else
			return (NULL);
		current = current->right;
	}
	cmd_args = ft_calloc(sizeof(char *), (size + 1));
	current = node;
	size--;
	while (i <= size)
	{
		validate_content(cmd_args, i, current);
		current = current->right;
		i++;
	}
	return (cmd_args);
}

char	**get_cmd_args(t_ast *node, int direction)
{
	char	**cmd_args;
	int		size;

	size = 1;
	cmd_args = NULL;
	if (direction == LEFT)
		cmd_args = get_cmd_args_left(cmd_args, size, node);
	else
		cmd_args = get_cmd_args_right(cmd_args, size, node);
	return (cmd_args);
}

char	**get_path(t_env_var **envp, char *var, char *cmd)
{
	t_env_var	*envp_temp;
	char		**full_path;

	if (!*envp)
		return (NULL);
	envp_temp = env_lstsearch(envp, var);
	if (!envp_temp)
	{
		full_path = (char **)malloc(sizeof(char *) * (2));
		full_path[0] = ft_strdup(cmd);
		full_path[1] = NULL;
	}
	else
		full_path = ft_split(envp_temp->value, ':');
	return (full_path);
}
