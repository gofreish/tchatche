#include "connection.h"
#include "cmd_list.h"
#include "../structures.h"
#include "../outils.h"
#include "../cste.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void new_connection(int read_pipe, int *last_free_id, entete en_tete, int *total_client, client **client_list){
    //Format attendu : "xxxxCONN pseudo\0"
    char *pseudo = (char*)malloc((en_tete.size-8+1)*sizeof(char));
    read(read_pipe, pseudo, en_tete.size-8);

    printf("new conn Pseudo=%s\nNew connection\n", pseudo);

    if( find_client_by_pseudo(*total_client, client_list, pseudo) != NULL ){
        printf("Personne trouver\n");
        say_connection_failled_to_client("Pseudo deja utiliser");
    }
    else{
        //nouveau client
        client *cl = create_client(*last_free_id, pseudo);
        *last_free_id = *last_free_id+1;
        //on ajoute le client
        client_list[*total_client] = cl;
        *total_client = *total_client+1;
        printf("Pseudo valide\n");
        
        //On lui envoit son identifiant
        say_connected_to_client(*cl, "Connected");
    }
    free(pseudo);
}

void say_connected_to_client( client cl, char *message ){
    long size = strlen(message);
    size+=13;
    char *reponse = (char*)malloc(size*sizeof(char));
    to4char(size, 0, reponse);
    strcat(reponse, CONNECTION_SUCCESS);
    to4char(cl.id, 8, reponse);
    strcat(reponse, message);
    printf("To client : %s\n", reponse);

    //Envoi de la reponse
    send_to_client(cl, reponse);
}

void say_connection_failled_to_client(char *msg){
    long size = strlen(msg);
    size+=9;
    char *reponse = (char*)malloc(size*sizeof(char));
    to4char(size, 0, reponse);
    strcat(reponse, CONNECTION_FAILLED);
    strcat(reponse, msg);
    printf("To client : %s\n", reponse);

    //Envoi de la reponse
    int write_client_pipe = open(TUBE_REPONSE_CONNECTION_SERVEUR, O_WRONLY);
    write(write_client_pipe, reponse, size);
    close(write_client_pipe);
}