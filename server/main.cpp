// http://gnosis.cx/publish/progra;ng/sockets.html 

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <pcap.h>
#include "global.h"
#include "coefficient.h"

using namespace std; 


/* We've included the UDP header struct for your ease of customization.
 *  * For your protocol,  you might want to look at netinet/tcp.h for hints
 *   * on how to deal with single bits or fields that are smaller than a byte
 *    * in length.
 *     *
 *      * Per RFC 768,  September,  1981.
 *       */
struct UDP_hdr {
    u_short uh_sport;       /* source port */
    u_short uh_dport;       /* destination port */
    u_short uh_ulen;        /* datagram length */
    u_short uh_sum;         /* datagram checksum */
}; 


//    struct pcap_pkthdr {
//        struct timeval ts;  /* time stamp */
//        bpf_u_int32 caplen;  /* length of portion present */
//        bpf_u_int32 len;  /* length this packet (off wire) */
//    }; 
void print_pcap_pkthdr(const struct pcap_pkthdr* pkthdr){
    cout  << "caplen: " << pkthdr->caplen << " len:" << pkthdr->len << endl; 
}


/* Note,  this routine returns a pointer into a static buffer,  and
 *  * so each call overwrites the value returned by the previous call.
 *   */
const char *timestamp_string(struct timeval ts)
{
    static char timestamp_string_buf[256]; 

    sprintf(timestamp_string_buf,  "%d.%06d", 
            (int) ts.tv_sec,  (int) ts.tv_usec); 

    return timestamp_string_buf; 
}

void problem_pkt(struct timeval ts,  const char *reason)
{
    fprintf(stderr,  "%s: %s\n",  timestamp_string(ts),  reason); 
}

void too_short(struct timeval ts,  const char *truncated_hdr)
{
    fprintf(stderr,  "packet with timestamp %s is truncated and lacks a full %s\n", 
            timestamp_string(ts),  truncated_hdr); 
}


static int g_last_loop = -1; 
static int g_last_done_loop = -1; 
FILE* g_pFile; 
vector<stock>  g_stocks; 
static bool g_ready = false; 
stockPriceMap   g_priceMap; 
int g_current_stocks = 0; 

bool allPriceOK(){
    if(g_ready  ==  true) return true; 
    stockPriceMap::iterator s = g_priceMap.begin(),  e = g_priceMap.end(); 
    for(; s!=e;  ++s){
        int size = s->second.size(); 
        //cout  << "size   " << size << endl; 
        if(size>PRICE_LIST_SIZE){
            for(int i=0;  i<size-PRICE_LIST_SIZE; ++i){
                s->second.pop_front(); 
            }
        }else if(size<PRICE_LIST_SIZE){
            return false; 
        }
    }
    g_ready = true; 
    return true; 
}

/* callback function that is passed to pcap_loop(..) and called each time 
 *  * a packet is recieved                                                    */
