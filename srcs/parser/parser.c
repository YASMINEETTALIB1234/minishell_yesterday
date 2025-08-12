/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 03:42:38 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/09 19:31:58 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// just for test
static const char *token_type_to_str(t_token_type type)
{
    if (type == TOKEN_WORD) return "WORD";
    if (type == TOKEN_PIPE) return "PIPE";
    if (type == TOKEN_REDIR_IN) return "REDIR_IN";
    if (type == TOKEN_REDIR_OUT) return "REDIR_OUT";
    if (type == TOKEN_APPEND) return "APPEND";
    if (type == TOKEN_HEREDOC) return "HEREDOC";
    return "UNKNOWN";
}

void print_redir_list(t_redir *redir)
{
    int i = 0;
    while (redir)
    {
        printf("    Redir %d: type = %-10s | target = \"%s\" | fd = %d | ambiguous = %d | expand = %d\n",
            i,
            token_type_to_str(redir->type),
            redir->target ? redir->target : "(null)",
            redir->fd,
            redir->ambiguous_flag,
            redir->should_expand);
        redir = redir->next;
        i++;
    }
}

void print_cmd_list(t_cmd *cmd)
{
    int i = 0, j= 0;
    while (cmd)
    {
        printf("Command %d:\n", i);
        printf("  line: \"%s\"\n", cmd->line ? cmd->line : "(null)");
        printf("  args:");
        if (cmd->args)
        {
            j = 0;
            while (cmd->args[j])
            {
                printf(" \"%s\", %zu", cmd->args[j], ft_strlen(cmd->args[j]));
                j++;
            }
        }
        printf("\n");
        printf("  Redirections:\n");
        print_redir_list(cmd->redirs);
        cmd = cmd->next;
        i++;
    }
}
// just for test end

static int	parse_word_token(t_cmd *cmd, t_token **token_ptr)
{
	cmd->line = ft_strjoin(cmd->line, (*token_ptr)->value, ' ');
	*token_ptr = (*token_ptr)->next;
	return (cmd->line != NULL);
}

static int	parse_redirection_token(t_cmd *cmd, t_token **token_ptr)
{
	t_redir			*redir;
	t_token_type    redir_type;

	redir_type = (*token_ptr)->type;
	*token_ptr = (*token_ptr)->next;
	redir = create_redirection(redir_type, (*token_ptr)->value);
	if (!redir)
		return (0);
	add_redirection_to_cmd(cmd, redir);
	*token_ptr = (*token_ptr)->next;
	return (1);
}

static t_cmd	*parse_single_command(t_token **token_ptr, global_struct *global_struct)
{
	t_cmd	*cmd;

	cmd = init_command_parsing(global_struct);
	if (!cmd)
		return (NULL);
	while (*token_ptr && (*token_ptr)->type != TOKEN_PIPE)
	{
		if ((*token_ptr)->type == TOKEN_WORD)
		{
			if (!parse_word_token(cmd, token_ptr))
				return (free_command(cmd), NULL);
		}
		else if ((*token_ptr)->type >= TOKEN_REDIR_IN
			&& (*token_ptr)->type <= TOKEN_HEREDOC)
		{
			if (!parse_redirection_token(cmd, token_ptr))
				return (free_command(cmd), NULL);
		}
	}
	return (cmd);
}

static t_cmd   *parse_pipeline(t_token **token_ptr, global_struct *global_struct)
{
    t_cmd   *head;
    t_cmd   *tail;
    t_cmd   *new_cmd;
    
    head = NULL;
    tail = NULL;
    while (token_ptr && *token_ptr)
    {
        new_cmd = parse_single_command(token_ptr, global_struct);
        if (!new_cmd)
            return (free_command_list(head), NULL);
        if (!head)
        {
            head = new_cmd;
            tail = new_cmd;
        }
        else
        {
            tail->next = new_cmd;
            tail = new_cmd;
        }
        if (*token_ptr && (*token_ptr)->type == TOKEN_PIPE)
            *token_ptr = (*token_ptr)->next;
    }
    return (head);
}

t_cmd   *parser(t_token *tokens, global_struct *global_struct)
{
	t_cmd *head;

    if (!tokens || !validate_syntax(tokens)){
		return (NULL);
	}
	head = parse_pipeline(&tokens, global_struct);
    if (!head)
		return (NULL);
    return (expand_pipeline(head, global_struct));
}
