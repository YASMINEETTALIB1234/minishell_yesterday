/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:50:59 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/10 13:20:08 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig = 0;

int init_global_struct(global_struct *global_struct, char **env)
{
    global_struct->cmds = 0;
    exit_status_set(0);
    global_struct->tokens = 0;
    global_struct->env = copy_environment(env);
    if (!global_struct->env)
        return (ft_putstr_fd("Failed to copy environment!\n", 2), 0);
    return (1);
}

char    *ft_readline(global_struct *global_struct)
{
    char    *input;

    input = readline("minishell-$ ");
    if (g_sig == 2)
        g_sig = 0;
    if (!input)
    {
        rl_clear_history();
		free_environment(&global_struct->env);
		ft_putstr_fd("exit\n", 2);
		exit(*exit_status_get());
    }
    else
        add_history(input);
    return (input);
}

void    minishell_loop(char **env)
{
    global_struct   global_struct;
    char		    **heredoc;
    char            *input;

    if ((!init_global_struct(&global_struct, env)) || (!isatty(0) || !isatty(1)))
        exit(1);
    while (1)
    {
        setup_signals();
        input = ft_readline(&global_struct);
        global_struct.tokens = lexer(input);
        global_struct.cmds = parser(global_struct.tokens, &global_struct);
        heredoc = herdoc_init(global_struct.tokens); // you need to protect malloc
        // print_cmd_list(global_struct.cmds);
        // heredoc = NULL;
        execute_command(global_struct.cmds,  heredoc, &global_struct.env, count_heredocs(global_struct.tokens));
        // i need to free everything
        free(input);
    }
}

int main(int argc, char **argv, char **env)
{
    (void)argv;
    if (argc != 1)
        return (ft_putstr_fd("Invalid Arguments !\n", 2), 1);
    minishell_loop(env);
}
