#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include "process.h"

#define BUFFER_SIZE 512

void catch_stdout(void *data, int len)
{
    ((char *)data)[len] = '\0';
    printf("stdout:%s, len:%d\n", (char *)data, len);
}

void catch_stderr(void *data, int len)
{
    ((char *)data)[len] = '\0';
    printf("stderr:%s, len:%d\n", (char *)data, len);
}

struct thread_params {
    int fd;
    output_cb cb;
};

static void *thread_proc(void *arg)
{
    struct thread_params *params = (struct thread_params *)arg;
    char buf[BUFFER_SIZE];
    while(read(params->fd, buf, BUFFER_SIZE) > 0)
    {
        params->cb(buf, strlen(buf));
    }
}

int mypopen(char *cmd, output_cb stdoutcb, output_cb stderrcb)
{
    pid_t pid;

    int stdout_pfd[2];
    int stderr_pfd[2];
    pipe(stdout_pfd);
    pipe(stderr_pfd);

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        close(stdout_pfd[0]);
        close(stderr_pfd[0]);

        dup2(stdout_pfd[1], STDOUT_FILENO);
        dup2(stderr_pfd[1], STDERR_FILENO);


        exit(execl("/bin/sh", "sh", "-c", cmd, NULL));
    }

    close(stdout_pfd[1]);
    close(stderr_pfd[1]);

    struct thread_params stdout_p = {fd:stdout_pfd[0], cb:stdoutcb};
    struct thread_params stderr_p = {fd:stderr_pfd[0], cb:stderrcb};

    pthread_t stdout_t, stderr_t;

    pthread_create(&stdout_t, NULL, thread_proc, &stdout_p);
    pthread_create(&stderr_t, NULL, thread_proc, &stderr_p);

    pthread_join(stdout_t, NULL);
    pthread_join(stderr_t, NULL);

    int status;
    waitpid(pid, &status, 0);
}

/*
int main (int argc, char **argv)
{
    char *cmd = "./test";
    mypopen(cmd, catch_stdout, catch_stderr);
    return 0;
}
*/
