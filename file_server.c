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
#define BACKLOG 10

int main()
{
    char *c[3]={"File_Server1 41000405","File_Server2 42000405","File_Server3 43000405"};
    char *port[3]={"41000405","42000405","43000405"};
    char *file[2]={"doc1","doc2"};
    int i;
    for(i=0;i<=2;i++)
    {pid_t pid = fork();
    if(pid==0){
    int socketid, new_fd;
    struct addrinfo hints, *res, *p;
    int status;
    int numbytes;
    int yes=1;
    int j;
    struct hostent *he;
    struct in_addr **addr_list;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char buf[MAXBUFLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;//UDP
    if ((status = getaddrinfo("nunki.usc.edu", directory_phase1_port, &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
    if ((he = gethostbyname("nunki.usc.edu")) == NULL) { // get the host info
                 herror("gethostbyname");
                    return 2;
                     }
    printf("Phase 1: The File Server %d has UDP port number %s and IP address ", i+1, port[i]);
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
       if ((numbytes = sendto(socketid, c[i], strlen(c[i]), 0,p->ai_addr, p->ai_addrlen)) == -1) {perror("file: sendto");exit(1);}//send
       freeaddrinfo(res);
       printf("Phase 1:The Registration request from File Server %d has been sent to the Directory Server.\n", i+1);
       printf("Phase 1:End of Phase 1 for File Server %d.\n",i+1);
       //printf("file: sent %d bytes to %s\n", numbytes, "nunki.usc.edu");
       //printf("file: %s\n", c);
       close(socketid);
   //phase3
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;//TCP
        hints.ai_flags = AI_PASSIVE;
        if ((status = getaddrinfo(NULL, port[i], &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      return 1;
    }
    if ((he = gethostbyname("nunki.usc.edu")) == NULL) { // get the host info
                 herror("gethostbyname");
                    return 2;
                     }
    printf("Phase 3:The File Server %d has TCP port number %s and IP address ", i+1, port[i]);
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
          if (setsockopt(socketid, SOL_SOCKET, SO_REUSEADDR, &yes,
          sizeof(int)) == -1) {
           perror("setsockopt");
           exit(1);
         }
         if (bind(socketid, p->ai_addr, p->ai_addrlen) == -1) {//bind
            close(socketid);
            perror("server: bind");
             continue;
               }
            break;
           }
           if (p == NULL) {
               fprintf(stderr, "server: failed to bind\n");
             return 2;
             }
          freeaddrinfo(res);
          if (listen(socketid, BACKLOG) == -1) {//listen
           perror("listen");
            exit(1);
           }
           //sa.sa_handler = sigchld_handler; // reap all dead processes
           //sigemptyset(&sa.sa_mask);
           //sa.sa_flags = SA_RESTART;
           //if (sigaction(SIGCHLD, &sa, NULL) == -1) {
           // perror("sigaction");
           //exit(1);
           // }
      //printf("server: waiting for connections...\n");
       while(1) { // main accept() loop
          sin_size = sizeof their_addr;
           new_fd = accept(socketid, (struct sockaddr *)&their_addr, &sin_size);//listen until accept
          if (new_fd == -1) {
            perror("accept");
            continue;
              }
        if (!fork()) { // this is the child process
           close(socketid); // child doesn't need the listener
        if ((numbytes = recv(new_fd, buf, MAXBUFLEN-1, 0)) == -1) {//receive request
                 perror("recv");
                   exit(1);
                }
         printf("Phase 3:File Server %d received the request from the client%c for the file doc%c.\n", i+1, buf[6],buf[11]);
         int doc=buf[11]-'0';
         if (send(new_fd, file[doc], strlen(file[doc]), 0) == -1)//send doc
               perror("send");
               printf("Phase 3:File Server %d has sent doc%c to client%c.\n", i+1, buf[11], buf[6]);
               close(new_fd);
            exit(0);
        }
        close(new_fd);
       }exit(0);
    }
    }

       return 0;
}
