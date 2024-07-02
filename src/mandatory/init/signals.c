/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:18:31 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 15:59:21 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>

int	inside_heredoc(int inside_heredoc)
{
	static int	heredoc;

	if (inside_heredoc != -1)
		heredoc = inside_heredoc;
	return (heredoc);
}

void	handle_new_line(int signum)
{
	(void)signum;
	g_signal = signum;
	if (inside_heredoc(-1) == TRUE)
	{
		printf("\n");
		close (STDIN_FILENO);
	}
	else
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0;
	}
}

void	handle_signal(int signum)
{
	(void)signum;
	g_signal = signum;
}

void	init_signals(void)
{
	signal(SIGINT, handle_new_line);
	signal(SIGQUIT, SIG_IGN);
}
