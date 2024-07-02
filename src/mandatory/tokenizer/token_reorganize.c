/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reorganize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:35:50 by mavitori          #+#    #+#             */
/*   Updated: 2024/07/02 18:50:54 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	fix_references(t_token *current, t_token *first_input,
		t_token *last_input)
{
	t_token	*last_output;
	t_token	*first_output;

	first_output = current->next;
	last_output = first_output->next;
	while (last_output->next)
	{
		if (last_output->next->token == FILE_TOKEN
			|| last_output->next->token == GREAT
			|| last_output->next->token == DGREAT)
			last_output = last_output->next;
		else
			break ;
	}
	first_output->prev = last_input;
	last_output->next = last_input->next;
	if (last_input->next && last_input->next->prev)
		last_input->next->prev = last_output;
	first_input->prev = current;
	last_input->next = first_output;
	current->next = first_input;
}

static void	fix_redirect_order(t_token *first_input)
{
	t_token	*current;
	t_token	*last_input;

	current = first_input->next;
	while (current->next)
	{
		if (current->next->token == FILE_TOKEN || current->next->token == DLESS
			|| current->next->token == LESS)
			current = current->next;
		else
			break ;
	}
	last_input = current;
	current = first_input;
	while (current)
	{
		if (current->token == CMD_TOKEN)
			fix_references(current, first_input, last_input);
		current = current->prev;
	}
}

t_token	*reorganize_redirect_tokens(t_token *root)
{
	int		has_output_redirect;
	int		in_order;
	t_token	*current;

	has_output_redirect = FALSE;
	in_order = TRUE;
	current = root;
	while (current && in_order == TRUE)
	{
		if (current->token == LESS || current->token == DLESS)
		{
			if (has_output_redirect == TRUE)
				in_order = FALSE;
		}
		else if (current->token == GREAT || current->token == DGREAT)
			has_output_redirect = TRUE;
		current = current->next;
	}
	if (in_order == FALSE)
		fix_redirect_order(current->prev);
	return (root);
}

int	parse_token_constructor(char *cmd_line, int *size, int has_quote)
{
	int	i;

	i = *size;
	if (ft_isspace(cmd_line[i]) == 0)
	{
		if (cmd_line[i] == '>' || cmd_line[i] == '<' || (cmd_line[i] == '>'
				&& cmd_line[i + 1] == '>')
			|| (cmd_line[i] == '<' && cmd_line[i + 1] == '<'))
			return (-1);
		else
			(*size)++;
	}
	else if (ft_isspace(cmd_line[i]) == 1 && has_quote == TRUE)
		(*size)++;
	else
		i = -1;
	return (i);
}