void my_callback(u_char *useless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    //static int count = 1; 
    //fprintf(stdout, "get %d, length: %d \n", count, strlen((const char*)(packet))); 
    ///print_pcap_pkthdr(pkthdr); 
    //fflush(stdout); 
    //count++; 
    int capture_len = pkthdr->caplen; 

    struct ip *ip; 
    struct UDP_hdr *udp; 
    struct timeval ts = pkthdr->ts; 
    unsigned int IP_header_length; 

    /* For simplicity,  we assume Ethernet encapsulation. */

    if (capture_len < sizeof(struct ether_header))
    {
        /* We didn't even capture a full Ethernet header,  so we
         *       * can't analyze this any further.
         *               */
        too_short(ts,  "Ethernet header"); 
        return; 
    }

    /* Skip over the Ethernet header. */
    packet  +=  sizeof(struct ether_header); 
    capture_len -= sizeof(struct ether_header); 

    if (capture_len < sizeof(struct ip))
    { /* Didn't capture a full IP header */
        too_short(ts,  "IP header"); 
        return; 
    }

    ip = (struct ip*) packet; 
    IP_header_length = ip->ip_hl * 4;   /* ip_hl is in 4-byte words */

    if (capture_len < IP_header_length)
    { /* didn't capture the full IP header including options */
        too_short(ts,  "IP header with options"); 
        return; 
    }

    if (ip->ip_p != IPPROTO_UDP)
    {
        problem_pkt(ts,  "non-UDP packet"); 
        return; 
    }

    /* Skip over the IP header to get to the UDP header. */
    packet  +=  IP_header_length; 
    capture_len -= IP_header_length; 

    if (capture_len < sizeof(struct UDP_hdr))
    {
        too_short(ts,  "UDP header"); 
        return; 
    }

    udp = (struct UDP_hdr*) packet; 

    //   printf("%s UDP src_port=%d dst_port=%d length=%d\n", 
    //           timestamp_string(ts), 
    //           ntohs(udp->uh_sport), 
    //           ntohs(udp->uh_dport), 
    //           ntohs(udp->uh_ulen)); 

    // content
    //stock skt; 
    int size = sizeof(UDP_hdr); 
    packet  +=  size; 
    //skt.decode((char*)packet); 
    //skt.print("server"); 
    // get loop id
    int  loop_id= (packet[0]  <<  24 | packet[1]  <<  16 | packet[2]  <<  8 | packet[3]);          
    // write to disk
    fwrite(packet,  1,  capture_len-size,  g_pFile); 
    if(g_last_done_loop == loop_id){
        return;
    }
    if(loop_id != g_last_loop){
        g_last_loop = loop_id; 
        //    g_stocks.clear(); 
        //    g_stocks.reserve(4000); 
    }

    packet  +=  4; 
    // decode 
    for(int i=0;  i<GROUP_STOCK; ++i){
        stock st; 
        st.decode(packet); 
        packet  +=  20; 
        int id = st.id_; 
        //st.print(); 
        if(g_priceMap.find(id)!=g_priceMap.end()){
            if(allPriceOK()){
                g_priceMap[id].pop_front(); 
            }
            //cout  << "xx"  << id << endl; 
            g_priceMap[id].push_back(st.close_price_); 
            ++g_current_stocks; 
            if(g_current_stocks  ==  SERVER_STOCK){
                cout  << "server get all" << endl; 
                if(allPriceOK()){
                    coefficient(&g_priceMap); 
                    g_last_loop = -1; 
                    g_last_done_loop = loop_id; 
                }
                g_current_stocks = 0; 
            }
        }
        //g_stocks.push_back(st); 
    }
}




void listAllDevice(){
    char *dev,  errbuf[PCAP_ERRBUF_SIZE]; 
    pcap_if_t *alldevsp;        /* list of interfaces */

    if (pcap_findalldevs (&alldevsp,  errbuf) < 0)   
    {
        fprintf (stderr,  "%s",  errbuf); 
        exit (1); 
    }
    while (alldevsp != NULL)
    {
        printf ("%s\n",  alldevsp->name); 
        alldevsp = alldevsp->next; 
    }
}

int pcapServer(int argc,  char** argv){
    listAllDevice(); 
    char *dev,  errbuf[PCAP_ERRBUF_SIZE]; 

    //    dev = pcap_lookupdev(errbuf); 
    //    if (dev  ==  NULL) {
    //        fprintf(stderr,  "Couldn't find default device: %s\n",  errbuf); 
    //        return(2); 
    //    }
    dev = SNIFF_DEVICE; 
    printf("Device: %s\n",  dev); 
    pcap_t *handle; 

    handle = pcap_open_live(dev,  BUFSIZ,  1,  1000,  errbuf); 
    if (handle  ==  NULL) {
        fprintf(stderr,  "Couldn't open device %s: %s\n",  dev,  errbuf); 
        return(2); 
    }
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr,  "Device %s doesn't provide Ethernet headers - not supported\n",  dev); 
        return(2); 
    }

    struct bpf_program fp;         /* The compiled filter expression */
    char filter_exp[] = "port 4567 and udp";     /* The filter expression */
    bpf_u_int32 mask;      /* The netmask of our sniffing device */
    bpf_u_int32 net;       /* The IP of our sniffing device */
    struct pcap_pkthdr header;  /* The header that pcap gives us */
    const u_char *packet;       /* The actual packet */

    if (pcap_lookupnet(dev,  &net,  &mask,  errbuf)  ==  -1) {
        fprintf(stderr,  "Can't get netmask for device %s\n",  dev); 
        net = 0; 
        mask = 0; 
    }else{
        cout  << "net: " << net << " mask:" << mask << endl; 
    }

    if (pcap_compile(handle,  &fp,  filter_exp,  0,  net)  ==  -1) {
        fprintf(stderr,  "Couldn't parse filter %s: %s\n",  filter_exp,  pcap_geterr(handle)); 
        return(2); 
    }

    if (pcap_setfilter(handle,  &fp)  ==  -1) {
        fprintf(stderr,  "Couldn't install filter %s: %s\n",  filter_exp,  pcap_geterr(handle)); 
        return(2); 
    }

    int package_count = SERVER_STOCK_COUNT; 
    pcap_loop(handle, package_count, my_callback, NULL); 
    //packet = pcap_next(handle,  &header); 
    /* And close the session */
    pcap_close(handle); 

    printf("server finish"); 
    return(0); 
}



