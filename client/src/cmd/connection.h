#ifndef CONNECTION_CLIENT_H
#define CONNECTION_CLIENT_H

/**
 * Cette fonction demande une 
 * nouvelle connection au serveur
 * Format du message envoyer : 
 *  ("%d CONN %s", taille_message, pseudo)
 * return 0 si echec
 * return 1 si succès
*/
void ask_connection(char *pseudo);

/**
 * Cette fonction attend la reponse du serveur et fait un affichage
*/
void wait_connection_response(int *id_client);

#endif