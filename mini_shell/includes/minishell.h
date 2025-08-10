#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/ioctl.h>
# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/readline.h>
# include <termios.h>
#include <readline/history.h>

#define NO_QUOTE 0
#define SINGLE_QUOTE 1
#define DOUBLE_QUOTE 2

# define IN     0
# define OUT    1
# define APPEND 2
# define MAX_HEREDOCS 16
# define NAME_OF_HEREDOC "/tmp/.heredoc_file"
# define EXIT_STATUS "$?"

# define AMBIGUOUS_MESSAGE "minishell: amgiguous redirect\n"
# define TARGET_NOT_FOUND "minishell: No such file or directory\n"

extern int	g_sig;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_END
}   t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
    struct s_token  *prev;
}   t_token;

typedef struct s_redir
{
    t_token_type    type;
    char            *target;
    char            *file; // file for heredoc
    int             fd; // file descriptor for heredoc
    int             ambiguous_flag;
    int             should_expand;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char			*line;
    char            **args;
    t_redir         *redirs;
    struct s_cmd    *next;
    struct s_env			*env;
}   t_cmd;

typedef struct s_env
{
    char			*key;
	char			*value;
    char			*kv;
    struct s_env    *next;
}   t_env;

typedef struct global_struct
{
    t_env   *env;
    t_token *tokens;
    t_cmd	*cmds;
}   global_struct;

// libft functions
void	ft_putstr_fd(char *s, int fd);
char	*ft_strdup(char *s1);
size_t	ft_strlen(char *s);
void	ft_putendl_fd(char *s, int fd);
char	*ft_itoa(int n);
int	    ft_isalnum(int c);
int	    ft_isalpha(int c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin_execution(char const *s1, char const *s2); // strjoin whithout free used in execution
int     ft_strcmp(char *s1, char *s2);
char	*ft_strdup_safe(const char *s);
int	    ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_strrchr(const char *s, int c);
char	*ft_strchr(const char *str, int character);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_isdigit(int c);
char	*ft_substr(char *s, size_t start, size_t len);
char	*ft_strjoin_char(char *s1, char c, char delimiter);
char	*ft_strjoin(char *s1, char *s2, char delimiter); // strjoin with delimiter and free s1 used in parsing
int	    count_words(char start, char *s, char last);
char	*ft_strndup(char *s, size_t n);

// utils parsing
int     is_space(char c);
int     is_operator(char c);
int     is_quote(char c);

// environment helpers
char	*join_kv(const char *key, const char *val);
t_env	*build_minimal_env(void);
char	*get_env_value(t_env *env_list, char *key);
void	set_env(t_env **env, const char *key, const char *value);
char	**env_to_array(t_env *env);
t_env	*create_env_node(const char *kv); // kaynin jouj
void	free_env_list(t_env *env); // join paths
t_env	*create_node(char *str, int add_equal);
void	free_environment(t_env **env);
t_env	*copy_environment(char **env);

// builtins check
int		execute_builtin(char **args, t_env **env);
int		check_builtin(char *cmd);

// builtins
int		cd_command(int argc, char **argv, t_env **env);
void	my_echo(int argc, char *argv[]);
int		pwd_command(t_env *env);
int		ft_exit(char **args, int must_exit_);
int		ft_unset(char **args, t_env **env);
void	print_env(char **env_array);
void	builtin_env(t_env *env);

// execute commandes
void execute_command(t_cmd *cmd_head, char **herdocs, t_env **env, int herdocs_count);

// rederections
int		ft_wait(pid_t *last_pid);
int		handle_redirections(t_cmd *cmd);

// unset utils
void	delete_env_variable(char *key, t_env **env);

// signals
void	setup_signals(void);

//getPath_strjoin3
char	*ft_strjoin3(char *s1, char *s2, char *s3);
void	clean_string_array(char **arr);

// heredocs
void	process_heredoc_line(char *line, t_redir *redir, t_env *env);
char	*handle_heredoc_input(t_redir *redir, char **heredocs, t_env *env , int index);
void	herdocs_clean(char **heredocs, int count);
// void	herdocs_prepare(t_cmd *cmd_list, char **heredoc);
int     herdocs_prepare(t_cmd *cmd_list, char **heredoc);
char	**herdoc_init(t_token *tokens);
int		count_heredocs(t_token *tokens);

// exit status
void    exit_status_set(int status);
int	    *exit_status_get(void);

// lexer
int     check_quotes_balance(char *input);
int     get_word_length(char *str);
t_token *create_token(char *value, t_token_type type);
void    add_token_to_list(t_token **head, t_token **tail, t_token *new_token);
t_token *lexer(char *input);
void    free_token_list(t_token *tokens);

// parser
void    free_redirections(t_redir *redirs);
void    free_command(t_cmd *cmd);
void    free_command_list(t_cmd *commands);
t_cmd   *parser(t_token *tokens, global_struct *global_struct);
int     validate_syntax(t_token *tokens);
t_cmd	*init_command_parsing(global_struct *global_struct);
t_redir *create_redirection(t_token_type type, char *target);
void    add_redirection_to_cmd(t_cmd *cmd, t_redir *redir);

// expander
t_cmd   *expand_pipeline(t_cmd *head, global_struct *global_struct);
char	*expand_variable(char *str, int *i, t_env *env_list, int exit_status);
int	    is_valid_var_char(char c);
int	    is_valid_var_start(char c);
void	expand_redir_target(t_redir *redir, global_struct *global_struct);
void	process_heredoc_delimiter(t_redir *redir);
char    *expand_export(char *line, global_struct *global_struct);
char    *expand(char *line, global_struct *global_struct, int start_quote_state);
void	expand_line(t_cmd *cmd, global_struct *global_struct);
void	free_args(char **p, int n_words);
char	**line_to_args(char *line);

// quotes
int     check_quotes_type(char *str);
char	*remove_quotes(char *target);
int     check_quotes_type(char *str);
void	update_quote_state(char c, int *quote_state);
char    get_quote_char(int macro);

// Print cmd list just for test
void print_cmd_list(t_cmd *cmd);

# endif