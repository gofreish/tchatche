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

void ask_connection(int serveur_tube_write, char *pseudo){
    //la longeur de la chaine
    long size = strlen(pseudo);
    //On ajoute 9 pour compléter. Format du message : "xxxxCONNpseudo"
    size+=9;

    //la commande pour le serveur
    char *result = (char*)malloc(size*sizeof(char));
    
    to4char(size, 0, result);
    strcat(result, CONNECTION);
    strcat(result, pseudo);
    //envoi au serveur
    
    write(serveur_tube_write, result, strlen(result));
    
    free(result);
}

void wait_connection_response(client *cl){
    //on attend une reponse
    int read_pipe_desc = open(TUBE_REPONSE_CONNECTION_SERVEUR, O_RDONLY);
    //On recupere les infos de la reponse
    entete en_tete;
    read_entete_in_pipe(read_pipe_desc, &en_tete);
    //printf("Entete lu : %d %s", en_tete.size, en_tete.cmd);

    //si c est un succes
    if( strcmp(en_tete.cmd, CONNECTION_SUCCESS) == 0 ){
        //Exemple de reponse : 0021CSUC0001Connected
        //on lit l identifiant et on cree le chemin vers le tube
        fill_client(read_pipe_desc, cl);
        //on ouvre le tube du client en lecture
        cl->tube_read_desc = open(cl->pipe, O_RDONLY);

        //on lit le reste
        char *message = (char*)malloc((en_tete.size-12-1)*sizeof(char));
        read(read_pipe_desc, message, en_tete.size-12);
        message[en_tete.size-12]='\0';
        
        printf("%s\n", message);
        
        free(message);
    }else if( strcmp(en_tete.cmd, CONNECTION_FAILLED) == 0 ){
        //exemple de reponse : xxxxCBADPseudo existe deja
        //on lit le message
        char *message = (char*)malloc((en_tete.size-8-1)*sizeof(char));
        read(read_pipe_desc, message, en_tete.size-8);
        message[en_tete.size-8]='\0';

        printf("%s\n", message);
        
        free(message);
    }
    close(read_pipe_desc);
}