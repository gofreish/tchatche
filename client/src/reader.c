#include "reader.h"
#include "outils.h"
#include "structures.h"
#include "cmd/cmd_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *reading(void *param){
    thread_data t_data = *((thread_data*)param);
    client cl = *(t_data.cl);

    entete en_tete;
    for (int i = 0; i < 2; i++)
    {
        printf("\n");
        read_entete_in_pipe(cl.tube_read_desc, &en_tete);
        //on affiche en fonction de la commande
        //Si c est un message publique
        if( strcmp(en_tete.cmd, PUBLIC_MSG)==0 ){
            //exemple recu : xxxxPUBL0005oliveBonjour a tous
            //format : longueur(4);PUBL(4);taille pseudo(4);pseudo(5);message(14)
            //total : 31; deja lu : xxxxPUBL
            char *pseudo_size_char = (char*)malloc(5*sizeof(char));
            read(cl.tube_read_desc, pseudo_size_char, 4);
            pseudo_size_char[4]='\0';
            int pseudo_size = atoi(pseudo_size_char);
                free(pseudo_size_char);
            //deja lu : xxxxPUBL0005
            char *pseudo = (char*)malloc((pseudo_size+1)*sizeof(char));
            read(cl.tube_read_desc, pseudo, pseudo_size);
            pseudo[pseudo_size]='\0';
            //deja lu xxxxPUBL0005olive
            int msg_size=en_tete.size-12-pseudo_size;
            char *message = (char*)malloc((msg_size+1)*sizeof(char));
            read(cl.tube_read_desc, message, msg_size);
            message[msg_size]='\0';

            printf("-----message-----\n%s>%s\n----------\n", pseudo, message);

            free(pseudo);
            free(message);
            printf("toto,%d\n",*t_data.msg_sended);
        }
        printf("%s>",cl.pseudo);
        //si le client avait déja envoyer une requete alors c est une reponse
        if( *t_data.msg_sended ){
            //on le débloque
            pthread_mutex_lock(t_data.mutex);
            pthread_mutex_lock(t_data.mutex);//celui débloque le lock du processus du client
            *t_data.msg_sended = 0;
            printf("client debloqué\n");
        }
        printf("Apres if\n");
    }
    return NULL;
}