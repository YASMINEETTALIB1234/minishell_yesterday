/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:36:08 by yettalib          #+#    #+#             */
/*   Updated: 2025/07/29 18:44:45 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pwd_command(t_env *env)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	pwd = get_env_value(env, "PWD");
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
