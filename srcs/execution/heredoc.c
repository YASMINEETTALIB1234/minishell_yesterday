/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:06:25 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/12 16:54:48 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	check_heredoc_limit(int count)
{
	if (count > MAX_HEREDOCS)
	{
		perror("maximum here-document count exceeded");
		exit(2);
	}
}

char	**herdoc_init(t_token *tokens)
{
	int		count;
	char	**table;
	int		i;

	if (tokens == NULL)
		return (NULL);
	count = count_heredocs(tokens);
	check_heredoc_limit(count);
	table = malloc((count + 1) * sizeof(char *));
	i = 0;
	while (table && i < (count + 1))
		table[i++] = NULL;
	return (table);
}

char	*random_name_of_heredoc(void)
{
	static int	counter;
	char		*num_str;
	char		*name;

	counter = 0;
	counter++;
	num_str = ft_itoa(counter);
	if (!num_str)
		return (NULL);
	name = ft_strjoin_execution(NAME_OF_HEREDOC, num_str);
	if (!name)
		return (free(num_str), NULL);
	free(num_str);
	return (name);
}

static int	read_heredoc_loop(t_redir *redir, t_env *env)
{
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (g_sig == 3)
		{
			free(input);
			return (1);
		}
		if (!input || !ft_strcmp(input, redir->target))
		{
			free(input);
			break ;
		}
		process_heredoc_line(input, redir, env);
	}
	return (0);
}

char	*handle_heredoc_input(t_redir *redir, char **heredocs,
		t_env *env, int index)
{
	char	*filename;
	int		interrupted;

	filename = random_name_of_heredoc();
	if (!filename)
		return (NULL);
	heredocs[index] = ft_strdup(filename);
	if (!heredocs[index])
		return (free(filename), NULL);
	redir->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (redir->fd == -1)
		return (free(filename), NULL);
	g_sig = 2;
	interrupted = read_heredoc_loop(redir, env);
	close(redir->fd);
	g_sig = 0;
	if (interrupted)
		return (free(filename), NULL);
	return (filename);
}
