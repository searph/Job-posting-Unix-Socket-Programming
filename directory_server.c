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

//void *get_in_addr(struct sockaddr *sa)
//{
//if (sa->sa_family == AF_INET) {
//return &(((struct sockaddr_in*)sa)->sin_addr);
//}
//return &(((struct sockaddr_in6*)sa)->sin6_addr);
//}

int main(void)
{
    int status;
    struct addrinfo hints;
    struct addrinfo *res,*p; // will point to the results
    int socketid1;
    int socketid2;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];
    void *addr;
    char ipstr[INET_ADDRSTRLEN];
    int i,j;
     struct hostent *he;
    struct in_addr **addr_list;
//get address
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_INET; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    if ((status = getaddrinfo(NULL, directory_phase1_port, &hints, &res)) != 0)
    {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }
     if ((he = gethostbyname("nunki.usc.edu")) == NULL) { // get the host info
                 herror("gethostbyname");
                    return 2;
                     }
    printf("Phase 1: The Directory Server has UDP port number %s and IP address ", directory_phase1_port);
    addr_list = (struct in_addr **)he->h_addr_list;
       for(i = 0; addr_list[i] != NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
         }
         printf("\n");
    //char *c=directory_phase1_port;
    //printf("%s", c);
    //struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
    //addr = &(ipv4->sin_addr);
    //inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    //printf("The Directory Server has UDP port number %s and IP address %s.\n", c, ipstr);
    //create socket
     for(p = res;p != NULL; p = p->ai_next) {
          if((socketid1=socket(p->ai_family, p->ai_socktype,p->ai_protocol))==-1)//create socket
          {perror("dirserver: socket");
          continue;
          }
          if(bind(socketid1, p->ai_addr, p->ai_addrlen)==-1){//bind
          close(socketid1);
          perror("dirserver: bind");
          continue;
          }
          break;
       }
       if(p==NULL){
       fprintf(stderr, "dirserver: failed to bind socket\n");
       return 2;
       }
       freeaddrinfo(res);
       //printf("dirserver: waiting to recvfrom file...\n");

       //wait for receiving
        char totalrec[100];
        char row;
        for(i=1;i<=3;i++)
        {
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(socketid1, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {//receive
        perror("recvfrom");
        exit(1);}
        //printf("dirserver: got packet from %s\n",inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
        //printf("dirserver: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\n';
        strcat(totalrec, buf);// store all the message received into totalrec
        row=buf[11];
        //printf("%s", totalrec);
        //printf("dirserver: packet contains \"%s\"\n", buf);
        printf("Phase 1:The Directory Server has received request from File Server %c\n", row);
        }
        strcat(totalrec, "\0");
        //printf("%s", totalrec);
        char ch;
        FILE *fp;
        if((fp=fopen("directory.txt", "r")) ==NULL){printf("The file 'directory.txt' does not exist.\n");}//create directpry.txt
        fp=fopen("directory.txt", "w+");//printf("open successfully.\n");
        rewind(fp);
        for(j=0;totalrec[j]!='\0';j++)
         {
             if(totalrec[j]=='\n'){fputc('\r',fp);fputc('\n',fp);}//cteate directory.txt
             fputc(totalrec[j],fp);
         }
        fclose(fp);//printf("socket1=%d\n",socketid1);
        close(socketid1);
        printf("Phase 1:The directory.txt file has been created. End of Phase 1 for the Directory Server.\n");

        //printf("i=%d",i);


//pahse2
        if(i==4){
        memset(&hints, 0, sizeof hints); // make sure the struct is empty
        hints.ai_family = AF_INET; // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM; // UDP stream sockets
        hints.ai_flags = AI_PASSIVE; // fill in my IP for me
        if ((status = getaddrinfo(NULL, directory_phase2_port, &hints, &res)) != 0)
     {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
     }
 if ((he = gethostbyname("nunki.usc.edu")) == NULL) { // get the host info
                 herror("gethostbyname");
                    return 2;
                     }
    printf("Phase 2: The Directory Server has UDP port number %s and IP address ", directory_phase2_port);
    addr_list = (struct in_addr **)he->h_addr_list;
       for(i = 0; addr_list[i] != NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
         }
         printf("\n");
     for(p = res;p != NULL; p = p->ai_next) {
          if((socketid2=socket(p->ai_family, p->ai_socktype,p->ai_protocol))==-1)
          {perror("dirserver: socket");
          continue;
          }

          if(bind(socketid2, p->ai_addr, p->ai_addrlen)==-1){
          close(socketid2);
          perror("dirserver: bind");
          continue;
          }
          break;
       }
       if(p==NULL){
       fprintf(stderr, "dirserver: failed to bind socket\n");
       return 2;
       }
       freeaddrinfo(res);
       //printf("dirserver: waiting to recvfrom client...\n");//printf("socket2=%d\n",socketid2);



        for(i=1;i<=2;i++)
        {memset(buf,NULL,MAXBUFLEN);
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(socketid2, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);}
        //printf("buf=%s\n",buf);
        char row, doc,tempc;
        doc=buf[11];//printf("doc=%c\n",doc);//doc number
        row=buf[6];//printf("row=%c\n",row);//client number
        int servernum[3]={0,0,0};//flag to mark which file server has the certain doc
        int temp=0;
        int cost[3]={0,0,0};//cost from client to file
        int choosenum;
        if(row=='1')//client1
        {
        printf("Phase 2:The Directory Server has received request from File Server %c\n", row);
        FILE *fp;
        fp=fopen("resource.txt","r");
        fseek(fp,16,0);//change the position of fp
        while(!feof(fp))
        {
            if((tempc=fgetc(fp))==doc)servernum[temp]=1;
            if(tempc=='\n'){temp++;fseek(fp,16,1);}//make flag 1 means that the needed doc is in such file server
        }
        fclose(fp);
         //printf("servernum:%d,%d,%d\n",servernum[0],servernum[1],servernum[2]);
        fp=fopen("topology.txt","r");

       for(j=0;j<3;j++)
       {
           fscanf(fp,"%d",&temp);
           if(servernum[j]==1) cost[j]=temp;//remember the cost
       }
       fclose(fp);
       //printf("cost:%d,%d,%d\n",cost[0],cost[1],cost[2]);
       int min=cost[0];
       choosenum=1;
       for(j=1;j<3;j++)
       {
         if (min==0) {min=cost[j];continue;}
         if(min>cost[j]&&cost[j]!=0){min=cost[j];choosenum=j+1;}//find the cheapest cost
         else min=min;
       }
       //printf("%d\n",choosenum);
       char dir[3][50];
       int k;
       fp=fopen("directory.txt","r");
       for(j=0;j<3;j++){
       for(k=0;(tempc=fgetc(fp))!='\n';k++)//record the detail of chosen file server
       {
           dir[j][k]=tempc;
       }
         dir[j][k]='\0';
          fgetc(fp);
       }
       for(j=0;j<3;j++)
        {
            if((choosenum+'0')==dir[j][11])break;
        }
         //printf("%d\n",j);
        //printf("%s\n",dir[j]);
       fclose(fp);
       if ((numbytes = sendto(socketid2, dir[j], strlen(dir[j]), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {perror("file: sendto");exit(1);}
       printf("Phase 2:File server details has been sent to Client %d.\n", 1);
       //printf("%s\n",dir[j]);
       }





       if(row=='2')//client2 is the same as client1.
       {
        printf("Phase 2: The Directory Server has received request from File Server %c\n", row);
        FILE *fp;
        fp=fopen("resource.txt","r");
        fseek(fp,16,0);
        while(!feof(fp))
        {
            if((tempc=fgetc(fp))==doc)servernum[temp]=1;
            if(tempc=='\n'){temp++;fseek(fp,16,1);}
        }
        fclose(fp);
       fp=fopen("topology.txt","r");
        fscanf(fp,"%d",&temp); fscanf(fp,"%d",&temp); fscanf(fp,"%d",&temp);
       for(j=0;j<3;j++)
       {
