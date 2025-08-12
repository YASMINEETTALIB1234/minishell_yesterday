/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:13:05 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/12 17:15:33 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handling_double_point( char *path, const char *old_pwd)
{
	char	*logical;

	if (ft_strcmp(path, "..") != 0 || access(path, X_OK) != 0)
		return (NULL);
	if (old_pwd[ft_strlen((char *)old_pwd) - 1] == '/')
		logical = ft_strjoin_execution(old_pwd, "..");
	else
		logical = ft_strjoin_execution(old_pwd, "/..");
	if (!logical)
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
	return (logical);
}

char	*get_new_pwd( char *target, const char *old_pwd)
{
	char	*new_pwd;
	char	*fallback_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd && old_pwd)
	{
		fallback_pwd = handling_double_point(target, old_pwd);
		if (!fallback_pwd)
			return (NULL);
		perror("cd: error retrieving current directory: \
		getcwd: cannot access parent directories: No such file or directory\n");
		return (fallback_pwd);
	}
	return (new_pwd);
}

int	execute_cd(char *target, t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = get_env_value(*env, "PWD");
	if (chdir(target) == -1)
	{
		write(2, "cd: ", 4);
		write(2, target, ft_strlen(target));
		write(2, ": No such file or directory\n", 29);
		return (1);
	}
	new_pwd = get_new_pwd(target, old_pwd);
	if (!new_pwd)
	{
		perror("cd: error retrieving current directory\n");
		return (1);
	}
	if (old_pwd)
		set_env(env, "OLDPWD", old_pwd);
	set_env(env, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

int	cd_command(int argc, char **argv, t_env **env)
{
	if (argc != 2)
	{
		perror("cd: usage: cd [path]\n");
		return (2);
	}
	return (execute_cd(argv[1], env));
}
