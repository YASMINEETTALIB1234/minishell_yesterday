/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:48:07 by yettalib          #+#    #+#             */
/*   Updated: 2025/07/29 19:37:14 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_dotdot(char *out)
{
	char	*slash;

	slash = ft_strrchr(out, '/');
	if (slash && slash != out)
		*slash = '\0';
	else if (slash)
		*(slash + 1) = '\0';
	return (out);
}

char	*join_paths(const char *base, char *rel)
{
	char	*out;
	char	*tmp;
	char	*joined;

	if (rel[0] == '/')
		return (ft_strdup(rel));
	out = ft_strdup((char *)base);
	if (!out)
		return (NULL);
	if (!ft_strcmp(rel, ".."))
		return (handle_dotdot(out));
	tmp = ft_strjoin_execution(out, "/");
	joined = ft_strjoin_execution(tmp, rel);
	free(tmp);
	free(out);
	return (joined);
}

char	*join_kv(const char *key, const char *val)
{
	size_t	k;
	size_t	v;
	char	*out;

	k = 0;
	v = 0;
	if (key)
		k = strlen(key);
	if (val)
		v = strlen(val);
	out = malloc(k + v + 2);
	if (!out)
		return (NULL);
	ft_memcpy(out, key, k);
	out[k] = '=';
	ft_memcpy(out + k + 1, val, v);
	out[k + v + 1] = '\0';
	return (out);
}

