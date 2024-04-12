/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:04 by  ladloff          #+#    #+#             */
/*   Updated: 2024/04/12 14:47:37 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>

# define STR_BEYOND_LLONG_MAX "9223372036854775808"
# define STR_BEYOND_LLONG_MIN "9223372036854775809"

# define ESTR_QUOTE "minishell: syntax error near unmatched `%c'\n"
# define ESTR_UNEXP "minishell: syntax error near unexpected token `%c'\n"
# define ESTR_UNEXP_STR "minishell: syntax error near unexpected token `%s'\n"
# define ESTR_OPSTART_P1 "minishell: syntax error near unexpected token"
# define ESTR_OPSTART_P2 " `newline'\n"
# define ESTR_NUM_ARG "minishell: exit: %s: numeric argument required\n"
# define ESTR_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"
# define ESTR_INVALID_ID "minishell: export: '%s': not a valid identifier\n"
# define ESTR_DOT_P1 "minishell: .: filename argument required\n"
# define ESTR_DOT_P2 ".: usage: . filename [arguments]\n"
# define ESTR_NO_FILE "minishell: %s: No such file or directory\n"
# define ESTR_PERM_DENIED "minishell: %s: Permission denied\n"
# define ESTR_CMD_NOT_FOUND "minishell: %s: command not found\n"
# define ESTR_DIR "minishell: %s: Is a directory\n"
# define ESTR_HEREDOC_P1 "\nminishell: warning: here-document at line"
# define ESTR_HEREDOC_P2 " %d delimited by end-of-file (wanted `%s')\n"

# define DEFAULT_PATH_1 "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin"
# define DEFAULT_PATH_2 ":/opt/homebrew/bin"

typedef enum e_cmd_type
{
	CMD_ERROR = -1,
	CMD_ECHO,
	CMD_ENV,
	CMD_PWD,
	CMD_CD,
	CMD_EXIT,
	CMD_UNSET,
	CMD_EXPORT,
	CMD_OTHERS,
	CMD_NOCMD,
	CMD_PIPE,
	CMD_RED_IN,
	CMD_D_RED_IN,
	CMD_RED_OUT,
	CMD_D_RED_OUT,
}						t_cmd_type;

typedef enum e_exit_value
{
	MISUSE = 2,
	CANNOT_EXECUTE = 126,
	NOT_FOUND = 127,
	INVALID_EXIT = 128,
	INTERRUPTED = 130,
	INTERRUPTED_HEREDOC = 131,
	INTERRUPTED_TEMP = 132
}						t_exit_value;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
	struct s_env		*last;
}						t_env_list;

typedef struct s_token
{
	t_cmd_type			type;
	char				*data;
	struct s_token		*redir;
	struct s_token		*next;
	struct s_token		*last;
}						t_token;

typedef struct s_exec
{
	bool				heredoc;
	bool				first_cmd;
	pid_t				pid;
	int					pipefd[2];
	int					old_pipefd[2];
	int					stdin_fd;
	int					stdout_fd;
	t_cmd_type			type;
}						t_exec;

typedef struct s_master
{
	char				*line_read;
	t_env_list			*env;
	t_token				*token;
	t_exec				*exec;
	char				**argv;
	pid_t				*pid_list;
	int					argc;
	int					line_count;
	int					exit_status;
	int					last_command_exit_value;
	bool				reset_exit_status;
	bool				env_empty;
	bool				path_unset;
	struct sigaction	minishell_sa;
	struct sigaction	heredoc_sa;
	struct sigaction	temp_sa;
}						t_master;

typedef struct s_expansion
{
	size_t				i;
	char				*name;
	char				*value;
	char				*substr_start;
	bool				is_expanded;
	char				quote;
}						t_expansion;

typedef struct s_builtin
{
	char				*name;
	t_cmd_type			type;
}						t_builtin;

typedef struct s_lexer
{
	char				*data_command;
	char				*data_redir;
	t_token				*redirect;
}						t_lexer;

extern int				*g_exit_status;

/* cd.c */

