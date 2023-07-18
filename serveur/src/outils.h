#ifndef OUTILS_CLIENT_H
#define OUTILS_CLIENT_H

#include "structures.h"
/**
 * Cette fonction converti la longeur entière sour une chaine de 4 caractères
*/
void to4char(long size, int begin_index, char *result);

/**
 * Cette fonction recupere la commande de la saisie
 * l utilisateur si il y en a. 
 * Il renvoit l'index de la derniere lettre de la commande.
*/
//NOT IMPLEMENTED YET
int get_cmd(char *msg, int client_cmd_nb, char **client_cmd_list);

/**
 * Cette fonction lit dans un tube la taille et la commande
 * et les met dans la structure entete passée en paramètres
*/
void read_entete_in_pipe(int read_desc, entete *en_tete);

/**
 * Cette fonction envoit un message a un utilisateur donné
*/
void send_to_client(client cl, char *message);

/**
 * Cette fonction crée un nouveau client et retourne son adresse
*/
client *create_client(int id, char *pseudo);

/**
 * Libere l espace allouer pour le stockage des clients
*/
void clean_all_client(int total_client, client **client_list);

/**
 * Libere l espace allouer pour le stockage du client avec cet identifiant
*/
void clean_client_by_id(int total_client, client **client_list, int id);

/**
 * Cette fonction recherche un client dans une liste de client a travers son pseudo
 * return 1 quand il est trouver
 * return 0 quand il est absent
*/
int is_client_pseudo_exist(int total_client, client **liste, char *pseudo);

/**
 * Cette fonction recherche un client dans une liste de client a travers son id
 * return un pointeur sur le client quand il est trouver
 * return NULL quand il est absent
*/
client *find_client_by_id(int total_client, client **liste, int id);

/**
 * Cette fonction recherche un client dans une liste de client a travers son pseudo
 * return un pointeur sur le client quand il est trouver
 * return NULL quand il est absent
*/
client *find_client_by_pseudo(int total_client, client **liste, char *pseudo);

#endif