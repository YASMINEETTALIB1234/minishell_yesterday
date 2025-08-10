#include "../../includes/minishell.h"

void    free_token_list(t_token *tokens)
{
    t_token *next;
    
    while (tokens)
    {
        next = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = next;
    }
}

void    free_redirections(t_redir *redirs)
{
    t_redir *next;
    
    while (redirs)
    {
        next = redirs->next;
        if (redirs->target)
            free(redirs->target);
        free(redirs);
        redirs = next;
    }
}

void	free_command(t_cmd *cmd)
{
	free(cmd->args);
	free_redirections(cmd->redirs);
    free(cmd->line);
	free(cmd);
}

void    free_command_list(t_cmd *commands)
{
    t_cmd *next;
    
    while (commands)
    {
        next = commands->next;
        free_command(commands);
        commands = next;
    }
}
