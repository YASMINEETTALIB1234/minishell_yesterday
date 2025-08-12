/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 10:11:58 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/12 18:09:25 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export"));
}

static int	handle_builtin(char **args, t_env **env, int argc)
{
	if (!ft_strcmp(args[0], "cd"))
		return (cd_command(argc, args, env));
	else if (!ft_strcmp(args[0], "pwd"))
		return (pwd_command(*env));
	else if (!ft_strcmp(args[0], "env"))
	{
		builtin_env(*env);
		return (0);
	}
	else if (!ft_strcmp(args[0], "echo"))
		my_echo(argc, args);
	else if (!ft_strcmp(args[0], "exit"))
		return (ft_exit(args, 1));
	else if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, env));
	else if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, env));
	return (0);
}

int	execute_builtin(char **args, t_env **env)
{
	int	argc;
	int	result;

	argc = 0;
	result = 0;
	while (args && args[argc])
		argc++;
	if (!argc)
		return (1);
	result = handle_builtin(args, env, argc);
	return (result);
}
