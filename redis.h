#ifndef __REDIS_H__
#define __REDIS_H__

void redis_init(char *host, int port);

void redis_free();

int redis_hash_set(char *key, char *field, char *value);
int redis_append(char *key, char *value);

int get_deploy_id();

#endif
