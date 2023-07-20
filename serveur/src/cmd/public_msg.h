#ifndef PUBLIC_MSG_SERVEUR_H
#define PUBLIC_MSG_SERVEUR_H

#include "../structures.h"

void broadcast_msg(int serveur_tube_read_desc, entete en_tete, int total_client, client **liste);

#endif