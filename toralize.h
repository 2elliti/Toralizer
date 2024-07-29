/* toralize.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>


/*
socket()       
connect()
close()
htons()
inet_addr()

understanding byte order.
-> lets say we have a port number(9050) : 11010011 11011011 (in binary, 16bits)

-> then we need to copy this right part to the left: 11011011 11010011


here htons() do this for the port number. and inet_addr() take ip addrs make convert
it into big number then perform Network byte order.

*/



#define PROXY "127.0.0.1"
#define PROXYPORT 9050
#define USERNAME "toraliz"      //it is 7 byte long because we need last byte to be null.
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

//REQUEST PACKET

/*
            +----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
 # :	   1    1      2              4           variable       1
*/

struct proxy_request{
    int8 VN;
    int8 CD;
    int16 DSTPORT;
    int32 DSTIP;
    unsigned char USERID[8];

};

typedef struct proxy_request Req;

// REPLY PACKET

/*
        
		+----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
 # :	   1    1      2              4


*/

struct proxy_response{

    int8 vn;
    int8 cd;
    int16 _;            // we used(_) because they are not important. for more knowledge refer to this site https://www.openssh.com/txt/socks4.protocol
    int32 __;           // or just simply google SOCKS4 rfc.
};

typedef struct proxy_response Res;

Req *request(struct sockaddr_in* );
int connect(int , const struct sockaddr *,socklen_t);