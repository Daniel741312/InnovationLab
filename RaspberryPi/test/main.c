#include "include/tcp_send.h"

int sendInfoByTCP(const char *stc, size_t length);

int main(int argc, char *argv[]){
    const char* str = "{\"id\":0,\"recyleBitMap\":77,\"location\":[120.3502, 30.3202],\"usage\":[0.3, 0.5, 0.1, 0.9]}\n";
	//const char* str="hello,world\n";

    int ret = sendInfoByTCP(str, strlen(str));
    printf("%d\n", ret);

    return 0;
}
