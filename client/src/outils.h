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
int get_cmd(char *msg, int client_cmd_nb, char **client_cmd_list);

/**
 * Cette fonction lit dans un tube la taille et la commande
 * et les met dans la structure entete passée en paramètres
*/
void read_entete_in_pipe(int read_desc, entete *en_tete);

#endif