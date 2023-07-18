#ifndef STRUCTURES_SERVEUR_H
#define STRUCTURES_SERVEUR_H

struct client{
    int id;
    char *pseudo;
    char *pipe;
    int tube_write_desc;
};
typedef struct client client;

struct entete{
    int size;
    char cmd[5];
};
typedef struct entete entete;

#endif