#include "connection.h"
#include "../outils.h"
#include "cmd_list.h"
#include "../cste.h"
#include "fcntl.h"
#include "../structures.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void ask_connection(char *pseudo){
    //la longeur de la chaine
    long size = strlen(pseudo);
    //On ajoute 9 pour compléter. Format du message : "xxxxCONNpseudo"
    size+=9;

    //la commande pour le serveur
    char *result = (char*)malloc(size*sizeof(char));
    
    to4char(size, 0, result);
    strcat(result, CONNECTION);
    strcat(result, ' ');
    strcat(result, pseudo);
    //envoi au serveur
    int serveur_tube_write = open(TUBE_SERVEUR, O_WRONLY);
    write(serveur_tube_write, result, strlen(result));
    close(serveur_tube_write);
    free(result);
}

void wait_connection_response(int *id_client){
    int read_pipe_desc = open(TUBE_REPONSE_CONNECTION_SERVEUR, O_RDONLY);
    //On recupere les infos de la reponse
    entete en_tete;
    read_entete_in_pipe(read_pipe_desc, &en_tete);

    //si c est un succes
    if( strcmp(en_tete.cmd, CONNECTION_SUCCESS) == 0 ){
        //on lit le reste
        char *message = (char*)malloc((en_tete.size-8-1)*sizeof(char));
        read(read_pipe_desc, message, en_tete.size-8);
        message[en_tete.size-8]='\0';
        //on recupère l'identifiant du client et la reponse
        char *reponse = (char*)malloc((en_tete.size-8)*sizeof(char));
        sscanf(message, " %d %s", id_client, reponse);
        
        printf("%s\n", reponse);
        
        free(message);
        free(reponse);
    }else if( strcmp(en_tete.cmd, CONNECTION_FAILLED) == 0 ){
        //on lit le reste
        char *reponse = (char*)malloc((en_tete.size-8-1)*sizeof(char));
        read(read_pipe_desc, reponse, en_tete.size-8);
        reponse[en_tete.size-8]='\0';

        printf("%s\n", reponse);
        
        free(reponse);
    }
}