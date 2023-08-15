#include<stdio.h>
#include<string.h>
#include "base64.h"

int main(int argc,char *argv[])
{
    if(argc == 1){
        printf("-e encode String\n");
        printf("-d decode String\n");
        return 0;
    }
    
    if(!strcmp(argv[1],"-e")){
        printf("%s",base64EncodeStr(argv[2]));  
    }
    else if(!strcmp(argv[1],"-d")){
        printf("%s",base64DecodeStr(argv[2]));
    }
    
}