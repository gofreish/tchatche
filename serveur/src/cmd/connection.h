#ifndef CONNECTION_H
#define CONNECTION_H

#include "../structures.h"

/**
 * Cette fonction cherche si le pseudo existe deja
 * Si oui il refuse la connexion
 * Si non il crée un nouveau client et augmengte le nombre total de client
*/
void new_connection(int read_pipe, int *last_free_id, entete en_tete, int *total_client, client **client_list);

/**
 * Cette fonction informe le client qu'il est connecté
 *Composition du message
 * 4 caractères pour la longueur
 * 4 caractères pour le type
 * 4 caractères pour le numero du client
 * un message
 * Exemple : 0021CSUC0001Connected
*/
void say_connected_to_client(client cl, char *message);

/**
 * Cette fonction informe le client que la connection a echouée
 * 4 caractères pour la longueur
 * 4 caractères pour le type
 * un message
 * Exemple : xxxxCBADPseudo existe deja
*/
void say_connection_failled_to_client(char *msg);

#endif