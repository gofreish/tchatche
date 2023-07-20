#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

#include "cste.h"
#include "cmd/cmd_list.h"
#include "cmd/connection.h"
#include "structures.h"
#include "data.h"
#include "reader.h"

int main(int argc, char **argv){
    //INITIALISATION
    command *all_commands = get_all_commands();
    client cl;
    cl.id = -1;
    cl.pseudo = (char*)malloc(PSEUDO_SIZE*sizeof(char));
    cl.pipe = (char*)malloc(TUBE_CLIENT_PATH_SIZE*sizeof(char));
    char *my_msg = (char *)malloc(MSG_TEXT_MAX*sizeof(char)); 
    data *d;
    //Ce mutext permet au client d'attendre la réponse a sa commande
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    thread_data t_data;
    t_data.cl = &cl;
    int msg_sended = 0;
    t_data.msg_sended = &msg_sended;
    t_data.mutex = &mutex;

    //on ouvre le tube du serveur en et on le garde.
    //cela rend la lecture bloquante 
    int serveur_tube_write = open(TUBE_SERVEUR, O_WRONLY);
    
    //CONNECTION
    printf("###############\n#   TCHATCHE   #\n###############\n\n");
    printf("Pseudo>");
    fgets(cl.pseudo, PSEUDO_SIZE, stdin);
    long pseudo_size = strlen(cl.pseudo);
    cl.pseudo[pseudo_size-1]='\0';

    ask_connection(serveur_tube_write, cl.pseudo);
    //attente de la reponse du serveur
    wait_connection_response(&cl);

    //si le client n a pas recu un identifiant on arrête le programme
    if( cl.id == -1 ){
        free(cl.pseudo);
        free(cl.pipe);
        free(my_msg);
        free(all_commands);
        exit(1);
    }

    //on cree le thread de lecture
    pthread_t thread;
    int resultat = pthread_create(&thread, NULL, reading, (void*)(&(t_data)) );
    if( resultat != 0 ){
        printf("Erreur de creation du thread\n");
        exit(1);
    }

    //si l'execution atteint cette partie alors le chat peut commencer
    long msg_size=0;
    for (int i = 0; i < 2; i++)
    {
        printf("%s>", cl.pseudo);
        fgets(my_msg, MSG_TEXT_MAX, stdin);
        msg_sended=1;
        msg_size=strlen(my_msg);
        my_msg[msg_size-1]='\0';
        d = prepare_data(my_msg, cl, all_commands);
        //printf("Datas :\nid: %d\ncmd: %s\nparam: %s\nmsg: %s\n", d->client_id, d->cmd, d->parameters, d->message);
        send_data(serveur_tube_write, *d);
        clean_data(d);

        printf("client bloqué\n");
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);//ce lock va attendre que le thread lecteur le débloque
        
    }

    pthread_join(thread, NULL);

    close(serveur_tube_write);
    free(cl.pseudo);
    free(cl.pipe);
    free(my_msg);
    free(all_commands);
    return 0;
}