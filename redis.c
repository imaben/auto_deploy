#include "redis.h"
#include <hiredis.h>
#include <stdlib.h>
#include <string.h>
#include "deploy.h"
#include "smart_str.h"

static redisContext *g_redis = NULL;

void redis_init(char *host, int port)
{
    char *msg;

    if (!host) {
        msg = "redis host have not set, using `-r' option";
        goto error;
    }

    g_redis = redisConnect(host, port);
    if (g_redis == NULL || g_redis->err) {
        msg = "Can not connect to redis server";
        goto error;
    }
    return;

error:
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void redis_free()
{
    if (g_redis) {
        redisFree(g_redis);
    }
}

int redis_hash_set(char *key, char *field, char *value)
{
    if (!key || !field || !value) {
        return -1;
    }

    int r = -1;
    redisReply *reply = NULL;
    smart_str command = { 0 };
    smart_str_appendl(&command, "HSET ", strlen("HSET "));
    smart_str_appendl(&command, key, strlen(key));
    smart_str_appendl(&command, " ", 1);
    smart_str_appendl(&command, field, strlen(field));
    smart_str_appendl(&command, " \"", 2);
    smart_str_appendl(&command, value, strlen(value));
    smart_str_appendl(&command, "\"", 1);
    smart_str_0(&command);
    reply = redisCommand(g_redis, command.c);
    if (g_redis->err != 0) {
        // todo write warining log
        fprintf(stderr, "redis hash set failure, error:%d, command:%s\n", g_redis->err, command.c);
    } else {
        r = (int)reply->integer;
    }
    smart_str_free(&command);

    freeReplyObject(reply);
    return r;
}

int redis_append(char *key, char *value)
{
    if (!key || !value) {
        return -1;
    }

    int r = -1;
    redisReply *reply = NULL;
    smart_str command = { 0 };
    smart_str_appendl(&command, "APPEND ", strlen("APPEND "));
    smart_str_appendl(&command, key, strlen(key));
    smart_str_appendl(&command, " ", 1);
    smart_str_appendl(&command, value, strlen(value));
    smart_str_0(&command);
    reply = redisCommand(g_redis, command.c);
    smart_str_free(&command);
    if (g_redis->err != 0)
    {
        // todo write warining log
        fprintf(stderr, "redis append failure\n");
    } else {
        r = (int)reply->integer;
    }

    freeReplyObject(reply);
    return r;
}

int get_deploy_id()
{
    redisReply *reply = NULL;
    int deploy_id = -1;
    char command[32] = { 0 };
    sprintf(command, "INCR %s", DEPLOY_ID_KEY);
    reply = redisCommand(g_redis, command);

    if (g_redis->err != 0
            || reply == NULL
            || reply->type != REDIS_REPLY_INTEGER)
    {
        // todo write warining log
        fprintf(stderr, "get deploy key failure\n");
    } else {
        deploy_id = (int)reply->integer;
    }

    freeReplyObject(reply);
    return deploy_id;
}
