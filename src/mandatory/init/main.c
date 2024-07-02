/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:18:31 by andrefil          #+#    #+#             */
/*   Updated: 2024/06/07 16:00:47 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

volatile int	g_signal;

void	term_properties(int restore)
{
	static struct termios	term;

	if (!restore)
		tcgetattr(STDIN_FILENO, &term);
	else
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static int	validate_input(char *input)
{
	int	i;

	if (!input)
	{
		printf("exit\n");
		return (END);
	}
	if (!ft_strncmp(input, "", 1))
	{
		free (input);
		return (0);
	}
	i = -1;
	while (input[++i])
	{
		if (ft_isspace(input[i]) == 0)
			break ;
	}
	if (input[i] == '\0')
	{
		free (input);
		return (0);
	}
	return (1);
}

static char	*ft_user_in_prompt(t_env_var **envp, char *tag)
{
	t_env_var	*tmp;
	char		*user;

	tmp = env_lstsearch(envp, tag);
	user = NULL;
	if (!tag || !tmp)
		user = ft_strjoin("$", "/minishell: ");
	else
		user = ft_strjoin(tmp->value, "/minishell: ");
	tmp = NULL;
	return (user);
}

static int	get_input(t_env_var **envp, int status)
{
	char		*input;
	char		*in_user;
	int			valid;

	in_user = ft_user_in_prompt(envp, "USER");
	term_properties(0);
	input = readline(in_user);
	free(in_user);
	valid = validate_input(input);
	if (valid != TRUE)
		return (valid);
	add_history(input);
	if (!syntax_checker(input))
	{
		free (input);
		ft_putendl_fd("Error in syntax.", 2);
		return (2);
	}
	return (start_minishell(envp, input, status));
}

int	main(void)
{
	t_env_var	*envp;
	int			status;

	init_signals();
	envp = NULL;
	status = 0;
	get_envp(&envp, __environ);
	while (1)
	{
		g_signal = 0;
		status = get_input(&envp, status);
		if (status == 131)
			printf("Quit (core dumped)\n");
		else if ((status == 2 || status == 0) && g_signal == SIGINT)
			printf("\n");
		if (status == 131 || (status == 2 && g_signal == SIGINT))
			term_properties(1);
		if (status == END)
			break ;
	}
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	free_envp(&envp);
	rl_clear_history();
	return (status);
}
