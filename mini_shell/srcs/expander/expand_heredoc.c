/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:01 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:02 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_heredoc_delimiter(t_redir *redir)
{
	char	*clean_delimiter;

	if (check_quotes_type(redir->target) != NO_QUOTE)
		redir->should_expand = 0;
	clean_delimiter = remove_quotes(redir->target);
	if (!clean_delimiter)
		return ;
	free(redir->target);
	redir->target = clean_delimiter;
}

static char    *expand_heredoc(char *line, t_env *env)
{
	char 	*var_expansion;
	char	*result;
    int		i;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    while (line[i])
    {
        if (line[i] == '$')
        {
            var_expansion = expand_variable(line, &i, env, *exit_status_get());
            if (var_expansion)
            {
                result = ft_strjoin(result, var_expansion, 0);
                free(var_expansion);
            }
        }
        else
            result = ft_strjoin_char(result, line[i++], 0);
        if (!result)
            return (NULL);
    }
    return (result);
}

void	process_heredoc_line(char *line, t_redir *redir, t_env *env)
{
	char	*clean_line;

	if (redir->should_expand)
	{
		clean_line = expand_heredoc(line, env);
		free(line);
	}
	else
		clean_line = line;
	write(redir->fd, clean_line, strlen(clean_line));
	write(redir->fd, "\n", 1);
	free(clean_line);
}