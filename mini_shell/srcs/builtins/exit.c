/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:35:35 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 18:05:55 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int		s;
	long	r;
	int		i;

	s = 1;
	r = 0;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s *= (-1);
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		if (s == -1 && r > (LONG_MAX / 10))
			return (0);
		else if (s == 1 && r > (LONG_MAX / 10))
			return (-1);
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (s * r);
}

static char	*skip_spaces(char *s)
{
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	return (s);
}

static char	*trim_arg(char *arg)
{
	char	*start;
	char	*end;

	start = skip_spaces(arg);
	end = start + ft_strlen(start) - 1;
	while (end > start && (*end == ' ' || (*end >= 9 && *end <= 13)))
		end--;
	*(end + 1) = '\0';
	return (start);
}

int	ft_exit(char **args, int must_exit_)
{
	long	code;
	char	*start;

	if (must_exit_ && (isatty(0) && isatty(1)))
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args || !args[1])
		exit(0);
	start = trim_arg(args[1]);
	code = ft_atoi(start);
	if (!is_numeric(start) || ((code == 0 && start[0] != '-')
			|| (code == -1 && start[0] == '-')))
	{
		write(2, "minishell: exit: ", 18);
		write(2, start, ft_strlen(start));
		write(2, ": numeric argument required\n", 29);
		exit(255);
	}
	if (args[2])
		return (write(2, "minishell: exit: too many arguments\n", 36), 1);
	exit((unsigned int)code);
}
