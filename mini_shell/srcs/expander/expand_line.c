#include "../../includes/minishell.h"

char    *expand(char *line, global_struct *global_struct, int start_quote_state)
{
    char	*result;
    int		i;
    int		quote_state;

    if (!line || !global_struct)
        return (NULL);
    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    quote_state = start_quote_state;
    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"'))
        {
            result = ft_strjoin_char(result, line[i], 0);
            update_quote_state(line[i], &quote_state);
            i++;
        }
        else if (line[i] == '$' && quote_state != SINGLE_QUOTE)
        {
            char *var_expansion = expand_variable(line, &i, global_struct->env,
                                                  *exit_status_get());
            if (var_expansion)
            {
                result = ft_strjoin(result, var_expansion, 0);
                free(var_expansion);
            }
        }
        else
        {
            result = ft_strjoin_char(result, line[i], 0);
            i++;
        }
        if (!result)
            return (NULL);
    }
    return (result);
}

static int is_export_case(char *line, global_struct *global_struct)
{
    char *expanded;
    char *temp;
    int i = 0;
    int ret = 0;

    if (!line || !global_struct)
        return (0);
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
        ret = 1;
    free(expanded);
    return (ret);
}

void	expand_line(t_cmd *cmd, global_struct *global_struct)
{
    char	*expanded;

    if (!cmd || !cmd->line || !global_struct)
        return ;
    if (is_export_case(cmd->line, global_struct))
    {
        expanded = expand_export(cmd->line, global_struct);
        free(cmd->line);
    }
    else
    {
        expanded = expand(cmd->line, global_struct, NO_QUOTE);
		free(cmd->line);
    }
    cmd->line = expanded;
}