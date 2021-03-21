#include "include/tcp_send.h"

int sendInfoByTCP(const char* stc,size_t length);

int main(int argc,char* argv[]){
    const char* str="{\"id\":6,\"recyleBitMap\":14,\"location\":[120.3532, 30.3242],\"usage\":[0.9, 1, 1, 0.5]}";

    int ret=sendInfoByTCP(str,strlen(str));
    
    printf("%d\n",ret);

    return 0;
}