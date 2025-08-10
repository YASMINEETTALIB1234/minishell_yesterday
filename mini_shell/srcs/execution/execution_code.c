/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_code.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yettalib <yettalib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 19:25:24 by yettalib          #+#    #+#             */
/*   Updated: 2025/08/09 19:16:15 by yettalib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void free_strarray(char **arr)
{
    size_t i = 0; 
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int size_list(t_cmd *head)
{
    int i;

    i = 0;
    if (!head)
        return (0);
    while (head)
    {
        i++;
        head = head->next;
    }
    return (i);
}

int is_single_builtin(t_cmd *cmd)
{
    return size_list(cmd) == 1 && cmd->args && check_builtin(cmd->args[0]);
}

void run_single_builtin(t_cmd *cmd, t_env **env)
{
    int save_in = dup(STDIN_FILENO);
    int save_out = dup(STDOUT_FILENO);

    if (handle_redirections(cmd) == -1)
        exit_status_set(1);
    else
    {
        exit_status_set(0);
        execute_builtin(cmd->args, env);
    }
    dup2(save_in, STDIN_FILENO);
    dup2(save_out, STDOUT_FILENO);
    close(save_in);
    close(save_out);
}

void try_exec_in_path(t_cmd *cmd, char *path)
{
    char **dirs = ft_split(path, ':');
    if (!dirs)
        exit(EXIT_FAILURE);
    for (int i = 0; dirs[i]; i++)
    {
        char *full = ft_strjoin3(dirs[i], "/", cmd->args[0]);
        if (full && access(full, X_OK) == 0)
        {
            char **env_arr = env_to_array(cmd->env);
            execve(full, cmd->args, env_arr);
            free_strarray(env_arr);
        }
        free(full);
    }
    clean_string_array(dirs);
    write(2, "minishell: ", 11);
    write(2, cmd->args[0], ft_strlen(cmd->args[0]));
    write(2, ": command not found\n", 21);
    exit(127);
}

void execute_external(t_cmd *cmd, t_env **env)
{
    char *path = get_env_value(*env, "PATH");
    int fd = -1;

    if (!path || ft_strchr(cmd->args[0], '/'))
    {
        fd = open(cmd->args[0], O_DIRECTORY);
        if (fd != -1)
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": Is a directory\n", 18);
            exit(126);
        }
        else if (access(cmd->args[0], X_OK) == -1)
        {
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": ", 2);
            perror("");
            exit(errno == 2 ? 127 : 126); // ternary operator forbidden
        }
        char **env_arr = env_to_array(cmd->env);
        printf("lkjhefd\n");
        execve(cmd->args[0], cmd->args, env_arr);
        printf(" hell sdfgh\n");
        perror(cmd->args[0]);
        free_strarray(env_arr);
        exit(126);
    }
    try_exec_in_path(cmd, path);
}

void run_child(t_cmd *cmd, int input_fd, int pipe_fd[2], t_env **env) // to verify
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (input_fd != 0)
        dup2(input_fd, STDIN_FILENO);
    if (cmd->next)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    if (cmd->redirs)
    {
        
        if (handle_redirections(cmd) == -1)
        {
            exit_status_set(1);
            exit(EXIT_FAILURE);
        }
    }
    if (cmd->args)
    {
        if (check_builtin(cmd->args[0]))
        {
            exit_status_set(0);
            exit(execute_builtin(cmd->args, env));
        }
        execute_external(cmd, env);
    }
    exit(0);
}

void run_pipeline(t_cmd *cmd, char **herdocs, t_env **env)
{
    int input_fd = 0;
    int pipe_fd[2];
    pid_t last_pid = -1;
    int heredoc_index = 0;

    while (cmd)
    {
        if (cmd->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit_status_set(1);
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
            run_child(cmd, input_fd, pipe_fd, env);
        else
        {
            if (input_fd != 0)
                close(input_fd);
            if (cmd->next)
            {
                close(pipe_fd[1]);
                input_fd = pipe_fd[0];
            }
            else
                input_fd = 0;
            last_pid = pid;
            cmd = cmd->next;
        }
    }
    if (input_fd != 0)
        close(input_fd);
    exit_status_set(ft_wait(&last_pid));
    herdocs_clean(herdocs, heredoc_index);
}

void execute_command(t_cmd *cmd_head, char **herdocs, t_env **env, int herdocs_count)
{
    if (!cmd_head)
        return;
    if (herdocs && herdocs_prepare(cmd_head, herdocs)) //herdocs_prepare(cmd_head, herdocs) || !cmd_head->args || !cmd_head->args[0]   had condition makhedamach 7itach makatkounch
    {
        herdocs_clean(herdocs, herdocs_count);
        return;
    }
    if (is_single_builtin(cmd_head))
    {
        run_single_builtin(cmd_head, env);
        return;
    }
    run_pipeline(cmd_head, herdocs, env);
}
