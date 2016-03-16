#include "redis.h"
#include <hiredis.h>
#include <stdlib.h>

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
