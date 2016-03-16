#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <event.h>
#include <evhttp.h>
#include "daemonize.h"
#include "worker.h"

static char *l_host     = "0.0.0.0";
static short l_port     = 8080;
static int daemon_mode  = 0;
static char *redis_addr = NULL;
static short redis_port = 6379;

#define fatal(fmt, ...) do {             \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    exit(1);                             \
} while (0)

#define ROUTE_END \
    {NULL, 0, NULL}


typedef void (*exec_handler)(struct evhttp_request *req, void *arg);

struct Route {
    char *prefix;
    enum evhttp_cmd_type type;
    exec_handler callback;
};

static long long realtime()
{
    struct timeval tv;
    long long retval;

    if (gettimeofday(&tv, NULL) == -1) {
        return 0LL;
    }

    retval = (long long)tv.tv_sec * 1000ULL
           + (long long)tv.tv_usec / 1000ULL;

    return retval;
}



void usage()
{
    printf("Usage: auto deploy [options]\n"
           "Options:\n"
           "    -r <host>        redis host and port\n"
           "    -l <addr>        IP/host listen to\n"
           "    -p <port>        port number\n"
           "    -d               run at daemon mode\n");
    exit(0);
}

static const struct option options[] = {
    {"redis",  2, NULL, 'r'},
    {"listen", 2, NULL, 'l'},
    {"port",   2, NULL, 'p'},
    {"daemon", 0, NULL, 'd'},
    {"help",   0, NULL, 'h'},
};

static const struct Route route_table[] = {
    {"/api/deploy", EVHTTP_REQ_GET, deploy_show},
    {"/api/deploy", EVHTTP_REQ_POST, deploy_create},

    {"/api/fire", EVHTTP_REQ_GET, fire_show},
    {"/api/fire", EVHTTP_REQ_POST, fire_create},

    {"/api/log", EVHTTP_REQ_GET, log_show},
    {"/api/status", EVHTTP_REQ_GET, status_show},
    ROUTE_END
};

void send_404_page(struct evhttp_request *req)
{
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "not found");
    evhttp_send_reply(req, HTTP_NOTFOUND, "not found", buf);
    evbuffer_free(buf);
}

void request_handler(struct evhttp_request *req, void *arg)
{
    // resolve route
    char *decode_uri = strdup((char *)evhttp_request_uri(req));
    fprintf(stdout , "request: %s\n", decode_uri);
    for (; ;) {
        static int i = 0;
        if (route_table[i].prefix == NULL && route_table[i].type == 0) {
            break;
        }

        if (strncmp(route_table[i].prefix, decode_uri, strlen(route_table[i].prefix)) == 0) {
            route_table[i].callback(req, arg);
            goto end;
        }
        i++;
    }
    send_404_page(req);
    fprintf(stderr, "404:%s\n", decode_uri);

end:
    free(decode_uri);
    return;
}

int parse_redis(char *host)
{
    struct hostent *he;
    struct in_addr **addrs;
    char *port;
    int i;

    if ((port = strchr(host, ':'))) {
        *port++ = '\0';
    }

    he = gethostbyname(host);
    if (!he) {
        return -1;
    }

    addrs = (struct in_addr **)he->h_addr_list;

    for(i = 0; addrs[i] != NULL; i++) {
        redis_addr = inet_ntoa(*addrs[i]);
        break;
    }

    if (port) {
        redis_port = atoi(port);
    }

    return 0;
}

void parse_options(int argc, char **argv)
{
    int opt, i;

    while ((opt = getopt_long(argc, argv,
                                 "r:l:p:dh", options, &i)) != -1)
    {
        switch (opt) {
        case 'r':
            parse_redis(optarg);
            break;
        case 'l':
            l_host = strdup(optarg);
            break;
        case 'p':
            l_port = atoi(optarg);
            break;
        case 'd':
            daemon_mode = 1;
            break;
        case 'h':
        case '?':
            usage();
        }
    }
}

int main(int argc, char **argv)
{
    struct evhttp *httpd;

    parse_options(argc, argv);

    if (daemon_mode) {
        daemonize();
    }

    event_init();

    httpd = evhttp_start(l_host, l_port);
    evhttp_set_gencb(httpd, request_handler, NULL);
    event_dispatch();

    return 0;
}
