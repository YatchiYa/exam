#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

 void crypt(char *clearText, int key) {
   int i = 0;
   while(clearText[i] != 0x0 ) {    // 0x00 est le caractere null pour
                                    //signaler la fin du string.
      if ( (clearText[i] - 32 + key ) < 0 ) {
         // On restreint les decallages sur les caractere ASCII imprimables
         clearText[i] = (clearText[i] - 32 + key + 94) % 94 + 32; 
      } else {
         clearText[i] = (clearText[i] - 32 + key) % 94 + 32;
      }
    i++;
 }
}

void decrypt(char *codedText, int key) {
  crypt(codedText, - key);
}


void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    // decrypt(buffer, 3);
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror(" Error in bind");
    exit(1);
  }
  printf(" Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf(" Listening....\n");
	}else{
		perror(" Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");

  return 0;
}
