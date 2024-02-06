/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfloren <alfloren@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 11:59:04 by  ladloff          #+#    #+#             */
/*   Updated: 2024/02/06 12:40:03 by alfloren         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <sys/types.h>
# include <stdio.h>
# include "libft.h"
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <unistd.h>
# include "ft_dprintf.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>


# define OP 5
# define MAX_PIDS 30

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_MISUSE 2
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_NOT_FOUND 127
# define EXIT_INVALID_EXIT 128

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define ESTR_DQUOTE "minishell: syntax error: unmatched double quote\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define ESTR_OPSTART "minishell: syntax error near unexpected token `newline'\n"
# define ESTR_OPEND "minishell: syntax error near unexpected token"
# define ESTR_DOT_P1 "minishell: .: filename argument required\n"
# define ESTR_DOT_P2 ".: usage: . filename [arguments]\n"
# define ESTR_DIR "minishell: %s: Is a directory\n"
# define ESTR_CMD_NOT_FOUND "minishell: %s: command not found\n"
# define NUM_ARG_ERR "minishell: exit: %s: numeric argument required\n"
# define TOO_MANY_ARGS_ERR "minishell: exit: too many arguments\n"
# define ESTR_PERM_DENIED "minishell: %s: Permission denied\n"
# define ESTR_NO_FILE "minishell: %s: No such file or directory\n"
# define ESTR_INVALID_ID "minishell: export: '%s': not a valid identifier\n"
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
}					t_cmd_type;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*last;
}					t_env;

typedef struct s_token
{
	t_cmd_type		type;
	char			*data;
	struct s_token	*redir;
	struct s_token	*next;
	struct s_token	*last;
}					t_token;

typedef struct s_exec
{
	int				argc;
	char			**argv;
	char			*pathname;
	t_token			*token;
	pid_t			pid;
	int				pipefd[2];
	int				old_pipefd[2];
	bool			first_cmd;
	bool			simple_quotes;
	bool			redir;
	int				stdin_fd;
	int				stdout_fd;
}					t_exec;

typedef struct s_master
{
	t_env			*env_list;
	char			*line_read;
	t_token			*token_list;
	t_exec			*exec;
	int				stdout_fd;
}					t_master;

typedef struct s_expansion
{
	size_t			i;
	size_t			j;
	char			*name;
	char			*value;
	char			*substr_start;
}					t_expansion;

typedef struct s_builtin
{
	char			*name;
	t_cmd_type		type;
}					t_builtin;

extern int			g_exit_status;

/* builtin_utils.c */

void				handle_error_cases(t_exec *exec);

/* builtin.c */

int					execute_builtin(t_master *master, t_exec *exec,
						t_cmd_type type);
t_cmd_type			execute_command_or_builtin(t_master *master, t_exec *exec);
void				handle_command_error(t_exec *exec);

/* execution_mem.c */

void				free_double_ptr(char **str);
t_exec				*create_arguments(t_master *master, t_token *token);

/* execution_utils.c */

char				**env_list_to_array(t_master *master, t_env *env_list);
void				init(t_exec *exec, int *status, int *num_pids);
void				execute_command(t_master *master);
void				chose_execute(t_master *master,
						t_exec *exec, t_cmd_type type);

/* execution.c */

void				launch_execution(t_master *master);
void				print_token_list(t_token *token);

/* execution_utils2.c */
void				creation_pipe(t_exec *exec,
						t_master *master, t_token *token);
t_cmd_type			preparation_args(t_master *master, t_token *token);
void				launch_builtin(t_master *master, t_exec *exec,
						t_cmd_type type, t_token *token);

/* expansion_utils.c */

char				*get_env_value(t_master *master, t_env *env, char *name);
char				*extract_expansion_name(t_master *master, char *str);

/* expansion.c */

void				launch_expansion(t_master *master, t_exec *exec);

/* split_args.c */

void				split_args(t_master *master, char *s, char **argv,
						bool *simple_quotes);

/* cleanup.c */

void				cleanup_executable(t_master *master);
void				cleanup_before_exit(t_master *master);

/* exit.c */

void				error_exit(t_master *master, char *error_str);
void				ft_error_exit(t_master *master, char *error_str,
						int errnum);
void				handle_eof(t_master *master);
void				ft_error_exit2(t_master *master, char *error_str,
						char errnum);

/* lexer.c */

int					launch_lexer(t_master *master, char *line_read,
						t_token **token_list);
char				*trim_spaces(t_master *master, const char *str,
						size_t start, size_t end);
int					manage_redirection(const char *line_read, size_t *i,
						bool redir);

/* lexer_utils.c */

int					is_heredoc_pipe(t_token **token_lst);
int					start_operator(t_cmd_type type);
int					is_clean(t_token **token_lst);
int					is_matched_quotes(const char *line_read);

/* lexer_utils2.c */

int					is_escaped(const char *str, int index);
bool				is_in_quotes(const char *line, size_t *i);
int					exit_handler(t_token **token_lst);
t_cmd_type			is_builtin(const char *line_read, size_t *i);
t_cmd_type			isnot_builtins(char c, const char *line_read, size_t *i);

/* lexer_utils3.c */

int					create_data_command(char *line_read, size_t startend[2],
						char **data);
void				next_sign(char *line_read, size_t *i, size_t *start,
						size_t *end);
void				pass_redirection(const char *line_read, size_t *i);
void				free_exit(char *data, t_master *master);

/* lexer_utils4.c */

t_cmd_type			if_redir(char *line_read, size_t *i, t_cmd_type type);
int					finish_line(char *line_read, size_t *i, t_master *master,
						t_token **token_list);
void				next_sign_redir(char *line_read, size_t *i, size_t *k);
int					create_redir(t_master *master, char *line_read,
						t_token **token_list, size_t *ik[2]);
char				*trim_spaces1(const char *str, size_t startend[2]);

/* lexer_mem.c */

void				free_token_list(t_token *token_list);
void				create_token_node(t_master *master, t_cmd_type type,
						char *data, t_token **token_list);

/* signals.c */

void				set_sigaction(void);

/* env_utils.c */

void				update_executable_path(t_exec *exec, t_env *env_list);
void				print_environement_list(t_env *env_list);
void				print_environement_var(t_env *env_list, char *name);
char				*update_shlvl(t_master *master, char *value, char *name);

/* env.c */

void				free_environment_list(t_env *env);
void				manage_environment(t_master *master, t_env **env_list);

int					ft_cd(int argc, char **argv, t_master *master);
int					ft_echo(int argc, char **argv, t_master *master);
void				ft_env(t_master *master);
void				ft_exit(t_master *master, int argc, char **argv);
int					ft_export(int argc, char **argv, t_master *master);
int					ft_pwd(void);
int					ft_unset(int argc, char **argv, t_master *master);

/* redirection.c */
int					launch_redirection(t_master *master, t_token *token,
						t_exec *exec);
void				restore_fd(int saved_stdin, int saved_stdout);

/* heredoc.c */

void				launch_heredoc(t_master *master);
#endif /* MINISHELL_H */
