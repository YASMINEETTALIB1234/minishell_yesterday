/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 01:50:59 by oafidi            #+#    #+#             */
/*   Updated: 2025/08/12 19:16:41 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig = 0;

int init_global_struct(global_struct *global_struct, char **env)
{
    global_struct->cmds = NULL;
    exit_status_set(0);
    global_struct->tokens = NULL;
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
		free_env_list(global_struct->env);
		ft_putstr_fd("exit\n", 2);
		exit(*exit_status_get());
    }
    else if (*input)
        add_history(input);
    return (input);
}

int minishell_loop(char **env)
{
    global_struct   global_struct;
    char		    **heredoc;
    char            *input;

    if ((!init_global_struct(&global_struct, env)) || (!isatty(0) || !isatty(1)))
        return (1);
    while (1)
    {
        setup_signals();
        input = ft_readline(&global_struct);
        global_struct.tokens = lexer(input);
        global_struct.cmds = parser(global_struct.tokens, &global_struct);
        if (!global_struct.cmds)
        {
            free_token_list(global_struct.tokens);
            free(input);
            continue ;
        }
        heredoc = herdoc_init(global_struct.tokens);
        //print_cmd_list(global_struct.cmds);
        execute_command(global_struct.cmds,  heredoc, &global_struct.env, count_heredocs(global_struct.tokens));
        free_token_list(global_struct.tokens);
        free_command_list(global_struct.cmds);
        free(input);
    }
    return (free_env_list(global_struct.env), 1);
}

int main(int argc, char **argv, char **env)
{
    (void)argv;
    if (argc != 1)
        return (ft_putstr_fd("Invalid Arguments !\n", 2), 1);
    return (minishell_loop(env));
}
