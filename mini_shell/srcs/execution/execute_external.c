/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:32:54 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 20:50:14 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execve_with_error(char *path, char **args, t_env **env)
{
	char	**env_arr;

	env_arr = env_to_array(*env);
	execve(path, args, env_arr);
	perror(path);
	free_strarray(env_arr);
	exit(126);
}

static void	handle_is_directory(char *cmd)
{
	int	fd;

	fd = open(cmd, O_DIRECTORY);
	if (fd != -1)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": Is a directory\n", 18);
		exit(126);
	}
}

static void	handle_access_error(char *cmd)
{
	if (access(cmd, X_OK) == -1)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
		perror("");
		if (errno == 2)
			exit(127);
		else if (errno == 13 || errno == 20)
			exit(126);
	}
}

void	execute_external(t_cmd *cmd, t_env **env)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		return ;
	path = get_env_value(*env, "PATH");
	if (!path || ft_strchr(cmd->args[0], '/'))
	{
		handle_is_directory(cmd->args[0]);
		handle_access_error(cmd->args[0]);
		execve_with_error(cmd->args[0], cmd->args, env);
	}
	try_exec_in_path(cmd, path);
}
