#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libs/libft/libft.h"
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>


typedef struct s_status_shell
{
    int last_return;
    int no_exec; 
    int exit_status;
} t_status_shell;

typedef struct s_tmp_values
{
    int active_quote;
    char    *str;
} t_tmp_values;

typedef struct s_token
{
	char	*str;
	char	*type;
}	t_token;

typedef struct s_env
{
	char			*key;
	bool				equal;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
    t_token *tokens;
    t_env *env;
    bool is_parent_process;
    int stdin;
    int stdout;
    int fd_in;
    int fd_out;
    int pipe_out;
    int pipe_in;
    int charge;
    t_status_shell status;
    t_tmp_values tmp;
    int tokens_size;
    bool incomplete_pipe;
} t_shell;

#include "bultin.h"

void    init_shell(t_shell *shell,char **env);
void    token_analysis(t_shell *shell, char *line);

void    duplicate_env(t_shell *shell, char **env);

int     ft_readline(t_shell *shell, char **line);
void    skip_whitespace(char *line, int *i);

void	process_tokens(t_shell *shell, char *line, t_token *tokens, int numb_tokens);
bool    is_separator(char c);
char    *extract_token(char *input, int *index_inp, int *in_quotes);
void    toggle_quote_state(int *in_quotes, char *input, int *index);
char    *process_expansion(t_shell *shell, char *input, int is_heredoc);
int     calculate_expanded_length(t_shell *shell, char *input, int is_heredoc);
int     get_variable_length(t_shell *shell, char *input, int *index);
int     handle_return_value(t_shell *shell, int *index, int *total_length);
char	*ft_getenv(t_env *env, char *var);
bool     is_valid_token(char *str, int i, t_shell *shell);
bool     is_quote_unclosed(char *str, int i, char quote, t_shell *shell);
void    handle_quotes(t_shell *shell, char *input, int *index);

void	perform_variable_expansion(t_shell *shell, char *input, char *expanded, int in_heredoc);
void    handle_variable_expansion(t_shell *shell, char *expanded, int *index_inp, int *index_exp);
char	*get_env_value(t_shell *shell, char *input, int *index_inp);
char    *type_token(char *str, int in_quotes);
void	process_separator(char *line, int *index_line, t_token *tokens, int *index_tok);
char	*get_separator(char *line, int *index_line);
bool	is_type_token(t_token token, char *type);
int	compare_type(t_token token, char *type);

void    gettokens(t_shell *shell, char *input_line, t_token **tokens);
void    reorganize_tokens(t_token *tokens);
bool    is_valid_redirect_syntax(t_token *tokens, t_shell *shell);
void	dup_tokens(t_shell *shell, t_token *tokens);
bool	has_heredoc(t_shell *shell, t_token *tokens);

void	handle_execution(t_shell *shell, int *pos_token);
void	execute_command(t_shell *shell, char **cmd);


char    *get_command_path(char *cmd, t_shell *shell);
char    **env_to_matrix(t_env *env, int i, bool has_quote);
int	    get_env_count(t_env *env);

void    reset_std(t_shell *shell);
void    reset_fds(t_shell *shell);
void    close_fds(t_shell *shell);
void    ft_close(int fd);
void	process_command(t_shell *shell);

bool	has_heredoc(t_shell *shell, t_token *tokens);
void	handle_redirection_test(t_shell *shell);
void	reset_fds(t_shell *shell);
void	reset_std(t_shell *shell);
void    close_fds(t_shell *shell);
void    ft_close(int fd);
void	handle_execution(t_shell *shell, int *pos_token);
void	process_command(t_shell *shell);
void	handle_redirection(t_shell *shell, int token_index, int pipe);
int     minipipe(t_shell *shell);

void	handle_signals(t_shell *shell);
void	reset_signals_in_child(void);
void	sigint_handler(int sig);

void	ft_free(void *ptr_to_free);
void	free_env(t_env *head);
void    free_matrix(char **matrix);
void	ft_free_tokens(t_token *tokens);
#endif
