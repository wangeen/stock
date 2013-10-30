#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include<sys/time.h>
#include "global.h"

using namespace std; 
#define BUFFSIZE 32
void Die(char *mess) { perror(mess);  exit(1);  }

static int stock_id = 0; 
stock stockGenerate(){
    int open = (int)(100.0*rand()/(RAND_MAX+1.0))+30; 
    int min = open - (int)(open*0.1*rand()/(RAND_MAX+1.0)); 
    int max = open + (int)(open*0.1*rand()/(RAND_MAX+1.0)); 
    int close = open -(int)(open*0.1*rand()/(RAND_MAX+1.0)); 
    close = close<min?min:close; 
    return stock(stock_id++,  open,  close,  max,  min); 
}


int tcpClient(int argc,  char** argv); 
int udpClient(int argc,  char** argv); 

int main(int argc,  char** argv){
    //return tcpClient(argc,  argv); 
    return udpClient(argc,  argv); 
}

int udpClient(int argc,  char** argv){
    cout  << "this is udp client" << endl; 
    int sock; 
    struct sockaddr_in echoserver; 
    struct sockaddr_in echoclient; 
    char buffer[BUFFSIZE]; 
    unsigned int echolen,  clientlen; 
    int received = 0; 

    /* Create the UDP socket */
    if ((sock = socket(PF_INET,  SOCK_DGRAM,  IPPROTO_UDP)) < 0) {
        Die("Failed to create socket"); 
    }
    /* Construct the server sockaddr_in structure */
    memset(&echoserver,  0,  sizeof(echoserver));        /* Clear struct */
    echoserver.sin_family = AF_INET;                   /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr(TEST_IP);   /* IP address */
    echoserver.sin_port = htons(TEST_PORT);        /* server port */
    /* Send the word to the server */
    srand((int)time(0)); 
    int loop_count = LOOP_COUNT; 
    for(int lc=0;  lc<loop_count;  ++lc){
        cout  << "client loop:" << lc << endl; 
        stock_id = 0; 
        int group_count = STOCK_COUNT/GROUP_STOCK; 
        int stock_size = 20; // byte
        //struct  timeval    tv; 
        //gettimeofday(&tv, NULL); 
        //long time = tv.tv_usec; 
        for(int i=0;  i<group_count; ++i){
            int buffer_size = GROUP_STOCK*stock_size+4; 
            unsigned char buffer[buffer_size]; 
            buffer[0] =(char)(lc>> 24); 
            buffer[1] =(char)(lc>> 16); 
            buffer[2] =(char)(lc>> 8); 
            buffer[3] =(char)(lc); 
            unsigned char* packet = buffer; 
            for(int j=0;  j<GROUP_STOCK; ++j){
                stock stk = stockGenerate(); 
                unsigned char* data = stk.encode(); 
                int buffer_offset = stock_size*j+4; 
                memcpy(buffer+buffer_offset,  data, stock_size); 
            }

            // print_memory(buffer, buffer_size); 
            if (sendto(sock, buffer, buffer_size,  0, 
                        (struct sockaddr *) &echoserver, 
                        sizeof(echoserver)) != buffer_size) {
                Die("Mismatch in number of sent bytes"); 
            }
        }
        usleep(100); 
    }
    cout  << "client finish" << endl; 
    return 0;
}

int tcpClient(int argc,  char** argv){
    cout  << "this is tcp client" << endl; 
    int sock; 
    struct sockaddr_in echoserver; 
    char buffer[BUFFSIZE]; 
    unsigned int echolen; 
    int received = 0; 

    /* Create the TCP socket */
    if ((sock = socket(PF_INET,  SOCK_STREAM,  IPPROTO_TCP)) < 0) {
        Die("Failed to create socket"); 
    }

    /* Construct the server sockaddr_in structure */
    memset(&echoserver,  0,  sizeof(echoserver));        /* Clear struct */
    echoserver.sin_family = AF_INET;                   /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr(TEST_IP);   /* IP address */
    echoserver.sin_port = htons(TEST_PORT);        /* server port */
    /* Establish connection */
    if (connect(sock, 
                (struct sockaddr *) &echoserver, 
                sizeof(echoserver)) < 0) {
        Die("Failed to connect with server"); 
    }

    /* Send the word to the server */
    srand((int)time(0)); 
    while(1){
        for(int i = 0;  i<STOCK_COUNT;  ++i){
            stock stk = stockGenerate(); 
            stk.print("client"); 
            unsigned char* data = stk.encode(); 
            echolen = 20; 
            if (send(sock,  data,  echolen,  0) != echolen) {
                Die("Mismatch in number of sent bytes"); 
            }
        }
        usleep(10); 
    }


    /* Receive the word back from the server */
    //    fprintf(stdout,  "Received: "); 
    //    while (received < echolen) {
    //        int bytes = 0; 
    //        if ((bytes = recv(sock,  buffer,  BUFFSIZE-1,  0)) < 1) {
    //            Die("Failed to receive bytes from server"); 
    //        }
    //        received  +=  bytes; 
    //        buffer[bytes] = '\0';         /* Assure null terminated string */
    //        fprintf(stdout,  buffer); 
    //    }
    //
    fprintf(stdout,  "\n"); 
    close(sock); 
    printf("client finish"); 
    exit(0); 
    return 0; 
}
