// Client side - UDP Code				    
// By Hugh Smith	4/1/2017		

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#include "gethostbyname.h"
#include "networks.h"
#include "safeUtil.h"
#include "pduFunc.h"
#include "cpe464.h"

#define MAXBUF 80

void talkToServer(int socketNum, struct sockaddr_in6 * server);
int readFromStdin(char * buffer);
void checkArgs(int argc, char * argv[]);

void rcopyControl(int socketNum,  struct sockaddr_in6 * server){
	int pollCheck;
	uint8_t sendBuf[MAXBUF];
	int sendLen;
	setupPollSet();
	addToPollSet(socketNum);
	addToPollSet(STDIN_FILENO);
	printf("Enter Data: \n");
	while(1){
		/*begin the process of asking the user for their message*/
		
		pollCheck = pollCall(-1);
		if(pollCheck < 0){
			printf("pollCall() Timed Out\n");
		}
		else if(pollCheck == socketNum){
			/*Server sent a message*/
			/*recvLen = safeRecvfrom(socketNum, buffer, MAXBUF, 0, (struct sockaddr *) server, &serverAddrLen);
		
			// print out bytes received
			ipString = ipAddressToString(server);
			printf("Server with ip: %s and port %d said it received %s\n", ipString, ntohs(server->sin6_port), buffer);
			printPDU((uint8_t *)buffer, recvLen);*/
			/*printPDU(buffer, recvLen);*/
			printf("Server sent a message back\n");
		}
		else if(pollCheck == STDIN_FILENO){
			talkToServer(socketNum, server);
		}
	}
}


int main (int argc, char *argv[])
 {
	/* rcopy .1 localhost 44444 */
	int socketNum = 0;				
	struct sockaddr_in6 server;		// Supports 4 and 6 but requires IPv6 struct
	int portNumber = 0;
	double errRate = 0;
	
	checkArgs(argc, argv);
	portNumber = atoi(argv[3]); 
	socketNum = setupUdpClientToServer(&server, argv[2], portNumber);

	errRate = atof(argv[1]);
	sendErr_init(errRate, DROP_ON, FLIP_ON, DEBUG_ON, RSEED_OFF);
	
	rcopyControl(socketNum, &server);
	/*talkToServer(socketNum, &server);*/
	
	close(socketNum);

	return 0;
}

void talkToServer(int socketNum, struct sockaddr_in6 * server)
{
	int serverAddrLen = sizeof(struct sockaddr_in6);
	char * ipString = NULL;
	int dataLen = 0; 
	char payLoadbuffer[MAXBUF+1];
	int totalLen = 0;

	int counter = 0;
	
	payLoadbuffer[0] = '\0';
	while (payLoadbuffer[0] != '.')
	{
		char buffer[MAXBUF+8];
		int recvLen = 0;
		dataLen = readFromStdin(payLoadbuffer);

		/*printf("Sending: %s with len: %d\n", buffer,dataLen);*/
		totalLen = createPDU((uint8_t *)buffer, counter, 0, (uint8_t *)payLoadbuffer, dataLen);
		counter++; 
		printPDU((uint8_t *)buffer, totalLen);

		safeSendto(socketNum, buffer, totalLen, 0, (struct sockaddr *) server, serverAddrLen);
		
		/*recvLen = safeRecvfrom(socketNum, buffer, MAXBUF, 0, (struct sockaddr *) server, &serverAddrLen);
		
		// print out bytes received
		ipString = ipAddressToString(server);
		printf("Server with ip: %s and port %d said it received %s\n", ipString, ntohs(server->sin6_port), buffer);
	    printPDU((uint8_t *)buffer, recvLen);*/
		/*printPDU(buffer, recvLen);*/
	}
}

int readFromStdin(char * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
	/*printf("Enter data: ");*/
	while (inputLen < (MAXBUF - 1) && aChar != '\n')
	{
		aChar = getchar();
		if (aChar != '\n')
		{
			buffer[inputLen] = aChar;
			inputLen++;
		}
	}
	
	// Null terminate the string
	buffer[inputLen] = '\0';
	inputLen++;
	
	return inputLen;
}

void checkArgs(int argc, char * argv[])
{

       /*int portNumber = 0;*/
	
        /* check command line arguments  */
	if (argc != 4)
	{
		printf("usage: %s [error-percentage] [host-name] port-number \n", argv[0]);
		exit(1);
	}
	
	/*portNumber = atoi(argv[2]);*/
		
	return ;
}





