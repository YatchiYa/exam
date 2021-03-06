#include "client_server.h"

int main(int argc, char **argv){

  if(argc != 3){
    puts("Usage : ./client.o hostname port");
    puts("example : ./client.o localhost 400 \n");
    exit(1);
  }


  /* Hostname to ip Starts here */
  // we try to select a list  of hostnames 
  char ip[100];
  struct hostent *he;
  struct in_addr **addr_list;

  // if we can't get the hostname like localhost or ip x.x.x.x
  if ((he = gethostbyname(argv[1])) == NULL)
  {
      herror("gethostbyname error :");
      return 1;
  }
  //# set the adress host list
  addr_list = (struct in_addr **) he->h_addr_list;

  // get the ip of each one
  for(int i = 0; addr_list[i] != NULL; i++)
  {
      strcpy(ip , inet_ntoa(*addr_list[i]) );
  }
  
  // configur the socket fd
  int sockfd, received_bytes, sended_bytes;
  char buffer[1024], buffer_to_send[1024];  // just for our msg

  // set the sockets
  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;

  int slen = sizeof(remote_addr);
  int mode = 0;
  int mode_2 = 0;
  int tab [12];

  // while loop 
  while(1)
  {
    // get our udp soket fd an check if no error occure while initialising our socket fd
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
      perror("Socket ");
      exit(1);
    }

    // set the configuration and the port we defined in argument 
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(argv[2]));
    remote_addr.sin_addr.s_addr = inet_addr(ip);
    memset(remote_addr.sin_zero, '\0', sizeof(remote_addr.sin_zero));

    // try to establish the connection
    int remote_connect = connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
    if(remote_connect == -1){
      perror("Connect ");
      exit(1);
    }

    // sending a msg :
    if (mode == 0 && mode_2 == 0){
      // printf("do you want to send trame 01234567100x1020 : ");

      printf("Hi : ");
      tab[0]=0;
      tab[1]=1;
      tab[2]=2;
      tab[3]=3;
      tab[4]=4;
      tab[5]=5;
      tab[6]=6;
      tab[7]=7;
      tab[8]=1;
      tab[9]=1;
      tab[10]=0;
      tab[11]=0x1020;
      //strcpy(buffer_to_send, "01234567100x1020");
      fgets(buffer_to_send, strlen(buffer_to_send), stdin);
      //strcpy(buffer_to_send, "01234567100x1020");
    }
    else{
      strcpy(buffer_to_send, "5");
      mode = 0;
      mode_2 = 0;
    }

    // try to send the msg and check error
    if((sended_bytes = sendto(sockfd, buffer_to_send, strlen(buffer_to_send), 0, (struct sockaddr*) &remote_addr, slen)) == -1){
      perror("Send ");
      exit(1);
    }
    buffer[sended_bytes] = '\0';

    printf("waiting for response... \n");
    // recieve a msg and check error 
    if((received_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote_addr, &slen)) == -1){
      perror("Recv ");
      exit(1);
    }
    buffer[received_bytes] = '\0';
    if (strcmp(buffer, "0") == 0){
      mode = 1;
      printf("Received : chat closed\n");
    }
    else{
      printf("Received : %s\n", buffer);
    }
  }

  return 0;
}