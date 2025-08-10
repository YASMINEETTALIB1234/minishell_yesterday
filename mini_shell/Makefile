CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3


SRC = \
libft/ft_count_words.c \
libft/ft_isalnum.c \
libft/ft_isalpha.c \
libft/ft_isdigit.c \
libft/ft_itoa.c \
libft/ft_memcpy.c \
libft/ft_putendl_fd.c \
libft/ft_putstr_fd.c \
libft/ft_strchr.c \
libft/ft_strcmp.c \
libft/ft_strdup.c \
libft/ft_strdup_safe.c \
libft/ft_strjoin.c \
libft/ft_strjoin_char.c \
libft/ft_strlen.c \
libft/ft_strncmp.c \
libft/ft_strndup.c \
libft/ft_strrchr.c \
libft/ft_substr.c \
\
srcs/main.c \
\
srcs/builtins/cd.c \
srcs/builtins/echo.c \
srcs/builtins/env.c \
srcs/builtins/exit.c \
srcs/builtins/pwd.c \
srcs/builtins/unset.c \
\
srcs/execution/builtins_check.c \
srcs/execution/exit_status.c \
srcs/execution/heredoc.c \
srcs/execution/prepare_heredoc.c \
srcs/execution/rederection.c \
srcs/execution/signals.c \
srcs/execution/execution_code.c \
\
srcs/expander/args_handler.c \
srcs/expander/expander.c \
srcs/expander/expand_export.c \
srcs/expander/expand_heredoc.c \
srcs/expander/expand_line.c \
srcs/expander/expand_target.c \
srcs/expander/quotes.c \
srcs/expander/utils.c \
\
srcs/lexer/tokenizer.c \
srcs/lexer/utils.c \
\
srcs/parser/parser.c \
srcs/parser/syntax_errors.c \
srcs/parser/utils.c \
\
srcs/utils_execution/env_management.c \
srcs/utils_execution/getPath_strjoin3.c \
srcs/utils_execution/helpers_env.c \
srcs/utils_execution/split_strjoin_utils.c \
srcs/utils_execution/strdup_strlen_utils.c \
srcs/utils_execution/unset_utils.c \
srcs/utils_execution/utils_env.c \
\
srcs/utils_parsing/free.c \
srcs/utils_parsing/utils.c


OBJS = $(SRC:.c=.o)
TARGET = minishell

all: $(TARGET)

READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

$(TARGET): includes/minishell.h $(OBJS)
	$(CC) $(CFLAGS) $(READLINE_COMPILE) $(OBJS) -o $(TARGET) $(READLINE_LINK)

%.o: %.c
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
