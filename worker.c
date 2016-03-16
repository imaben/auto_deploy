#include <event.h>
#include <evhttp.h>
#include <string.h>
#include <stdlib.h>
#include "worker.h"

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
    if (strcmp(decode_uri, orig_uri) == 0) {
        // todo: show deploy list
    } else {
        // todo: send 400 page
    }
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

