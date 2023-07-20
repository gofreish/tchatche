#ifndef DATA_CLIENT_H
#define DATA_CLIENT_H

#include "structures.h"

/**
 * Cette fonction libere les espaces allouer pour la création d une structure data
*/
void clean_data(data *d);

/**
 * Cette fonction lit ce que l'utilisateur a saisi et appele la fonction repondra
 * a la demande du client
 * @param input la chaine saisie par l'utilisateur
 * @param size la taille de la chaine
*/
data *prepare_data(char *input, client cl, command *all_cmd);

/**
 * Cette fonction retourne un pointeur sur une chaine contenant tout les 
 * carectères a gauche de charactère c. Cela représente la commande(privé de ce caractère).
 * @param src la chaine dans laquelle on recherche
 * @param c le caractère a chercher
 * @return un pointeur vers la chaine contenant tous les caractères de la commande
*/
char *get_full_command_str(char *src, char c);

/**
 * Cette fonction retourne un pointeur sur une chaine contenant tout les 
 * carectères a droite de charactère c. Cela représente le message(privé de ce caractère).
 * @param src la chaine dans laquelle on recherche
 * @param c le caractère a chercher
 * @return un pointeur vers la chaine contenant tous les caractères du message
*/
char *get_full_message_str(char *src, char c);

/**
 * Cette fonction fait ce qui suit : 
 * si la chaine 1 contient la chaine 2 alors on renvoit un pointeur cntenant 
 *  le reste de la chaine 1
 * sinon on renvoi NULL
*/
BOOLEAN get_command_and_parameter(data *d, char *client_cmd, char *sys_cmd, char *serv_cmd);

/**
 * Retourne la liste des commandes utilisables
*/
command *get_all_commands();

/**
 * Formate et envoi la structure data au serveur
 * @param d la structure a envoyer
*/
void send_data(int serveur_tube_write_desc, data d);

#endif