int						builtin_cd(int argc, char **argv, t_master *master);

/* echo.c */

int						builtin_echo(int argc, char **argv, t_master *master);

/* env.c */

int						builtin_env(t_master *master);

/* exit.c */

int						builtin_exit(t_master *master, int argc, char **argv);

/* export.c */
int						builtin_export(int argc, char **argv, t_master *master);

/* pwd.c */

int						builtin_pwd(void);

/* unset.c */

int						builtin_unset(char **argv, t_master *master);

/* env.c */

int						manage_environment(t_env_list **env);

/* argument_creation.c */

void					create_arguments(t_master *master, t_token *token);

/* argument_quotes_removal.c */

void					replace_argv_without_quotes(t_master *master);

/* command_execution_utils.c */

void					find_executable_command_path(t_master *master);

/* command_execution_utils2.c */

bool					handle_command_not_found_error(t_master *master);

/* command_execution.c */

bool					is_executable_command(t_master *master);
t_cmd_type				identify_builtin_command(char *arg);
int						execute_builtin(t_master *master, t_cmd_type type);

/* execution_utils.c */

void					update_executable_path(t_master *master,
							t_env_list *current);
char					**env_list_to_array(t_master *master, t_env_list *env);
void					init_exec(t_master *master);
void					execute_command(t_master *master);

/* execution_utils2.c */

int						count_pipe(t_token *token);
t_token					*handle_command_error(t_master *master, t_token *token,
							t_cmd_type type);
void					wait_for_processes(t_master *master, int num_pids);

/* execution.c */

void					launch_execution(t_master *master);

/* expansion_utils.c */

char					*getenv_value(t_master *master, t_env_list *env,
							char *name);
char					*extract_expansion_name(t_master *master, char *str);
char					*create_new_string_with_value(t_master *master,
							char *str, t_expansion *exp);

/* expansion.c */

void					launch_expansion(t_master *master, char **str);

/* heredoc_utils.c */

int						create_or_open_file(char **filename, int *index);

/* heredoc.c */

void					launch_heredoc(t_master *master);

/* no_command.c */

int						no_command(t_master *master, t_token **token);

/* redirection.c */

void					replace_redir_without_quotes(t_master *master,
							char **str);
void					launch_redirection(t_master *master, t_token *token);

/* lexer_mem.c */

char					*trim_spaces(t_master *master, t_lexer *lexer,
							char *str);
void					create_token_node(t_master *master, t_lexer *lexer,
							t_cmd_type type, bool command);

/* lexer_utils.c */

int						start_operator(t_master *master);
int						to_pass(char *str, char *quote, char *ex_quote,
							size_t *i);

/* lexer_utils2.c */

t_cmd_type				redir_type(char *line_read, size_t *i);
void					exit_redir(t_master *master, size_t i);
bool					is_valid_character(char c, bool command, char *quote);
char					*creates_data(t_master *master, t_lexer *lexer,
							size_t *i, bool command);

/* lexer_utils4.c */

int						two_consecutive_pipe(t_master *master);
void					lexer_exit(t_master *master, t_lexer *lexer, char *str);
int						last_operator(t_master *master);

/* lexer.c */

int						launch_lexer(t_master *master);

/* quote_handling.c */

bool					is_matched_quotes(t_master *master, const char *str);

/* handler.c */

void					handle_minishell_sig(int signum);
void					handle_heredoc_sig(int signum);
void					handle_temp_sig(int signum);

/* signals.c */

int						set_sigaction(t_master *master);
int						set_sigaction_temp(t_master *master);
int						set_sigaction_heredoc(t_master *master);
int						restore_sigaction(t_master *master);

/* cleanup.c */

void					free_environment_list(t_env_list **env);
void					free_string_array(char ***str);
void					free_token(t_token **token);
void					cleanup_before_exit(t_master *master);
void					clean_lexer(t_lexer *lexer);

/* main_utils.c */

void					error_exit(t_master *master, char *error_str);
void					handle_eof(t_master *master);

#endif /* MINISHELL_H */
