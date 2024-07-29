/* toralize.c */

/*

1. TURN CLIENT INTO LIB (SHARED LIBRARY) .SO
2. TURN main() INTO OUR OWN CONNECT()
3. REPLACE REGULAR CONNECT()
4. GRAB THE IP AND PORT FROM THE ORIGINAL CONNECT.

*/


#include "toralize.h"


//this function creates a  -->REQUEST PACKET<-- 
Req *request(struct sockaddr_in *sock2){

    Req *req;
    req = malloc(reqsize);
    req->VN = 4;
    req->CD = 1;
    req->DSTPORT = sock2->sin_port;
    req->DSTIP = sock2->sin_addr.s_addr;
    strncpy(req->USERID,USERNAME,8);

    return req;
}





int connect(int s2, const struct sockaddr *sock2,
        socklen_t addrlen){

//    char *host;             //host name that we actually want to connect to. server that we wanna connect to.

    int s;

    struct sockaddr_in sock;
    Req *req;
    Res *res;  
    char buf[ressize];
    int success;
    char tmp[512];

    int (*p)(int , const struct sockaddr *,socklen_t);

    /* predicate */


    // this if statement check wether argument passed are correct or not.
    // if(argc < 3){

    //     fprintf(stderr, "Usage: %s <host> <port> \n", argv[0]); //here fprintf() is used to pass output to different stream. google it, if you don't know
    //     return -1;
    // }   

    // host = argv[1];

    // port = atoi(argv[2]);

    p = dlsym(RTLD_NEXT,"connect");

    


    s = socket(AF_INET, SOCK_STREAM,0);

    if(s<0){
        perror("socket");

        return -1;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if(p(s,(struct sockaddr *)&sock,sizeof(sock))){

        perror("connect");
        return -1;
    }

    printf("connected to proxy server\n");

    req = request((struct sockaddr_in *)sock2);
    write(s, req, reqsize);

    memset (buf,0, ressize);
    if(read(s,buf,ressize)<1){
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buf;

    success = (res->cd == 90);
    if(!success){
        fprintf(stderr,"Unable to traverse the proxy,"
                    "error code = %d\n",res->cd);
        
        close(s);
        free(req);
        return -1;
    }

    printf("Successfully Connected through the proxy to");


    dup2(s,s2);
    free(req);
    return 0;


    
}
