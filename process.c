#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <util.h>
#include "process.h"

#define BUFFER_SIZE 512

void catch_stdout(void *data, int len)
{
    ((char *)data)[len] = '\0';
    printf("stdout:%s", (char *)data);
}

void catch_stderr(void *data, int len)
{
    ((char *)data)[len] = '\0';
    printf("stderr:%s", (char *)data);
}

struct thread_params {
    int fd;
    output_cb cb;
};

static void *thread_proc(void *arg)
{
    struct thread_params *params = (struct thread_params *)arg;
    char buf[BUFFER_SIZE];
    size_t size;
    while ((size = read(params->fd, buf, BUFFER_SIZE)) > 0) {
        params->cb(buf, size);
    }
}

int run_process(char *cmd, int mode, output_cb stdoutcb, output_cb stderrcb)
{
    pid_t pid;

    int stdout_pfd[2];
    int stderr_pfd[2];

    int master, slave;
    openpty(&master, &slave, NULL, NULL, NULL);

    pipe(stdout_pfd);
    pipe(stderr_pfd);

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        login_tty(slave);
        close(master);

        close(stdout_pfd[0]);
        close(stderr_pfd[0]);

        //dup2(stdout_pfd[1], STDOUT_FILENO);
        if (mode == MODE_CATCH_BOTH || mode == MODE_CATCH_STDERR) {
            dup2(stderr_pfd[1], STDERR_FILENO);
        } else if (mode == MODE_CATCH_MERGE) {
            //dup2(stdout_pfd[1], STDERR_FILENO);
        } else if (mode == MODE_CATCH_STDOUT) {
            close(STDERR_FILENO);
        }

        int code = execl("/bin/sh", "sh", "-c", cmd, NULL);
        close(slave);
        exit(code);
    }
    close(stdout_pfd[1]);
    close(stderr_pfd[1]);

    close(slave);
    stdout_pfd[0] = master;

    struct thread_params stdout_p = {fd:stdout_pfd[0], cb:stdoutcb};
    struct thread_params stderr_p = {fd:stderr_pfd[0], cb:stderrcb};

    pthread_t stdout_t = 0, stderr_t = 0;
    if (mode == MODE_CATCH_BOTH) {
        pthread_create(&stdout_t, NULL, thread_proc, &stdout_p);
        pthread_create(&stderr_t, NULL, thread_proc, &stderr_p);
    } else if (mode == MODE_CATCH_STDOUT || mode == MODE_CATCH_MERGE) {
        pthread_create(&stdout_t, NULL, thread_proc, &stdout_p);
    } else if (mode == MODE_CATCH_STDERR) {
        pthread_create(&stderr_t, NULL, thread_proc, &stderr_p);
    }

    if (stdout_t)
        pthread_join(stdout_t, NULL);

    if (stderr_t)
        pthread_join(stderr_t, NULL);

    close(master);
    int status;
    waitpid(pid, &status, 0);
}


int main (int argc, char **argv)
{
    char *cmd = "./test";
    run_process(cmd, MODE_CATCH_STDERR, catch_stdout, catch_stderr);
    return 0;
}

