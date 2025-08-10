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

void execute_command(t_cmd *cmd_head, char **herdocs, t_env **env, int herdocs_count)
{
    t_cmd *cmd = cmd_head;
    int pipe_fd[2];
    pid_t pid;
    int input_fd = 0;
    pid_t last_pid = -1;
    int heredoc_index = 0;

    if (!cmd)
        return;
    // herdocs_prepare(cmd_head, herdocs);
    // if (!cmd->args || !cmd->args[0]) {
    //     herdocs_clean(herdocs, herdocs_count);
    //     return;
    // }
    if (herdocs_prepare(cmd_head, herdocs) || (!cmd || !cmd->args || !cmd->args[0]))
    {
        herdocs_clean(herdocs, herdocs_count);
        return ;
    }

    int size  = size_list(cmd);
    if (!cmd)
        return ;
    if (size == 1 && cmd->args[0] && check_builtin(cmd->args[0]))
    {
        int	save_in;
        int	save_out;

        save_in = dup(STDIN_FILENO);
        save_out = dup(STDOUT_FILENO);
        if (handle_redirections(cmd) == -1)
        {
            // printf("before redir redirection %d\n", *(exit_status_get()));
			exit_status_set(1);
			// printf("after redir redirection %d\n", *(exit_status_get()));
            write(1, "minishell: No such file or directory\n", 38);
        }
        else if (check_builtin(cmd->args[0]))
        {
            // printf("before redir buil %d\n", *(exit_status_get()));
            exit_status_set(0);
            // printf("after redir buil %d\n", *(exit_status_get()));
            execute_builtin(cmd->args, env);
        }
        dup2(save_in, STDIN_FILENO);
        close(save_in);
        dup2(save_out, STDOUT_FILENO);
        close(save_out);
        cmd=cmd->next;
    }
    while (cmd)
    {
        if (cmd->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            // printf("before redir %d\n", *(exit_status_get()));
			exit_status_set(1);
			// printf("after redir %d\n", *(exit_status_get()));
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (!cmd->args || !cmd->args[0])
            {
                ft_putstr_fd("minishell: command not found\n", 2);
                exit(127);
            }
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            if (input_fd != 0)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (cmd->next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            if (handle_redirections(cmd) == -1)
            {
                exit_status_set(1);
                write(1, "minishell: No such file or directory\n", 38);
                exit(EXIT_FAILURE);
            }
            if (check_builtin(cmd->args[0]))
            {
                exit_status_set(0);
                exit(execute_builtin(cmd->args, env));
            }
            char *path = get_env_value(*env, "PATH");
            int fd = -1;//for not directory
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
                    printf("yessssss\n");
                    write(2, "minishell: ", 11);
                    write(2, cmd->args[0], ft_strlen(cmd->args[0]));
                    write(2, ": ", 2);
                    perror("");
                    if (errno == 2)
                        exit(127);
                    else if (errno == 13 || errno == 20)
                        exit(126);
                    
                }
                else
                {
                    char **env_arr = env_to_array(cmd->env);
                    execve(cmd->args[0], cmd->args, env_arr);
                    char *tmp = ft_strjoin_execution("minishell: ",  cmd->args[0]);
                    if (!tmp)
                        return ;
                    perror(tmp);
                    free(tmp);
                    free_strarray(env_arr);
                    exit(126);
                }
            }
            char **dirs = ft_split(path, ':');
            if (!dirs)
                exit(EXIT_FAILURE);
            int i;
            char *full;

            i = 0;
            while (dirs[i])
            {
                full = ft_strjoin3(dirs[i], "/", cmd->args[0]);
                if (!full)
                {
                    i++;
                    continue;
                }
                if (access(full, X_OK) == 0)
                {
                    char **env_arr = env_to_array(cmd->env);
                    exit_status_set(0);
                    execve(full, cmd->args, env_arr);
                    free_strarray(env_arr);
                }
                free(full);
                i++;
            }
            clean_string_array(dirs);
            write(2, "minishell: ", 11);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, ": command not found\n", 21);
            exit(127);
        }
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

