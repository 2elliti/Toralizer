#include "toralize.h"





void print_usage(char * executable){

    fprintf(stderr,"USAGE: %s <host> <port>\n",executable);
    exit(1);
}


Req *request_generator(const char *dst_ip,int dst_port){
    Req *req = malloc(sizeof(reqsize));

    req->DSTPORT = htons(dst_port);
    req->DSTIP = inet_addr(dst_ip);
    req->VN = 4;
    req->CD = 1;
    strncpy(req->USERID,USERNAME,8);

    return req;
}


int main(int argc, char*argv[]){

    if(argc != 3){
        print_usage(argv[1]);
    }

    Req *req;
    Res *res;

    char *host;
    int port;
    host = argv[1];
    port = atoi(argv[2]);

    char buf[ressize];
    char tmp[512];

    int some_socket = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in socks;

    socks.sin_addr.s_addr = inet_addr(PROXY);
    socks.sin_family = AF_INET;
    socks.sin_port = htons(PROXYPORT);


    int compare = connect(some_socket,(struct sockaddr *)&socks,sizeof(socks));

    if(compare !=0){
        fprintf(stderr,"Cant' connect\n");
        exit(1);
    }

    printf("connection successfull.\n");


    req = request_generator(host,port);
    write(some_socket,req,reqsize);

    memset(buf,0,ressize);

    if(read(some_socket,buf,ressize) <1){
        fprintf(stderr,"Cant read data\n");
        free(req);
        close(some_socket);
        exit(1);
    }

    res = (Res *)buf;

    if(res->cd == 90){
        printf("access granted\n");
    }else{
        fprintf(stderr,"access not granted\n");
        exit(1);
    }


    memset(tmp,0,512);


    snprintf(tmp,511,
        "HEAD/ HTTP/1.0\r\n"
        "Host: www.google.com\r\n"
        "\r\n"

    );

    write(some_socket,tmp,strlen(tmp));

    memset(tmp,0,512);

    read(some_socket,tmp,511);

    printf("\n----------------------\n %s \n-------------------\n",tmp);
    

    close(some_socket);

    return 0;
}