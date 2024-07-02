# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/24 14:25:11 by andrefil          #+#    #+#              #
#    Updated: 2024/06/07 19:20:42 by andrefil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	minishell

INCLUDES	:=	includes/ \
				libs/libft/

CC			:=	cc
CFLAGS		:=	-Wall -Wextra -Werror -g3
CPP_FLAGS	:=	$(addprefix -I, $(INCLUDES))

LIBS		:=	libs/libft/libft.a -lreadline

OBJS_DIR	:=	objs/

SRC_FILES	+=	$(addprefix src/mandatory/init/, \
				main.c \
				signals.c \
				str_utils.c \
				init_utils.c)

SRC_FILES	+=	$(addprefix src/mandatory/builtins/, \
				dir.c \
				env.c \
				echo.c \
				bultins_utils.c \
				export.c \
				export_utils.c)

SRC_FILES	+=	$(addprefix src/mandatory/env_vars/, \
				env_vars_utils.c \
				get_env.c)

SRC_FILES	+=	$(addprefix src/mandatory/error/, \
				close_free.c)

SRC_FILES	+=	$(addprefix src/mandatory/abstract_syntax_tree/, \
				ast_constructor.c \
				ast_utils.c \
				ast_list_utils.c \
				ast_division_utils.c )

SRC_FILES	+=	$(addprefix src/mandatory/tokenizer/, \
				token_constructor.c \
				token_utils.c \
				token_upgrade.c \
				token_upgrade_utils.c \
				token_sequence.c \
				token_list_utils.c \
				env_token_upgrade.c \
				token_reorganize.c )

SRC_FILES	+=	$(addprefix src/mandatory/execution/, \
				begin_executing.c \
				execute_single_cmd.c \
				execute_command.c \
				execute_expansion_utils.c \
				execute_command_utils.c \
				terminate_program.c \
				execute_pipe.c \
				execute_expansions.c \
				execute_here_doc.c \
				here_doc_utils.c \
				execute_redirects.c \
				execute_command_errors.c )

SRC_FILES	+=	$(addprefix src/mandatory/parser/, \
				syntax_checker.c \
				syntax_checker_utils.c)

OBJS		:=	$(SRC_FILES:%.c=$(OBJS_DIR)%.o)

all: LIBFT $(NAME)
	@echo "\033[0;32mMinishell compiled\033[0m"

LIBFT:
	@$(MAKE) -C libs/libft

$(OBJS_DIR)%.o: %.c
	mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(CPP_FLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) clean -C libs/libft
	@echo "\033[0;34mObjects clean\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C libs/libft

re: fclean all

.PHONY: all clean fclean bonus
.SILENT:
