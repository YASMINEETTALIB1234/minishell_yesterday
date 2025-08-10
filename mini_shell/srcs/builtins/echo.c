/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:21:31 by yettalib          #+#    #+#             */
/*   Updated: 2025/07/29 18:44:13 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_n_option(const char *arg)
{
	int	i;

	i = 2;
	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	my_echo(int argc, char *argv[])
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	while (i < argc && is_valid_n_option(argv[i]))
	{
		newline = false;
		i++;
	}
	while (i < argc)
	{
		ft_putstr_fd(argv[i], 1);
		if (i < argc - 1)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}
