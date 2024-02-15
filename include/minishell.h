/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <ladloff@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:04 by  ladloff          #+#    #+#             */
/*   Updated: 2024/02/15 11:25:41 by ladloff          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>

# define OP 5
# define MAX_PIDS 30

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_MISUSE 2
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_NOT_FOUND 127
# define EXIT_INVALID_EXIT 128
# define EXIT_INTERRUPTED 130

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define ESTR_DQUOTE "minishell: syntax error: unmatched double quote\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define ESTR_OPSTART_P1 "minishell: syntax error near unexpected token"
# define ESTR_OPSTART_P2 " `newline'\n"
# define ESTR_NUM_ARG "minishell: exit: %s: numeric argument required\n"
# define ESTR_EXIT_TOO_MANY_ARGS "minishell: exit: too many arguments\n"
# define ESTR_CD_TOO_MANY_ARGS "minishell: cd: too many arguments\n"
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

# define SHLVL "SHLVL"

typedef enum e_cmd_type
{
	CMD_ERROR = -1,
	CMD_CD,
	CMD_ECHO,
	CMD_ENV,
	CMD_EXIT,
	CMD_PWD,
	CMD_UNSET,
	CMD_EXPORT,
	CMD_OTHERS,
	CMD_PIPE,
	CMD_RED_IN,
	CMD_D_RED_IN,
	CMD_RED_OUT,
	CMD_D_RED_OUT,
}						t_cmd_type;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
	struct s_env		*last;
}						t_env;

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
	int					argc;
	char				**argv;
	bool				pipe;
	bool				heredoc;
	bool				first_cmd;
	pid_t				pid;
	int					pipefd[2];
	int					old_pipefd[2];
	int					stdin_fd;
	int					stdout_fd;
}						t_exec;

typedef struct s_master
{
	t_env				*env_list;
	char				*line_read;
	t_token				*token;
	t_exec				*exec;
	int					line_count;
	int					exit_status;
	int					prev_exit_status;
	struct sigaction	minishell_sa;
	struct sigaction	heredoc_sa;
	struct sigaction	temp_sa;
}						t_master;

typedef struct s_expansion
{
	size_t				i;
	size_t				j;
	char				*name;
	char				*value;
	char				*substr_start;
	bool				double_quote;
}						t_expansion;

typedef struct s_builtin
{
	char				*name;
	t_cmd_type			type;
}						t_builtin;

extern int				*g_exit_status;

/* builtin_utils.c */

bool					handle_command_not_found_error(t_master *master);

/* builtin.c */

int						execute_builtin(t_master *master, t_cmd_type type);
t_cmd_type				execute_command_or_builtin(t_master *master);

/* execution_mem.c */

void					create_arguments(t_master *master, t_token *token);

/* execution_utils.c */

char					**env_list_to_array(t_master *master, t_env *env_list);
void					init_exec(t_master *master);
void					execute_command(t_master *master);

/* execution.c */

void					launch_execution(t_master *master);

/* expansion_utils.c */

char					*get_env_value(t_master *master, t_env *env,
							char *name);
char					*extract_expansion_name(t_master *master, char *str);
int						replace_argv_without_quotes(t_master *master,
							t_expansion *exp);
char					*replace_redir_without_quotes(t_master *master,
							char *str);

/* expansion.c */

void					launch_expansion(t_master *master);

/* split_args.c */

int						split_args(t_master *master, char *s, char **argv,
							bool echo);
char					*creates_arg(t_master *master, char *s, size_t *j);
char					*creates_arg_for_echo(t_master *master, char *s,
							size_t *j);
char					*creates_quoted_arg(t_master *master, char *s,
							size_t *j);
bool					is_not_escaped(char *s, int index);

/* cleanup.c */

void					free_string_array(char **str);
void					free_token(t_token **token);
void					cleanup_executable(t_master *master);
void					cleanup_before_exit(t_master *master);

/* exit.c */

void					error_exit(t_master *master, char *error_str,
							bool free_all_exec);
void					ft_error_exit(t_master *master, char *error_str,
							int errnum, bool free_all_exec);
void					handle_eof(t_master *master);

/* lexer.c */

int						launch_lexer(t_master *master, char *line_read,
							t_token **token);
char					*trim_spaces(t_master *master, const char *str,
							size_t start, size_t end);
int						manage_redirection(const char *line_read, size_t *i,
							bool redir);

/* quote_handling.c */

bool					is_matched_quotes(t_master *master,
							const char *line_read);

/* lexer_utils.c */
int						exit_handler(t_master *master, t_token **token);

/* lexer_utils2.c */

bool					is_in_quotes(const char *line, size_t *i);
t_cmd_type				is_builtin(const char *line_read, size_t *i);

/* lexer_utils3.c */

int						create_data_command(char *line_read, size_t startend[2],
							char **data);
void					next_sign(char *line_read, size_t *i, size_t *start,
							size_t *end);
void					pass_redirection(const char *line_read, size_t *i);

/* lexer_utils4.c */

void					next_sign_redir(char *line_read, size_t *i, size_t *k);
int						create_redir(t_master *master, char *line_read,
							t_token **token, size_t *ik[2]);

/* lexer_mem.c */

void					create_token_node(t_master *master, t_cmd_type type,
							char *data, t_token **token);

/* handlers.c */

void					handle_minishell_sig(int signum);
void					handle_heredoc_sig(int signum);
void					handle_temp_sig(int signum);

/* signals.c */

void					restore_sigaction(t_master *master);
void					set_sigaction(t_master *master);
void					set_sigaction_heredoc(t_master *master);
void					set_sigaction_temp(t_master *master);

/* env_utils.c */

void					update_executable_path(t_exec *exec, t_env *env_list);
char					*update_shlvl(t_master *master, char *value,
							char *name);

/* env.c */

void					free_environment_list(t_env *env);
void					manage_environment(t_master *master, t_env **env_list);

int						ft_cd(int argc, char **argv, t_master *master);
int						ft_echo(int argc, char **argv, t_master *master);
int						ft_env(t_master *master);
void					ft_exit(t_master *master, int argc, char **argv);
int						ft_export(int argc, char **argv, t_master *master);
int						ft_pwd(void);
int						ft_unset(int argc, char **argv, t_master *master);

/* redirection.c */

void					launch_redirection(t_master *master, t_token *token);

/* heredoc.c */

void					launch_heredoc(t_master *master);

#endif /* MINISHELL_H */
