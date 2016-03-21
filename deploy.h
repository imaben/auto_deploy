#ifndef __DEPLOY_H__
#define __DEPLOY_H__

typedef struct {
    char *listen_host;
    short listen_port;
    char *redis_addr;
    short redis_port;
    char *config_path;
    int daemon_mode;
} deploy_setting_t;

extern deploy_setting_t *setting;

#endif
