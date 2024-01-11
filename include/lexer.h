/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ladloff <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 10:44:30 by ladloff           #+#    #+#             */
/*   Updated: 2023/07/20 10:35:33 by ladloff         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# define OP 5

# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"
# define ESTR_DQUOTE "minishell: syntax error: unmatched double quote\n"
# define ESTR_UNEXP "minishell: syntax error: unexpected token '%c'\n"
# define ESTR_OPSTART "minishell: syntax error near unexpected token 'newline'\n"

typedef enum e_token_type
{
	T_BUILTIN,
	T_PIPE,
	T_RED_IN,
	T_D_RED_IN,
	T_RED_OUT,
	T_D_RED_OUT,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*data;
	struct s_token	*next;
	struct s_token	*last;
}	t_token;

/* lexer.c */
int		launch_lexer(char *line_read, t_token **token_list);

/* lexer_utils.c */
int		is_heredoc_pipe(t_token **token_lst);
int		start_operator(t_token_type type);
int		is_clean(t_token **token_lst);
int		is_matched_quotes(const char *line_read);
void	print_token_list(t_token *token_list);

/* lexer_mem.c */
void	free_token_list(t_token *token_list);
void	create_token_node(t_token_type type, char *data, t_token **token_list);

#endif /* LEXER_H */
