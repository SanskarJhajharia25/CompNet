#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_INPUT_SIZE 256
//Our program takes the input of the server IP address on the terminal. The port number is also taken as an input from the user
// while executing the program, you are supposed to enter the IP and the port number
int main(int argc, char *argv[])
{
  int sockfd, portnum, n;
  struct sockaddr_in server_addr;

  char inputbuf[MAX_INPUT_SIZE];
  if (argc < 3) {
    fprintf(stderr,"You havent specified all the details of the client program. We expect 2 command line arguements too. please enter in the format ./a.out ip_addr port_num");
    exit(0);
  }

  portnum = atoi(argv[2]); //extracting the port number from the command line arguments.

    /* Create client socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
      {
      	fprintf(stderr, "ERROR opening socket\n");
	      exit(1);
      }

    /* Fill in server address */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton(argv[1], &server_addr.sin_addr)) //checking if the server address is valid
      {
	fprintf(stderr, "ERROR invalid server IP address\n");
	exit(1);
      }
    server_addr.sin_port = htons(portnum);

    /* Connect to server */
    if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
      {
	fprintf(stderr, "ERROR connecting\n");
	exit(1);
      }
    printf("Connected to server\n");
    
    do
      {
    	/* Ask user for message to send to server */
	    printf("Enter request: ");
	    bzero(inputbuf,MAX_INPUT_SIZE); //clearing the buffer
	    fgets(inputbuf,MAX_INPUT_SIZE-1,stdin);
	
	    /* Write to server */
	    n = write(sockfd,inputbuf,strlen(inputbuf));
	    if (n < 0) 
	    {
	      fprintf(stderr, "ERROR writing to socket\n");
	      exit(1);
	    }
	
    	/* Read reply */
	    bzero(inputbuf,MAX_INPUT_SIZE);
	    n = read(sockfd,inputbuf,(MAX_INPUT_SIZE-1));
	    if (n < 0) 
	    {
	      fprintf(stderr, "ERROR reading from socket\n");
	      exit(1);
	    }
	    printf("Server replied: %s\n",inputbuf);

      } while(!(inputbuf[0] == 'G' && inputbuf[1] == 'o' && inputbuf[2] == 'o' && inputbuf[3] == 'd' && inputbuf[4] == 'b' && inputbuf[5] == 'y' && inputbuf[6] == 'e'));
    //the loop runs as long as the server doesnt reply with "Goodbye"
    return 0;
}


// /*
// #include <stdio.h>
// #include <winsock2.h>
// #include <sys/socket.h>  //for socket(), connect(), send(), recv() functions
// #include <arpa/inet.h>  // different address structures are declared here
// #include <stdlib.h>   // atoi() which convert  string to integer
// #include <string.h>   
// #include <unistd.h>  // close() function
// #define BUFSIZE 32
// int main()
// {
//  /* CREATE A TCP SOCKET*/
//  int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//  if (sock < 0) { printf ("Error in opening a socket"); exit (0);}
//  printf ("Client Socket Created\n");

// /*CONSTRUCT SERVER ADDRESS STRUCTURE*/
//  struct sockaddr_in serverAddr;
//  memset (&serverAddr,0,sizeof(serverAddr)); 

// /*memset() is used to fill a block of memory with a particular value*/
//  serverAddr.sin_family = AF_INET;
//  serverAddr.sin_port = htons(32345); //You can change port number here
//  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Specify server's IP address here
//  printf ("Address assigned\n");
 
// /*ESTABLISH CONNECTION*/
//  int c = connect (sock, (struct sockaddr*) &serverAddr , sizeof (serverAddr));
//  printf ("%d\n",c);
//  if (c < 0) 
//    { printf ("Error while establishing connection"); 
//      exit (0);
//    }
//  printf ("Connection Established\n");
 
// /*SEND DATA*/
//  printf ("ENTER MESSAGE FOR SERVER with max 32 characters\n");
//  char msg[BUFSIZE];
//  fgets(msg,BUFSIZE,stdin);
//  int bytesSent = send (sock, msg, strlen(msg), 0);
//  if (bytesSent != strlen(msg)) 
//    { printf("Error while sending the message"); 
//      exit(0);
//    }
//  printf ("Data Sent\n");

//  /*RECEIVE BYTES*/
//  char recvBuffer[BUFSIZE];
//  int bytesRecvd = recv (sock, recvBuffer, BUFSIZE-1, 0);
//  if (bytesRecvd < 0) 
//    { printf ("Error while receiving data from server"); 
//      exit (0);
//    }
//  recvBuffer[bytesRecvd] = '\0';
//  printf ("%s\n",recvBuffer);

// close(sock);
// }
