#ifndef STRUCTURE_CLIENT_H
#define STRUCTURE_CLIENT_H

#include <pthread.h>

typedef enum {
    TRUE,
    FALSE
} BOOLEAN;

struct entete{
    int size;
    char cmd[5];
};
typedef struct entete entete;

struct client{
    int id;
    char *pseudo;
    char *pipe;
    int tube_read_desc;
};
typedef struct client client;

typedef struct{
    char *sys_cmd; //ce que l utilisateur tape
    char *serv_cmd; //ce qu il faut envoyer au serveur
} command;

typedef struct data{
    int client_id;
    char *cmd;
    char *parameters;
    char *message;
} data;

//cette structure est utilisé pour le thread
typedef struct{
    client *cl;
    int *msg_sended;
    pthread_mutex_t *mutex;
} thread_data;

#endif