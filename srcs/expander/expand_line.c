#include "../../includes/minishell.h"

static void	handle_var_expand(char *target, t_expand_ctx *ctx, global_struct *g)
{
	char	*var_exp;

	var_exp = expand_variable(target, &ctx->i, g->env);
	if (!var_exp)
    {
        free(ctx->result);
        ctx->result = NULL;
        return ;
    }
	ctx->result = ft_strjoin(ctx->result, var_exp, 0);
	free(var_exp);
}

char    *expand(char *line, global_struct *global_struct, int start_quote_state)
{
    t_expand_ctx    ctx;

	ctx.i = 0;
	ctx.quote_state = start_quote_state;
	ctx.result = ft_strdup("");
    if (!ctx.result)
		return (NULL);
    while (line[ctx.i])
    {
        if ((line[ctx.i] == '\'' || line[ctx.i] == '"'))
        {
            ctx.result = ft_strjoin_char(ctx.result, line[ctx.i], 0);
            update_quote_state(line[(ctx.i)++], &(ctx.quote_state));
        }
        else if (line[ctx.i] == '$' && ctx.quote_state != SINGLE_QUOTE)
            handle_var_expand(line, &ctx, global_struct);
        else
            ctx.result = ft_strjoin_char(ctx.result, line[ctx.i++], 0);
        if (!ctx.result)
            return (NULL);
    }
    return (ctx.result);
}

static int is_export_case(char *line, global_struct *global_struct)
{
    char    *expanded;
    int     result;
    char    *temp;
    int     i;

    i = 0;
    result = 0;
    temp = expand(line, global_struct, NO_QUOTE);
    if (!temp)
        return (0);
    expanded = remove_quotes(temp);
    free(temp);
    if (!expanded)
        return (0);
    while (expanded[i] && is_space(expanded[i]))
        i++;
    if (ft_strncmp(expanded + i, "export", 6) == 0 && (is_space(expanded[i + 6]) || expanded[i + 6] == '\0'))
        result = 1;
    free(expanded);
    return (result);
}

int	expand_line(t_cmd *cmd, global_struct *global_struct)
{
    char	*expanded;

    if (!cmd->line)
        return (1);
    if (is_export_case(cmd->line, global_struct))
    {
        expanded = expand_export(cmd->line, global_struct);
        if (!expanded)
            return (0);
        free(cmd->line);
    }
    else
    {
        expanded = expand(cmd->line, global_struct, NO_QUOTE);
        if (!expanded)
            return (0);
		free(cmd->line);
    }
    cmd->line = expanded;
    return (1);
}