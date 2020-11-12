// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT    62000 
#define MAXLINE 1024 


typedef struct ve_packet_t {
    uint32_t type;
    float distance;
    float angle;
} ve_packet;
  
// Driver code 
int main() { 
    int sockfd; 
    ve_packet_t packet_buffer;



    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
  
    len = sizeof(cliaddr);  //len is value/resuslt 
    while(1){
        printf("size of ve_packet %d \n", sizeof(ve_packet));
        n = recvfrom(sockfd, &packet_buffer, MAXLINE,  
                    0, ( struct sockaddr *) &cliaddr, 
                    &len); 
        printf("Packet Received: type: %d , distance: %f , angle %f\n", packet_buffer.type, packet_buffer.distance, packet_buffer.angle);
    }
    return 0; 
} 