#include "../minishell.h"

// void create_pipes(char **commande, t_env **env, t_data **data, t_hold **hold_vars, int rank)
// {
//     int pid;
//     int fd[2];

//     if (rank != 2)
//     {
//         if (pipe(fd) == -1)
//         {
//             perror("Error creating pipe");
//             exit(1);
//         }
//     }
//     if ((pid = fork()) == -1)
//     {
//         perror("Error creating new process");
//         exit(1);
//     }
//     if (pid == 0)
//     {
//         if (rank == 0)
//         {
//             dup2(fd[1], 1);
//             close(fd[1]);
//         }
//         else if (rank == 1)
//         {
//             dup2(fd[0], 0);
//             close(fd[0]);
//             dup2(fd[1], 1);
//             close(fd[1]);

//         }
//         else if (rank == 2)
//         {
//             dup2(fd[0], 0);
//             close(fd[0]);
//         }
//         exec_simple_commande(commande, env, data, hold_vars);
//         exit(0);
//     }
//     if (rank == 0)
//     {
//         close(fd[1]);
//     }
//     if (rank == 1)
//     {
//         close(fd[0]);
//         close(fd[1]);
//     }
//     if (rank == 2)
//     {
//         close(fd[0]);
//     }
// }

// void exec_with_pipes(t_env **envp, t_data **data, t_hold **hold_vars)
// {
//     t_data *temp;
//     int rank;
//     int status;

//     temp = *data;
//     rank = 0;
//     while (temp)
//     {
//         if (temp->next == NULL)
//             rank = 2;
//         create_pipes(temp->argumment, envp, data, hold_vars, rank);
//         temp = temp->next;
//         rank = 1;
//     }
//     while (wait(&status) > 0);
// }

int create_pipes(char **commande, t_env **env, t_data **data, t_hold **hold_vars, int fd_in, int fd_out)
{
    int pid;

    if ((pid = fork()) == -1)
    {
        perror("Error creating new process");
        exit(1);
    }
    if (pid == 0)
    {
        if (fd_in != 0)
        {
            dup2(fd_in, 0);
            close(fd_in);
        }
        if (fd_out != 1)
        {
            dup2(fd_out, 1);
            close(fd_out);
        }
        exit(exec_simple_commande(commande, env, data, hold_vars));
    }
    return(pid);
}

void exec_with_pipes(t_env **envp, t_data **data, t_hold **hold_vars)
{
    t_data *temp;
    int fd[2];
    int fd_in;
    int status;
    int pid;
    int exit_status;

    temp = *data;
    fd_in = 0;
    while (temp)
    {
        if (temp->next != NULL)
        {
            if (pipe(fd) == -1)
            {
                perror("Error creating pipe");
                exit(1);
            }
        }
        else
            fd[1] = 1;
        pid  = create_pipes(temp->argumment, envp, data, hold_vars, fd_in, fd[1]);
        if (fd_in != 0)
            close(fd_in);
        if (fd[1] != 1)
            close(fd[1]);
        fd_in = fd[0];
        temp = temp->next;
    }
    waitpid(pid, &exit_status, 0);
    while (wait(&status) > 0);
    exit_code = WEXITSTATUS(exit_status);
}
