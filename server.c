#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXPENDING 5
#define BUFFERSIZE 32

struct key_value
{
  int key;
  char value[100];
};
int main ()
{
  pid_t childpid; 
/*CREATE A TCP SOCKET*/
  int serverSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0) { printf ("Error while server socket creation"); exit (0); } 
  printf ("Server Socket Created\n"); 

 /*CONSTRUCT LOCAL ADDRESS STRUCTURE*/
  struct sockaddr_in serverAddress, clientAddress;
  memset (&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(32345);
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  printf ("Server address assigned\n");
 
  int temp = bind(serverSocket, (struct sockaddr*) &serverAddress,  sizeof(serverAddress));
  if (temp < 0) 
    { printf ("Error while binding\n"); 
      exit (0);
    }
  printf ("Binding successful\n");
  int temp1 = listen(serverSocket, MAXPENDING);
  if (temp1 < 0) 
  {  printf ("Error in listen"); 
     exit (0);
  }
  printf ("Now Listening\n");
  char msg[BUFFERSIZE];
  struct key_value * kv[1000] ;
  for(int i=0;i<1000;i++)
    kv[i] = (struct key_value *)malloc(sizeof(struct key_value));
  for(int i=0;i<1000;i++)
  {
    kv[i]->key=-1;
    strcpy(kv[i]->value, "dummy");
  }
  for ( ; ; ) 
  {
    int clientLength = sizeof(clientAddress);
    int clientSocket = accept (serverSocket, (struct sockaddr*) &clientAddress, &clientLength);
    if (clientLength < 0) {printf ("Error in client socket"); exit(0);}
    printf("%s\n","Received request...\n");
    if ( (childpid = fork ()) == 0 )
      printf ("%s\n","Child created for dealing with client requests\n");
    printf ("Handling Client %s\n", inet_ntoa(clientAddress.sin_addr));
    int temp2 = recv(clientSocket, msg, BUFFERSIZE, 0);

    msg[temp2] = '\0';
    printf ("%s\n", msg);

    if(msg[0]=='p' && msg[1]=='u' && msg[2]=='t')
    {
      int pos=4;
      int num=0;
      while(msg[pos]!=' ')
      {
        num=num*10 + (msg[pos]-'0');
        pos++;
      }
      for(int i=0;i<1000;i++)
      {
        if(kv[i]->key==num)
        {
          strcpy(msg, "Key Already exists");
          int bytesSent = send (clientSocket,msg,strlen(msg),0);
          break;
        }
      }
      char* va = (char *)malloc(sizeof(char)*100);
      int pp=0;
      pos++;
      while(msg[pos]!='\0')
        {
          va[pp++]=msg[pos++];
        }
      
      for(int i=0;i<1000;i++)
      {
        if(kv[i]->key == -1)
        {
          kv[i]->key=num;
          strcpy(kv[i]->value,va);
          strcpy(msg, "OK");
          int bytesSent = send (clientSocket,msg,strlen(msg),0);
          break;
        }
      }
    }

    else if(msg[0]=='g' && msg[1]=='e' && msg[2]=='t')
    {
      int pos=4;
      int num=0;
      while(msg[pos]!='\0')
      {
        num=num*10 + (msg[pos]-'0');
        pos++;
      }
      for(int i=0;i<1000;i++)
      {
        if(kv[i]->key==num)
        {
          strcpy(msg, kv[i]->value);
          int bytesSent = send (clientSocket,msg,strlen(msg),0);
          break;
        }
      }
    }
    else if(msg[0] =='B' && msg[1]=='y' && msg[2]=='e')
      {
        strcpy(msg, "Goodbye");
        int bytesSent = send (clientSocket,msg,strlen(msg),0);
        break;
      }
    else if(msg[0] == 'd' && msg[1]=='e' && msg[2]=='l')
      {
        int pos=4;
      int num=0;
      while(msg[pos]!='\0')
      {
        num=num*10 + (msg[pos]-'0');
        pos++;
      }
      for(int i=0;i<1000;i++)
      {
        if(kv[i]->key==num)
        {
          kv[i]->key=-1;
          kv[i]->value='\0';
          strcpy(msg,"OK")
          int bytesSent = send (clientSocket,msg,strlen(msg),0);
          break;
        }
      }
      }
      // char* va = (char *)malloc(sizeof(char)*100);
      // int pp=0;
      // pos++;
      // while(msg[pos]!='\0')
      //   {
      //     va[pp++]=msg[pos++];
      //   }
      
      // for(int i=0;i<10000;i++)
      // {
      //   if(kv[i]->key == -1)
      //   {
      //     kv[i]->key=num;
      //     strcpy(kv[i]->val,va);
      //     strcpy(msg, "OK");
      //     int bytesSent = send (clientSocket,msg,strlen(msg),0);
      //     break;
      //   }
      // }
    }
  
  


      // printf ("ENTER MESSAGE FOR CLIENT\n");
  
      //fgets(msg,BUFFERSIZE,stdin);
 
      // int bytesSent = send (clientSocket,msg,strlen(msg),0);
      // if (bytesSent != strlen(msg)) 
      //   { printf ("Error while sending message to client");   
      //     exit(0);
      //   }
    close (clientSocket);
  break;
  }
return;
}

