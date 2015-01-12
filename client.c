#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>

#define directory_phase1_port "2505"
#define directory_phase2_port "3505"
#define file1_phase1_port "2605"
#define file1_phase3_port "4505"
#define file2_phase1_port "2705"
#define file2_phase3_port "4605"
#define file3_phase1_port "2805"
#define file3_phase3_port "4705"
#define client1_phase2_port "3605"
#define client2_phase2_port "3705"
#define MAXBUFLEN 100

int main()
{
    char *c[2]={"Client1 doc1","Client2 doc2"};
    int i;
    char filenumber[2];
    char *port[2]={"32000405","33000405"};
    for(i=0;i<=1;i++)
    {
    pid_t pid = fork();
    if(pid==0)
    {
        struct sockaddr_in dy;
       int dy_len = sizeof(dy);
       char dy_ip[20];
	   int getsock_check;
	   struct hostent *he;
    struct in_addr **addr_list;
    int socketid;
    int j;
    int k=i+1;
    struct addrinfo hints, *res, *p;
    int status;
    int numbytes;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    struct sockaddr_storage their_addr;
    char filename;
    char portnum[8];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((status = getaddrinfo("nunki.usc.edu", directory_phase2_port, &hints, &res)) != 0)//get address info
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
     if ((he = gethostbyname("nunki.usc.edu")) == NULL) { // get the host info
                 herror("gethostbyname");
                    return 2;
                     }
    printf("Phase 2:The Client %d has UDP port number %s and IP address ", i+1, port[i]);
    addr_list = (struct in_addr **)he->h_addr_list;
       for(j = 0; addr_list[j] != NULL; j++) {
        printf("%s ", inet_ntoa(*addr_list[j]));
         }
         printf("\n");
    for(p = res;p != NULL; p = p->ai_next) {
          if((socketid=socket(p->ai_family, p->ai_socktype,p->ai_protocol))==-1)//create socket
          {perror("file: socket");
          continue;
          }
          break;
       }
       if(p==NULL){
       fprintf(stderr, "dirserver: failed to bind socket\n");
       return 2;
       }
       if ((numbytes = sendto(socketid, c[i], strlen(c[i]), 0,p->ai_addr, p->ai_addrlen)) == -1) {perror("file: sendto");exit(1);}//send message
       freeaddrinfo(res);
       printf("The File request from Client %d has been sent to the Directory Server.\n", i+1);
       addr_len = sizeof their_addr;
       if ((numbytes = recvfrom(socketid, buf, MAXBUFLEN-1, 0,(struct sockaddr *)&their_addr, &addr_len)) == -1)//receive message
       {
       perror("recvfrom");
       exit(1);}
       //printf("buffer=%s\n", buf);
       filename=buf[11];
       //printf("filename=%c\n", filename);
       filenumber[i]=filename;
       //printf("%c\n", filenumber[i]);
       for(j=0;j<8;j++)//store TCP num
       {
        portnum[j]=buf[j+13];

        //printf("%c", portnum[j]);
       }
        printf("Phase 2:The File requested by Client %d is present in File_Server %c and the File Server's TCP port number is %s\n", k, filename, portnum);
        printf("Phase 2:End of Phase 2 for Client %d\n", k);
        close(socketid);
//phase3

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if ((status = getaddrinfo("nunki.usc.edu", portnum, &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      return 1;
    }
     //printf("The Directory Server has UDP port number %s and IP address %s.\n", directory_phase1_port, hostname);
    for(p = res;p != NULL; p = p->ai_next) {
          if((socketid=socket(p->ai_family, p->ai_socktype,p->ai_protocol))==-1)
          {perror("file: socket");
          continue;
          }
          if (connect(socketid, p->ai_addr, p->ai_addrlen) == -1) {
          close(socketid);
          perror("client: connect");
          continue;
          }
           break;
       }
       if(p==NULL){
       fprintf(stderr, "client: failed to connect\n");
       return 2;
       }
        getsock_check=getsockname(socketid, (struct sockaddr *)&dy, &dy_len);//get dynamical TCP port
	   inet_ntop(AF_INET, &dy.sin_addr, dy_ip, sizeof(dy_ip));
       printf("Phase 3: Client %d has dynamic TCP port number %d and IP address %s.\n", i+1, ntohs(dy.sin_port),dy_ip);

        if (send(socketid, c[i], strlen(c[i]), 0) == -1)perror("send");//send request
        freeaddrinfo(res);
        printf("Phase 3:The File request from Client %d has been sent to the File_server %c.\n", k, filename);
        if ((numbytes = recv(socketid, buf, MAXBUFLEN-1, 0)) == -1) {//receive doc
         perror("recv");
         exit(1);
         }
         printf("Phase 3:Client %d received doc%c from File_server %c.\n", k, c[i][11], filename);
         close(socketid);
         printf("Phase 3: End of Phase 3 for Client %d.\n", k);
         exit(0);
     }

    }


       return 0;
}
