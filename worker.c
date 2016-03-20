#include <event.h>
#include <evhttp.h>
#include <string.h>
#include <stdlib.h>
#include "worker.h"
#include "smart_str.h"
#include "cJSON.h"
#include "utils.h"
#include "redis.h"

#define get_buffer_from_req(body, req) do { \
    smart_str *__str = (smart_str *)body; \
    struct evbuffer *__buff = evhttp_request_get_input_buffer(req); \
    size_t __size = evbuffer_get_length(__buff); \
    smart_str_appendl(__str, evbuffer_pullup(__buff, -1), __size); \
    smart_str_0(__str); \
 } while(0)

static void output(struct evhttp_request *req, char *content, int code)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", content);
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=utf-8");
    evhttp_send_reply(req, code, NULL, buf);
    evbuffer_free(buf);
}

#define send_bad_request(req, msg) \
    output(req, msg, HTTP_BADREQUEST)
#define send_normal_request(req, msg) \
    output(req, msg, HTTP_OK)
#define send_internal_request(req, msg) \
    output(req, msg, HTTP_INTERNAL)

void deploy_show(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/deploy";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show deploy list
    } else {
        if (decode_uri[orig_uri_len] != '/' || strlen(decode_uri) == orig_uri_len + 1) {
            // todo: send 400 page
        }
        char *task_id = strdup(decode_uri + orig_uri_len + 1);
        // todo
        free(task_id);
    }
    free(decode_uri);
}

void deploy_create(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/deploy";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    smart_str body = {0};
    if (strcmp(decode_uri, orig_uri) == 0) {
        get_buffer_from_req(&body, req);

        cJSON *json, *json_ver, *json_author;
        json = cJSON_Parse(body.c);
        if (!json) {
            send_bad_request(req, "params error");
            goto end;
        }

        json_ver = cJSON_GetObjectItem(json, "version");
        if (!json_ver) {
            send_bad_request(req, "params error");
            goto end;
        }

        json_author = cJSON_GetObjectItem(json, "author");
        if (!json_author) {
            send_bad_request(req, "params error");
            goto end;
        }

        // return json
        char fmt_time[20] = { 0 };
        get_format_timestamp(fmt_time, sizeof(fmt_time));

        int deploy_id = get_deploy_id();
        if (-1 == deploy_id) {
            send_internal_request(req, "get deploy id failure");
            goto end;
        }

        cJSON *json_ret = cJSON_CreateObject();
        cJSON_AddItemToObject(json_ret, "id", cJSON_CreateNumber(deploy_id));
        cJSON_AddItemToObject(json_ret, "version", cJSON_CreateString(json_ver->valuestring));
        cJSON_AddItemToObject(json_ret, "author", cJSON_CreateString(json_author->valuestring));
        cJSON_AddItemToObject(json_ret, "log_url", cJSON_CreateString("http://xxx.xx"));
        cJSON_AddItemToObject(json_ret, "create_at", cJSON_CreateString(fmt_time));
        cJSON_AddItemToObject(json_ret, "status", cJSON_CreateString("running"));
        char *json_encoded = cJSON_Print(json_ret);

        send_normal_request(req, json_encoded);

        free(json_encoded);
        cJSON_Delete(json);
        cJSON_Delete(json_ret);
    } else {
        // todo: send 400 page
    }
end:
    smart_str_free(&body);
    free(decode_uri);
}

void fire_show(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/fire";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show fire list
    } else {
        if (decode_uri[orig_uri_len] != '/' || strlen(decode_uri) == orig_uri_len + 1) {
            // todo: send 400 page
        }
        char *task_id = strdup(decode_uri + orig_uri_len + 1);
        // todo
        free(task_id);
    }
    free(decode_uri);

}

void fire_create(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/fire";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show deploy list
    } else {
        // todo: send 400 page
    }
    free(decode_uri);
}

void log_show(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/fire";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show deploy list
    } else {
        // todo: send 400 page
    }
    free(decode_uri);
}

void status_show(struct evhttp_request *req, void *arg)
{
    const char *orig_uri = "/api/log";
    int orig_uri_len = strlen(orig_uri);
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show deploy list
    } else {
        // todo: send 400 page
    }
    free(decode_uri);
}

