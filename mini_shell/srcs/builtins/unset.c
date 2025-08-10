/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:38:06 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/05 18:08:46 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_unset(char **args, t_env **env)
{
	int	i;

	i = 1;
	if (!args || !args[1] || !env || !*env)
	{
		// printf("entered jher\n");
		return (0);
	}
	while (args[i])
	{
		delete_env_variable(args[i], env);
		i++;
	}
	return (1);
}