#define MAXPENDING 5    /* Max co; 
                           ection requests */
#define BUFFSIZE 32
void Die(char *mess) { perror(mess);  exit(1);  }


void HandleClient(int sock) {
    unsigned char buffer[BUFFSIZE]; 
    int received = -1; 
    /* Receive message */
    if ((received = recv(sock,  buffer,  BUFFSIZE,  0)) < 0) {
        Die("Failed to receive initial bytes from client"); 
    }
    /* Send bytes and check for more incoming data in loop */
    while (received > 0) {
        /* Send back received data */
        // if (send(sock,  buffer,  received,  0) != received) {
        //     Die("Failed to send bytes to client"); 
        // }
        /* Check for more data */
        if ((received = recv(sock,  buffer,  BUFFSIZE,  0)) < 0) {
            Die("Failed to receive additional bytes from client"); 
        }
        stock stk; 
        stk.decode(buffer); 
        stk.print("server"); 
    }
    close(sock); 
}


//int socketServer(int argc,  char** argv){
//    cout  << "this is server" << endl; 
//    int serversock,  clientsock; 
//    struct sockaddr_in echoserver,  echoclient; 
//
//    /* Create the TCP socket */
//    if ((serversock = socket(PF_INET,  SOCK_STREAM,  IPPROTO_TCP)) < 0) {
//        Die("Failed to create socket"); 
//    }
//    /* Construct the server sockaddr_in structure */
//    memset(&echoserver,  0,  sizeof(echoserver));        /* Clear struct */
//    echoserver.sin_family = AF_INET;                   /* Internet/IP */
//    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);    /* Incoming addr */
//    echoserver.sin_port = htons(TEST_PORT); /* server port */
//
//
//    /* Bind the server socket */
//    if (bind(serversock,  (struct sockaddr *) &echoserver, 
//                sizeof(echoserver)) < 0) {
//        Die("Failed to bind the server socket"); 
//    }
//    /* Listen on the server socket */
//    if (listen(serversock,  MAXPENDING) < 0) {
//        Die("Failed to listen on server socket"); 
//    }
//
//    /* Run until cancelled */
//    for(int i = 0;  i<STOCK_COUNT;  ++i){
//        //while (1) {
//        unsigned int clientlen = sizeof(echoclient); 
//        /* Wait for client connection */
//        if ((clientsock =
//                    accept(serversock,  (struct sockaddr *) &echoclient, 
//                        &clientlen)) < 0) {
//            Die("Failed to accept client connection"); 
//        }
//        fprintf(stdout,  "Client connected: %s\n", 
//                inet_ntoa(echoclient.sin_addr)); 
//        HandleClient(clientsock); 
//    }
//    return 0; 
//    }

// netif_receive_skb
//
// Libpcap
// http://blog.csdn.net/htttw/article/details/7521053
// http://www.tcpdump.org/#documentation
//
int main(int argc,  char** argv){
    g_pFile = fopen("stocks.db",  "wb");
    for(int i = 0;  i<SERVER_STOCK;  ++i)
        g_priceMap[i] = deque<int>();  

    pcapServer(argc,  argv); 
    fclose(g_pFile); 
    return 0; 
}



