/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mavitori <mavitori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:00:35 by andrefil          #+#    #+#             */
/*   Updated: 2024/07/02 18:46:48 by mavitori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h>

typedef enum e_tokens		t_tokens;
enum						e_tokens
{
	DELIMITER_TOKEN = -3,
	CMD_TOKEN = -2,
	FILE_TOKEN = -1,
	NONE = 0,
	WORD,
	EXPANSION,
	GREAT,
	DGREAT,
	LESS,
	DLESS,
	PIPE,
};

typedef enum e_ast_type		t_type_ast;
enum						e_ast_type
{
	STRING = 10,
	DELIMITER,
	FILENAME,
	COMMAND,
	OUTPUT,
	APPEND,
	INPUT,
	HERE_DOC,
	PIPE_LINE,
};

typedef enum e_returns		t_returns;
enum						e_returns
{
	END = 666,
	ERROR = -1,
	FALSE = 0,
	TRUE = 1,
};

typedef enum e_sides		t_side;
enum						e_sides
{
	LEFT = 0,
	RIGHT,
};

typedef enum e_check		t_check;
enum						e_check
{
	EMPTY = -2,
	OK = 0,
	KO = 1,
};

typedef struct s_token		t_token;
struct						s_token
{
	char					*content;
	t_tokens				token;
	t_token					*next;
	t_token					*prev;
};

typedef struct s_ast		t_ast;
struct						s_ast
{
	char					*content;
	t_type_ast				ast_type;
	t_token					*left_token;
	t_token					*right_token;
	t_ast					*left;
	t_ast					*right;
};

typedef struct s_env_var	t_env_var;
struct						s_env_var
{
	char					*key;
	char					*value;
	t_env_var				*next;
};

typedef struct s_cmd		t_cmd;
struct						s_cmd
{
	char					*bin;
	char					**cmd_flag;
};

typedef struct s_data		t_data;
struct						s_data
{
	t_token					*token_list;
	t_ast					*ast_list;
	t_env_var				*envp;
	int						last_status;
	int						has_att;
	int						has_env;
	int						direction;
	int						pipe_fd[2];
	pid_t					fork[2];

};

/*----- INIT-UTILS: -----*/
int							start_minishell(t_env_var **envp, char *input,
								int status);

/*----- SIGNALS: -----*/
extern volatile int			g_signal;
int							inside_heredoc(int inside_heredoc);
void						handle_new_line(int signum);
void						handle_signal(int signum);
void						init_signals(void);

/*----- STR-UTILS: -----*/
int							ft_isspace(int c);
char						*ft_strndup(const char *src, int size);

/*----- TOKEN-CONSTRUCTOR: -----*/
int							build_token(char *cmd_line, t_token **token_list);

/*----- TOKEN-UTILS: -----*/
t_token						*create_redirect_token(int *i, char *cmd_line,
								char c);
void						create_quote_token(char *cmd_line, int *size);
t_token						*find_last_cmd(t_token *current);
void						relink_tokens(t_token *target, t_token *first_cmd,
								t_token *last_cmd,
								t_token *input);
t_token						*build_redirect_token(int *i, char *cmd_line);

/*----- TOKEN-LIST-UTILS: -----*/
t_token						*token_lstnew(char *content, t_tokens type);
void						token_lstadd_back(t_token **lst, t_token *new);
t_token						*token_lstlast(t_token *lst);
void						token_lstclear(t_token **lst);

/*----- TOKEN-UPGRADE-UTILS: -----*/
t_token						*upgrade_token_by_type(t_token *current,
								int *has_operator, t_data *data,
								t_env_var **envp);

/*----- TOKEN-UPGRADE: -----*/
t_token						*upgrade_token_list(t_data *data, t_env_var **envp);
t_token						*upgrade_env_var(t_token *token, t_env_var **envp,
								t_data *data);

/*----- TOKEN-REORGANIZE: -----*/
t_token						*reorganize_redirect_tokens(t_token *root);
int							parse_token_constructor(char *cmd_line, int *size,
								int has_quote);

/*----- ENV-TOKEN-UPGRADE: -----*/
void						search_env_key(char *env_key,
								char **final_line, t_env_var **envp);
char						*concat_env_var(int *i, char *content,
								char **final_line, t_data *data);
void						concat_word(int *i, char *content,
								char **final_line, int has_single_quote);
t_token						*rebuild_env_token(t_token *token,
								char **final_line, t_data *data);

/*----- TOKEN-SEQUENCE: -----*/
t_token						*reorganize_tokens(t_token **token_list);

/*----- AST-CONSTRUCTOR: -----*/
void						scan_token_list(t_token **token_list,
								t_ast **ast_list);

/*----- AST-LIST-UTILS: -----*/
void						ast_lstadd_right(t_ast **lst, t_ast *new);
void						ast_lstadd_left(t_ast **lst, t_ast *new);
t_ast						*ast_lstnew(char *content, t_tokens type);
void						ast_lstclear(t_ast **lst);
void						build_ast_node(t_token *new_node,
								t_ast **ast_list, int direction);

