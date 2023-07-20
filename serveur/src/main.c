/**
 * Code du serveur ttchatche
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "structures.h"
#include "cste.h"
#include "outils.h"
#include "cmd/cmd_list.h"
#include "cmd/connection.h"
#include "cmd/public_msg.h"

int main(int argc, char **argv){
    
    //INITIALISATION
    int serveur_tube_read_desc;
    int last_free_id = 0;
    int total_client=0;
    client *client_list[MAX_CLIENT]; //tableau de pointeur
    entete en_tete;

    //creation du tube du serveur
    if( mkfifo(TUBE_SERVEUR, 0644) < 0 ){
        printf("Echec de la création du tube /tmp/tchatche_serveur verifier si il n existe pas deja\n");
        exit(0);
    }
    //creation du tube pour le controle des clients
    if( mkfifo(TUBE_REPONSE_CONNECTION_SERVEUR, 0644) < 0 ){
        printf("Echec de la création du tube /tmp/tchatche_new_conn_ser verifier si il n existe pas deja\n");
        exit(0);
    }

    //descripteur pour la lecture
    serveur_tube_read_desc = open(TUBE_SERVEUR, O_RDONLY);
    printf("###############\n#   TCHATCHE   #\n###############\n");
    printf("--> SERVEUR LANCE<--\n");

    //on écoute perpetuelement si il y a un message
    for (size_t i = 0; i < 3; i++)
    {
        read_entete_in_pipe(serveur_tube_read_desc, &en_tete);
        //printf("---->Nouveau message\nentete: %d | %s\n", en_tete.size, en_tete.cmd);

        //Si c est une connection
        if( strcmp(en_tete.cmd, CONNECTION)  == 0 ){
            //on traite la commande
            new_connection(serveur_tube_read_desc, &last_free_id, en_tete, &total_client, client_list);
        }
        //si c est un message publique
        else if ( strcmp(en_tete.cmd, PUBLIC_MSG)  == 0 )
        {
            broadcast_msg(serveur_tube_read_desc, en_tete, total_client, client_list);
        
        }
    }
    
    clean_all_client(total_client, client_list);
    close(serveur_tube_read_desc);
    unlink(TUBE_SERVEUR);
    unlink(TUBE_REPONSE_CONNECTION_SERVEUR);
    return 0;
}