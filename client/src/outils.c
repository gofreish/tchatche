#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "structures.h"

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
}

void read_entete_in_pipe(int read_desc, entete *en_tete){
    char *rep_size = (char*)malloc(5*sizeof(char));
    read(read_desc, rep_size, 4);
    en_tete->size = atoi(rep_size);

    read(read_desc, en_tete->cmd, 4);
}
