#include "client_server.h"
#include <stdint.h>

unsigned int get_crc(int data[], int taille){
  int i,j;
     
      unsigned int poly_gen = 0xEDB88320;
      unsigned int crc=0;

      for ( j=0 ; j<taille ; j++)
    {
    crc = crc ^ data[j];
    for ( i=0 ; i<7 ; i++){

      if (crc & 1)
        {
        crc = crc ^ poly_gen;
        crc >>= 1;
      }

      else crc >>= 1;
    }
      }
     
      return(crc);
}

void testCRC(int sended_bytes, int received_bytes, int sockfd, char *buffer, char *buffer_to_send,struct sockaddr_in remote_addr, int slen)
{
      int tab[12];
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
      unsigned long result_crc= get_crc(tab,12);
      char bufferx[50];

      sprintf (bufferx, "%lu", result_crc);

    printf("waiting for response... \n");
    if((received_bytes = recvfrom(sockfd, buffer, 12, 0, (struct sockaddr *)&remote_addr, &slen)) == -1){
      perror("Recv ");
      exit(1);
    }
    buffer[received_bytes] = '\0';
    printf("Received : %s\n", buffer);
    strcpy(buffer_to_send, bufferx);
    
    if((sended_bytes = sendto(sockfd, buffer_to_send, strlen(buffer_to_send), 0, (struct sockaddr*) &remote_addr, slen)) == -1){
      perror("Send error ");
      exit(1);
    }
    buffer_to_send[sended_bytes] = '\0';

}

void greating(int sended_bytes, int received_bytes, int sockfd, char *buffer, char *buffer_to_send,struct sockaddr_in remote_addr, int slen)
{

    printf("waiting for response... \n");
    if((received_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote_addr, &slen)) == -1){
      perror("Recv ");
      exit(1);
    }
    buffer[received_bytes] = '\0';
    printf("Received : %s\n", buffer);
    printf ("msg sent :)\n");

    strcpy(buffer_to_send, "Oh God, hello from mars to earth !! \
    \n please choose one of these options \
    \n 1 : Calcule de crc \n");

    if((sended_bytes = sendto(sockfd, buffer_to_send, strlen(buffer_to_send), 0, (struct sockaddr*) &remote_addr, slen)) == -1){
      perror("Send error ");
      exit(1);
    }
    buffer_to_send[sended_bytes] = '\0';
}

