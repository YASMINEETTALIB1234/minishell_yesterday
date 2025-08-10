/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPath_strjoin3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:48:04 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/06 16:36:46 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*get_path(void)
// {
// 	char	*path;

// 	path = getenv("PATH");
// 	if (path != NULL)
// 		return (path);
// 	else
// 		return (NULL);
// }

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin_execution(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin_execution(temp, s3);
	free(temp);
	return (result);
}

void	clean_string_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
