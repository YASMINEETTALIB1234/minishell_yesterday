#include "../../includes/minishell.h"

static void	skip_spaces(char *str, int *i)
{
	while (str[*i] && is_space(str[*i]))
		(*i)++;
}

static int	get_arg_len(char *str, int start)
{
	int		i;
	char	in_quotes;
	char	quote_char;

	i = 0;
	in_quotes = 0;
	quote_char = '\0';
	while (str[start + i])
	{
		if (is_quote(str[start + i]))
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = str[start + i];
			}
			else if (str[start + i] == quote_char)
				in_quotes = 0;
		}
		else if (is_space(str[start + i]) && !in_quotes)
			break ;
		i++;
	}
	return (i);
}

static char	*extract_arg(char *str, int *i)
{
	int		len;
	char	*arg;

	skip_spaces(str, i);
	len = get_arg_len(str, *i);
	arg = ft_strndup(str + *i, len);
	if (!arg)
		return (NULL);
	*i += len;
	return (arg);
}

int	count_args(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		skip_spaces(str, &i);
		if (str[i])
		{
			count++;
			i += get_arg_len(str, i);
		}
	}
	return (count);
}

char	**line_to_args(char *line)
{
	char	**args;
	int		arg_count;
	int		i;
	int		j;

	if (!line)
		return (NULL);
	arg_count = count_args(line);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (j < arg_count)
	{
		args[j] = extract_arg(line, &i);
		if (!args[j])
			return (free_args(args, j), NULL);
		j++;
	}
	args[arg_count] = NULL;
	return (args);
}
