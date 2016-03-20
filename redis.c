#include "redis.h"
#include <hiredis.h>
#include <stdlib.h>

static redisContext *g_redis = NULL;

#define DEPLOY_ID_KEY "ad_deploy_id"

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
