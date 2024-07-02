/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:58:40 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/03 12:01:11 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int	parse_pipe(const char *input, int single_q, int double_q)
{
	int	index;
	int	valid;

	index = 0;
	valid = 0;
	while (input[index] != '\0')
	{
		if (input[index] == '\'')
			single_q++;
		else if (input[index] == '"')
			double_q++;
		if (input[index] == '|' && !(single_q % 2) && !(double_q % 2))
		{
			if (valid)
				return (0);
			valid = 1;
		}
		else if (ft_isspace(input[index]))
			valid = 0;
		else if (ft_isalnum(input[index]))
			valid = 0;
		index++;
	}
	return (valid);
}

int	check_pipe(const char *input)
{
	int	double_q;
	int	single_q;
	int	valid;

	double_q = 0;
	single_q = 0;
	valid = 0;
	if (input[0] == '|')
		return (0);
	valid = parse_pipe(input, single_q, double_q);
	if (valid)
		return (0);
	return (1);
}
