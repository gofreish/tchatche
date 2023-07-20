#include "public_msg.h"
#include "cmd_list.h"
#include "../structures.h"
#include "../outils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void broadcast_msg(int serveur_tube_read_desc, entete en_tete, int total_client, client **liste){
    //exemple recu : xxxxPUBL0011Bonjour a tous\0; entete:12; message:14; total=12+14+1=27
    //deja lu : xxxxPUBL
    char *id_char = (char*)malloc(5*sizeof(char));
    read(serveur_tube_read_desc, id_char, 4);
    id_char[4]='\0';
    int client_id= atoi(id_char);
    free(id_char);
    client *cl = find_client_by_id(total_client, liste, client_id);
    if(cl==NULL){
        printf("sender not found\n");
        return;
    }
    //deja lu : xxxxPUBL0011
    char *message=(char*)malloc((en_tete.size-12)*sizeof(char));
    read(serveur_tube_read_desc, message, en_tete.size-12);
    //exemple a envoyer : xxxxPUBL0005oliveBonjour a tous
    //format : longueur;PUBL;taille pseudo;pseudo;message
    long size = strlen(cl->pseudo) + strlen(message) + 13;
    char *broadcast=(char*)malloc((size)*sizeof(char));
    to4char(size, 0, broadcast);
    strcat(broadcast, PUBLIC_MSG);
    to4char(strlen(cl->pseudo), 8, broadcast);
    strcat(broadcast, cl->pseudo);
    strcat(broadcast, message);
    broadcast[size-1]='\0';
    printf("Message publique de : %s\n", cl->pseudo);
    
    write(cl->tube_write_desc, broadcast, size);
    
    free(message);
    free(broadcast);
}