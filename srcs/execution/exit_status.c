/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:22:15 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/10 15:14:41 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*exit_status_get(void)
{
	static int	status;

	return (&status);
}

void	exit_status_set(int status)
{
	int	*st;

	st = exit_status_get();
	*st = status;
}
