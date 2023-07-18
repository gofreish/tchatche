#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "cste.h"
#include "cmd/cmd_list.h"
#include "cmd/connection.h"

int main(int argc, char **argv){
    
    //INITIALISATION
    int my_id = -1;
    char my_pseudo[PSEUDO_SIZE];
    // *msg = (char*)malloc(MSG_TEXT_MAX*sizeof(char));

    //CONNECTION
    printf("###############\n#   TCHATCHE   #\n###############\n\n");
    printf("Pseudo>");
    fgets(my_pseudo, PSEUDO_SIZE, stdin);

    ask_connection(my_pseudo);

    //attente de la reponse du serveur
    wait_connection_response(&my_id);

    //si le client n a pas recu un identifiant on arrête le programme
    if( my_id == -1 ){
        exit(1);
    }

    //si l'execution atteint cette partie alors le chat peut commencer
    
    return 0;
}