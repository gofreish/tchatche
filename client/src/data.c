#include "data.h"
#include "outils.h"
#include "structures.h"
#include "cste.h"
#include "cmd/cmd_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void clean_data(data *d){
    if(d->message!=NULL) {free(d->message);}
    if(d->cmd!=NULL) {free(d->cmd);}
    if(d->parameters!=NULL) {free(d->parameters);}
    free(d); 
}

data *prepare_data(char *input, client cl, command *all_cmd){
    //On cherche le caracteres : qui indique qu'il y a une commande
    //separer en deux parties cmd et message
    char *cmd = get_full_command_str(input, ':');
    char *msg = get_full_message_str(input, ':');
    data *d = (data*)malloc(sizeof(data));
    d->client_id = cl.id;
    
    //si il ya un commande
    if( cmd != NULL ){
        for (int i = 0; i < CLIENT_CMD_NUMBER; i++)
        {
            BOOLEAN result = get_command_and_parameter(d, cmd, all_cmd[i].sys_cmd, all_cmd[i].serv_cmd);
            if( result == TRUE ){
                break;
            }
        }
        free(cmd);
    }else{
        d->cmd=(char*)malloc(5*sizeof(char));
        strcpy(d->cmd, PUBLIC_MSG);
        d->parameters=NULL;
    }
    //si il y a un message
    if( msg != NULL ){
        long msg_len = strlen(msg);
        d->message = (char*)malloc((msg_len+1)*sizeof(char)); 
        strcpy(d->message, msg);
        d->message[msg_len]='\0';
        free(msg);
    }else{
        d->message = NULL;
    }
    return d;
}

char *get_full_command_str(char *src, char c){
    //ex : inbox toto: tatatatatatatat
    //i=10
    int i=0, is_find=0;
    while( 1 ){
        if( src[i] ==':' ){
            is_find = 1;
            break;
        }else if ( src[i+1]=='\0' )
        {
            break;
        }
        i++;
    }
    //si c est trouver
    if( is_find ){
        char *cmd = (char*)malloc((i+1)*sizeof(char));
        //on copie juste la partie a gauche dans cmd
        strncpy(cmd, src, i);
        cmd[i] = '\0';
        return cmd;
    }else{
        return NULL;
    }
}

char *get_full_message_str(char *src, char c){
    //ex : inbox toto: tatatatatatatat total=27
    //i=10
    //a lire 16 = total-i-1
    //a allouer 17 = total-i
    int i=0, is_find=0;
    while( 1 ){
        if( src[i] ==':' ){
            is_find = 1;
            break;
        }else if ( src[i+1]=='\0' )
        {
            break;
        }
        i++;
    }
    long src_len = strlen(src);
    //si il y a un message
    if( src_len-i > 0 ){
        //si c est trouver
        if( is_find ){
            char *message = (char*)malloc((src_len-i)*sizeof(char));
            //on copie juste la partie a droite dans message
            strcpy(message, src+i+1);
            message[src_len-i-1] = '\0';
            return message;
        }else{
            char *message = (char*)malloc((src_len+1)*sizeof(char));
            //on copie tout car il n'y a pas de commande
            strcpy(message, src);
            message[src_len] = '\0';
            return message;
        }
    }
    return NULL;
}

BOOLEAN get_command_and_parameter(data *d, char *client_cmd, char *sys_cmd, char *serv_cmd){
    //ex1 : CH1="INBOX olive"; size1=11; CH2="INBOX "; size2=6, size1-size2=5 
    long size1 = strlen(client_cmd), size2 = strlen(sys_cmd);
    if( size1 < size2 ) return FALSE;

    for (int i = 0; i < size2; i++)
    {   
        if( toupper(client_cmd[i]) != sys_cmd[i] ) return FALSE;
    }
    //a partir d ici on a une correspondance
    //si ils n ont pas la même taille alors il y a un paramètre
    int param_size = size1 - size2;
    if( param_size > 0 ){
        d->parameters = (char*)malloc((param_size+1)*sizeof(char));
        strcpy(d->parameters, client_cmd+size2);
        d->parameters[param_size] = '\0';
    }else{
        d->parameters = NULL;
    }
    //on copie la commande pour le serveur
    long cmd_size = strlen(serv_cmd);
    d->cmd = (char*)malloc((cmd_size+1)*sizeof(char));
    strcpy(d->cmd, serv_cmd);
    d->cmd[cmd_size] = '\0';
    return TRUE;
}

command *get_all_commands(){
    command *liste = (command*)malloc(CLIENT_CMD_NUMBER*sizeof(command));
    
    command exit; exit.sys_cmd = "EXIT"; exit.serv_cmd = "EXIT";
    liste[0]=exit;

    command inbox; inbox.sys_cmd = "INBOX "; inbox.serv_cmd = "PRIV";
    liste[1]=inbox;
    
    command list; list.sys_cmd = "LIST"; list.serv_cmd = "LIST";
    liste[2]=list;
    
    command shut; shut.sys_cmd = "SHUT"; shut.serv_cmd = "SHUT";
    liste[3]=shut;

    return liste;
}

void send_data(int serveur_tube_write_desc, data d){
    if( strcmp(d.cmd, PUBLIC_MSG)==0 ){
        //exemple : xxxxPUBL0011Bonjour a tous
        //entete size = 12; msg size=14; total=12+14+1=27
        long size = strlen(d.message);
        size+=13;
        char *paquet = (char*)malloc(size*sizeof(char));
        to4char(size, 0, paquet);
        strcat(paquet, PUBLIC_MSG);
        to4char(d.client_id, 8, paquet);
        strcat(paquet, d.message);
        paquet[size-1]='\0';
        //printf("paquet: %s\n", paquet);
        write(serveur_tube_write_desc, paquet, size);
        free(paquet);
    }
    
    else if(strcmp(d.cmd, PRIVATE_MSG)==0){
        printf("Priver a : %s\n", d.parameters);
    }
    
    else if(strcmp(d.cmd, EXIT_CLIENT)==0){
        printf("Exit\n");
    }

    else if(strcmp(d.cmd, SHUTDOWN)==0){
        printf("Extinction serveur\n");
    }

    else if(strcmp(d.cmd, LIST)==0){
        printf("User list\n");
    }
    else{
        printf("CMD inconue\n");
    }
}