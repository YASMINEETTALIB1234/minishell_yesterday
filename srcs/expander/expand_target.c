/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_target.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oafidi <oafidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:06 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:42:08 by oafidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_quote(char *target, t_expand_ctx *ctx)
{
	if (target[ctx->i] != get_quote_char(ctx->quote_state) && ctx->quote_state != NO_QUOTE)
		ctx->result = ft_strjoin_char(ctx->result, target[ctx->i], 0);
	update_quote_state(target[(ctx->i)++], &ctx->quote_state);
}

static void	handle_var_expand(t_redir *redir, char *target, t_expand_ctx *ctx, global_struct *g)
{
	char	*var_exp;
	int		last_index;

	var_exp = expand_variable(target, &ctx->i, g->env);
	if (!var_exp)
	{
		free(ctx->result);
		ctx->result = NULL;
		return ;
	}
	last_index = ft_strlen(ctx->result) - 1;
	if (last_index < 0)
		last_index = 0;
	if (ctx->quote_state == NO_QUOTE && count_words(ctx->result[last_index], var_exp, target[ctx->i]) != 1)
		redir->ambiguous_flag = 1;
	ctx->result = ft_strjoin(ctx->result, var_exp, 0);
	free(var_exp);
}

static char	*expand_target(t_redir *redir, char *target, global_struct *g)
{
	t_expand_ctx	ctx;

	ctx.i = 0;
	ctx.quote_state = NO_QUOTE;
	ctx.result = ft_strdup("");
	if (!ctx.result)
		return (NULL);
	while (target[ctx.i])
	{
		if (target[ctx.i] == '\'' || target[ctx.i] == '"')
			handle_quote(target, &ctx);
		else if (target[ctx.i] == '$' && ctx.quote_state != SINGLE_QUOTE)
			handle_var_expand(redir, target, &ctx, g);
		else
			ctx.result = ft_strjoin_char(ctx.result, target[ctx.i++], 0);
		if (!ctx.result)
			return (NULL);
	}
	return (ctx.result);
}

int	expand_redir_target(t_redir *redir, global_struct *global_struct)
{
	char	*expanded;

	if (!redir || !redir->target)
		return (0);
	expanded = expand_target(redir, redir->target, global_struct);
	if (!expanded)
		return (0);
	free(redir->target);
	redir->target = expanded;
	return (1);
}
