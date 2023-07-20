#ifndef OUTILS_CLIENT_H
#define OUTILS_CLIENT_H

#include "structures.h"
/**
 * Cette fonction converti la longeur entière sour une chaine de 4 caractères
 * @param size entier a convertir
 * @param begin_index index du premier caractère
 * @param result la chaine ou inserer la longueur convertie 
*/
void to4char(long size, int begin_index, char *result);

/**
 * Cette focntion convertis une chaine en MAJUSCULE
 * @param str la chaine a mettre en majuscule
*/
void strupr(char *str);

/**
 * Cette fonction recupere la commande de la saisie
 * l utilisateur si il y en a. 
 * Il renvoit l'index de la derniere lettre de la commande.
*/
int get_cmd(char *msg, int client_cmd_nb, char **client_cmd_list);

/**
 * Cette fonction lit dans un tube la taille et la commande
 * et les met dans la structure entete passée en paramètres
 * @param read_desc descripteur pour la lecture dans le tube
 * @param en_tete la sructure qui va contenir la taille et la commande
*/
void read_entete_in_pipe(int read_desc, entete *en_tete);

/**
 * Cette fonction lit les information du client dans le tube
 * et les met dans la structure client
 * @param read_desc descripteur pour la lecture dans le tube
 * @param cl la structure qui contiendra l'id, le tube et le descripteur de lecture
*/
void fill_client(int read_desc, client *cl);

#endif