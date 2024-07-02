/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_upgrade.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 22:59:22 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/06 17:29:46 by andrefil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_token	*upgrade_token_list(t_data *data, t_env_var **envp)
{
	t_token	*current;
	int		has_operator;

	current = data->token_list;
	has_operator = FALSE;
	current = upgrade_token_by_type(current, &has_operator, data, envp);
	if (!data->token_list || data->token_list->next == NULL)
		return (current);
	if (has_operator == FALSE)
	{
		current = data->token_list;
		while (current)
		{
			current = upgrade_env_var(current, envp, data);
			if (current == NULL)
				break ;
			current->token = CMD_TOKEN;
			current = current->next;
		}
	}
	current = reorganize_tokens(&(data->token_list));
	return (current);
}

t_token	*upgrade_env_var(t_token *token, t_env_var **envp, t_data *data)
{
	int		i;
	char	*final_line;
	char	*content;
	char	*env_key;
	int		has_single_quote;

	i = 0;
	final_line = NULL;
	content = token->content;
	has_single_quote = FALSE;
	if (content[i] == '\'')
		has_single_quote = TRUE;
	while (content[i])
	{
		if (content[i] == '$' && has_single_quote == FALSE && \
			ft_isspace(content[i + 1]) == 0 && content[i + 1])
		{
			env_key = concat_env_var(&i, content, &final_line, data);
			if (env_key != NULL)
				search_env_key(env_key, &final_line, envp);
		}
		else
			concat_word(&i, content, &final_line, has_single_quote);
	}
	return (rebuild_env_token(token, &final_line, data));
}
