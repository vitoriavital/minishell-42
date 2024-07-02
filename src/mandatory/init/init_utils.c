/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 15:59:24 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 17:56:56 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

static t_data	*initialize_data(int status)
{
	t_data	*data;

	data = NULL;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->token_list = NULL;
	data->ast_list = NULL;
	data->envp = NULL;
	data->last_status = status;
	data->has_env = FALSE;
	return (data);
}

static int	build_data_storage(t_data *data, t_env_var **envp, char *input)
{
	if (build_token(input, &data->token_list) == ERROR)
		return (0);
	data->token_list = upgrade_token_list(data, envp);
	if (data->token_list == NULL)
	{
		free_data(&data);
		return (0);
	}
	scan_token_list(&data->token_list, &data->ast_list);
	if (!data->ast_list)
		return (0);
	token_lstclear(&data->token_list);
	data->envp = *envp;
	return (1);
}

int	start_minishell(t_env_var **envp, char *input, int status)
{
	t_data	*data;

	data = initialize_data(status);
	signal(SIGINT, handle_new_line);
	if (!build_data_storage(data, envp, input))
		return (0);
	status = begin_executing(data, envp);
	signal(SIGINT, handle_new_line);
	unlink_here_doc(data->ast_list);
	if (data)
		free_data(&data);
	return (status);
}
