#include "structures.h"
#include "cste.h"
#include "outils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

void to4char(long size, int begin_index, char *result)
{
    if(0<=size && size<10)
    {
        char char_size[2];
        sprintf(char_size, "%ld", size);

        result[begin_index]='0';
        result[begin_index+1]='0';
        result[begin_index+2]='0';
        result[begin_index+3]=char_size[0];
    }
    else if (10<=size && size<100)
    {
        char char_size[3];
        sprintf(char_size, "%ld", size);

        result[begin_index]='0';
        result[begin_index+1]='0';
        result[begin_index+2]=char_size[0];
        result[begin_index+3]=char_size[1];
    }
    else if (100<=size && size <1000)
    {
        char char_size[4];
        sprintf(char_size, "%ld", size);

        result[begin_index]='0';
        result[begin_index+1]=char_size[0];
        result[begin_index+2]=char_size[1];
        result[begin_index+3]=char_size[2];
    }
    else if (1000<=size && size<10000)
    {
        char char_size[5];
        sprintf(char_size, "%ld", size);

        result[begin_index]=char_size[0];
        result[begin_index+1]=char_size[1];
        result[begin_index+2]=char_size[2];
        result[begin_index+3]=char_size[3];
    }
    result[begin_index+4]='\0';
}

void strupr(char *str){
    int i=0;
    while (str[i]!='\0')
    {
        str[i] = toupper(str[i]);
        i++;
    }
}

void read_entete_in_pipe(int read_desc, entete *en_tete){
    char *rep_size = (char*)malloc(5*sizeof(char));
    read(read_desc, rep_size, 4);
    en_tete->size = atoi(rep_size);
    free(rep_size);
    read(read_desc, en_tete->cmd, 4);
}

void send_to_client(client cl, char *message){
    write(cl.tube_write_desc, message, strlen(message));
}

client *create_client(int id, char *pseudo){
    client *cl = (client*)malloc(sizeof(client));
    cl->id = id;
    cl->pseudo = (char*)malloc((strlen(pseudo)+1)*sizeof(char));
    strcpy(cl->pseudo, pseudo);
    cl->pipe = (char*)malloc(TUBE_CLIENT_PATH_SIZE*sizeof(char));
    sprintf(cl->pipe, "%s_%d", TUBE_CLIENT_PREFIX, id);
    
    //on cree le tube du client
    //creation du tube du serveur
    if( mkfifo(cl->pipe, 0644) < 0 ){
        printf("Echec de la création du tube du client %d\n", id);
        exit(0);
    }
    //cl->tube_write_desc = open(cl->pipe, O_WRONLY);
    return cl;
}

void clean_all_client(int total_client, client **client_list){
    for (int i = 0; i < total_client; i++){
        free(client_list[i]->pseudo);
        close(client_list[i]->tube_write_desc);
        unlink(client_list[i]->pipe);
        free(client_list[i]->pipe);
        free(client_list[i]);
    }
    //la liste est un tableau
    //free(client_list);
}

void clean_client_by_id(int total_client, client **client_list, int id){
    client *cl = find_client_by_id(total_client, client_list, id);
    free(cl->pseudo);
    close(cl->tube_write_desc);
    unlink(cl->pipe);
    free(cl->pipe);
    free(cl);
}

int is_client_pseudo_exist(int total_client, client **liste, char *pseudo){
    for (int i = 0; i < total_client; i++)
    {
        if( strcmp(pseudo, liste[i]->pseudo) == 0 ){
            return 1;
        }
    }
    return 0;
}

client *find_client_by_id(int total_client, client **liste, int id){
    for (int i = 0; i < total_client; i++)
    {
        if( liste[i]->id == id ){
            return liste[i];
        }
    }
    return NULL;
}

client *find_client_by_pseudo(int total_client, client **liste, char *pseudo){
    for (int i = 0; i < total_client; i++)
    {
        if( strcmp(pseudo, liste[i]->pseudo) == 0 ){
            return liste[i];
        }
    }
    return NULL;
}