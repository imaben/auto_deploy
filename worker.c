#include <event.h>
#include <evhttp.h>
#include "worker.h"

void deploy_show(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "deploy show");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void deploy_create(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "deploy_create");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void fire_show(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "fire show");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void fire_create(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "fire_create");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void log_show(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "log show");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void status_show(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "status show");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

