/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_code.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:25:24 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 15:54:02 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_single_builtin(t_cmd *cmd)
{
	return (size_list(cmd) == 1 && cmd->args && check_builtin(cmd->args[0]));
}

void	run_single_builtin(t_cmd *cmd, t_env **env)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (handle_redirections(cmd) == -1)
		exit_status_set(1);
	else
	{
		exit_status_set(0);
		execute_builtin(cmd->args, env);
	}
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
}

static void	try_exec_in_dirs(char **dirs, t_cmd *cmd)
{
	int		i;
	char	*full;
	char	**env_arr;

	i = 0;
	while (dirs[i])
	{
		full = ft_strjoin3(dirs[i], "/", cmd->args[0]);
		if (full && access(full, X_OK) == 0)
		{
			env_arr = env_to_array(cmd->env);
			execve(full, cmd->args, env_arr);
			free_strarray(env_arr);
		}
		free(full);
		i++;
	}
}

void	try_exec_in_path(t_cmd *cmd, char *path)
{
	char	**dirs;

	dirs = ft_split(path, ':');
	if (!dirs)
		exit(EXIT_FAILURE);
	try_exec_in_dirs(dirs, cmd);
	clean_string_array(dirs);
	write(2, "minishell: ", 11);
	write(2, cmd->args[0], ft_strlen(cmd->args[0]));
	write(2, ": command not found\n", 21);
	exit(127);
}