/*----- AST-UTILS: -----*/
t_type_ast					type_check(t_tokens token);
void						check_multi_cmd_left(t_ast *node,
								t_ast *current_ast_node);
void						check_multi_cmd_right(t_ast *node,
								t_ast *current_ast_node);

/*----- AST-DIVISION-UTILS: -----*/
t_ast						*break_in_two(t_token *current, t_ast **ast_list,
								int state);
void						continue_division_left(t_tokens operator,
								t_ast *ast_current_node, t_ast **ast_list,
								int status);
void						continue_division_right(t_tokens operator,
								t_ast *ast_current_node, t_ast **ast_list,
								int status);

/*----- BUILTINS-UTILS: -----*/
int							call_builtins(char **args, t_data *data,
								t_env_var **envp);

/*----- DIR: -----*/
int							call_pwd(char *input);
int							change_dir(char **input, t_env_var **envp);
int							ft_clear(char *input);

/*----- ECHO: -----*/
int							ft_echo(char **arg, t_data *data);

/*----- BUILTINS-UTILS: -----*/
int							ft_env(t_env_var **envp, int status);
int							ft_unset(t_env_var **head, char *key);

/*----- EXPORT-UTILS: -----*/
void						ft_export_new_var(char *var, t_env_var **envp);

/*----- EXPORT: -----*/
int							print_env_order(t_env_var *env);

/*----- BEGIN-EXECUTING: -----*/
int							begin_executing(t_data *data, t_env_var **envp);
int							execute_ast(t_ast *node, t_data *data);

/*----- EXECUTE-COMMAND-UTILS: -----*/
char						**get_cmd_args(t_ast *node, int direction);
char						**get_path(t_env_var **envp, char *var, char *cmd);
t_ast						*find_cmd(t_ast **root, int *count);
int							consume_redir_in_ast(t_ast *node, \
								t_ast **cmd);

/*----- EXECUTE-COMMAND: -----*/
int							execute_command(t_ast *node, t_data *data,
								t_env_var **envp, int direction);

/*----- EXECUTE-COMMAND-ERRORS: -----*/
int							execution_error(char **cmd_args, t_env_var **envp,
								int status);

/*----- ENV-VARS-UTILS: -----*/
t_env_var					*env_lstnew(char *key, char *value);
void						env_lstadd_back(t_env_var **lst, \
								t_env_var *new_node);
void						env_lstclear(t_env_var **lst);
t_env_var					*env_lstsearch(t_env_var **lst, char *key);
int							env_size(t_env_var **env);

/*----- GET-ENV: -----*/
void						get_envp(t_env_var **envp, char **environ);

/*----- SYNTAX-CHECKER: -----*/
int							syntax_checker(const char *input);

/*----- SYNTAX-CHECKER-UTILS: -----*/
int							check_pipe(const char *input);

/*----- CLOSE-FREE: -----*/
void						close_fd_fork(int *fd, char *error);
void						free_matrix(char **matrix);
void						free_data(t_data **data);
void						free_envp(t_env_var **envp);
void						free_all_exec_args(char **path_args, char **args);

/*----- EXECUTE-EXPANSIONS: -----*/
char						*get_cmds_args_quote(char *content);
char						*get_cmds_args_word(char *content);

/*----- EXECUTE-EXPANSIONS-UTILS: -----*/
char						*remove_quotes_expansion(char *content);

/*-------- OPEN-REDIRECTS: ------*/
int							open_redir_input(t_ast *operator);
int							open_redir_output(t_ast *operator);
int							open_redir_append(t_ast *operator);

/*----- EXECUTE-HERE-DOC: -----*/
int							execute_heredoc(t_ast *operator, t_data *data,
								t_env_var **envp, int index);
char						*expand_env(char *content, t_env_var **envp,
								t_data *data);

/*----- TERMINATE-PROGRAM: -----*/
int							terminate_program(t_data *data,
								t_env_var **envp, char **cmd_args);
int							check_exit(char *input);
char						**check_exit_execute(t_ast *node, int direction);

/*----- EXECUTE-PIPE: -----*/
int							execute_pipe(t_ast *operator, t_data *data);
int							finish_pipe(int *status, t_data *data);

/*----- EXECUTE-SINGLE-CMD: -----*/
int							execute_single_multi_cmd(t_ast *node, t_data *data, \
								t_env_var **envp);

/*----- HERE-DOC-UTILS: -----*/
void						here_doc_eof(int line_num, char *content, \
								char *line);
int							unlink_here_doc(t_ast *operator);
int							verify_quotes_heredoc(char *content);
void						write_on_heredoc(char *line, int fd_heredoc, \
								t_data *data, t_env_var **envp);
int							search_here_doc(t_ast *root, t_data *data,
								t_env_var **envp);

#endif
