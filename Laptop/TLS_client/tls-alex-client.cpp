#include <curses.h>

// Routines to create a TLS client
#include "make_tls_client.h"

// Network packet types
#include "netconstants.h"

// Packet types, error codes, etc.
#include "constants.h"

// Tells us that the network is running.
static volatile int networkActive=0;

void handleError(const char *buffer)
{
	switch(buffer[1])
	{
		case RESP_OK:
			printf("Alex is OK and ready to execute movement\n");
			break;
		case RESP_DONE:
			printf("Alex has done moving :D\n");
			break;

		case RESP_BAD_PACKET:
			printf("BAD MAGIC NUMBER FROM ARDUINO\n");
			break;

		case RESP_BAD_CHECKSUM:
			printf("BAD CHECKSUM FROM ARDUINO\n");
			break;

		case RESP_BAD_COMMAND:
			printf("PI SENT BAD COMMAND TO ARDUINO\n");
			break;

		case RESP_BAD_RESPONSE:
			printf("PI GOT BAD RESPONSE FROM ARDUINO\n");
			break;

		default:
			printf("PI IS CONFUSED!\n");
	}
}

void handleStatus(const char *buffer)
{
	int32_t data[16];
	memcpy(data, &buffer[1], sizeof(data));

	printf("\n ------- ALEX STATUS REPORT ------- \n\n");
	printf("Left Forward Ticks:\t\t%d\n", data[0]);
	printf("Right Forward Ticks:\t\t%d\n", data[1]);
	printf("Left Reverse Ticks:\t\t%d\n", data[2]);
	printf("Right Reverse Ticks:\t\t%d\n", data[3]);
	printf("Left Forward Ticks Turns:\t%d\n", data[4]);
	printf("Right Forward Ticks Turns:\t%d\n", data[5]);
	printf("Left Reverse Ticks Turns:\t%d\n", data[6]);
	printf("Right Reverse Ticks Turns:\t%d\n", data[7]);
	printf("Forward Distance:\t\t%d\n", data[8]);
	printf("Reverse Distance:\t\t%d\n", data[9]);
	printf("\n---------------------------------------\n\n");
}

void handleMessage(const char *buffer)
{
	printf("MESSAGE FROM ALEX: %s\n", &buffer[1]);
}

void handleCommand(const char *buffer)
{
	// We don't do anything because we issue commands
	// but we don't get them. Put this here
	// for future expansion
}

void handleNetwork(const char *buffer, int len)
{
	// The first byte is the packet type
	int type = buffer[0];

	switch(type)
	{
		case NET_ERROR_PACKET:
		handleError(buffer);
		break;

		case NET_STATUS_PACKET:
		handleStatus(buffer);
		break;

		case NET_MESSAGE_PACKET:
		handleMessage(buffer);
		break;

		case NET_COMMAND_PACKET:
		handleCommand(buffer);
		break;
	}
}

void sendData(void *conn, const char *buffer, int len)
{
	int c;
	printf("\nSENDING %d BYTES DATA\n\n", len);
	if(networkActive)
	{
		/* TODO: Insert SSL write here to write buffer to network */
		c = sslWrite(conn, buffer, len);
		/* END TODO */	
		networkActive = (c > 0);
	}
}

void *readerThread(void *conn)
{

	while(networkActive)
	{
		int len;
		char buffer[128];
		/* TODO: Insert SSL read here into buffer */
		len = sslRead(conn, buffer, sizeof(buffer));

	        printf("read %d bytes from server.\n", len);
		
		/* END TODO */

		networkActive = (len > 0);

		if(networkActive)
			handleNetwork(buffer, len);
	}

	printf("Exiting network listener thread\n");
    
    /* TODO: Stop the client loop and call EXIT_THREAD */
	stopClient();
	EXIT_THREAD(conn);


    /* END TODO */
}

void flushInput()
{
	char c;

	while((c = getchar()) != '\n' && c != EOF);
}

void getParams(int32_t *params)
{
	printf("Enter distance/angle in cm/degrees (e.g. 50) and power in %% (e.g. 75) separated by space.\n");
	printf("E.g. 50 75 means go at 50 cm at 75%% power for forward/backward, or 50 degrees left or right turn at 75%%  power\n");
	scanf("%d %d", &params[0], &params[1]);
	flushInput();
}

void getFixedParams(int32_t* params, char ch) {
	switch (ch) {
		case 'w':
		case 's':
			params[0] = 10;
			params[1] = 75;
			break;
		case 'a':
		case 'd':
			params[0] = 15;
			params[1] = 75;
			break;
		case 'h':
		case 'k':
			params[0] = 90;
			params[1] = 70;
			break;
		case 'u':
		case 'j':
			params[0] = 30;
			params[1] = 100;
			break;
	}
}

void *writerThread(void *conn)
{
	int quit=0;

	while(!quit)
	{
		char ch;
		printf("Command (f=forward, b=reverse, l=turn left, r=turn right, s=stop, c=clear stats, g=get stats q=exit)\n");
		printf("Command (w=forward 15cm, s=reverse 15cm, a=turn left 10 degrees, d=turn right 10 degrees)\n");
		printf("Command (u=forward 30cm, j=reverse 30cm, h=turn left 90 degrees, k=turn right 90 degrees)\n");
		scanf("%c", &ch);

		// Purge extraneous characters from input stream
		flushInput();

		char buffer[10];
		int32_t params[2];

		buffer[0] = NET_COMMAND_PACKET;
		switch(ch)
		{
			case 'f':
			case 'F':
			case 'b':
			case 'B':
			case 'l':
			case 'L':
			case 'r':
			case 'R':
						getParams(params);
						buffer[1] = ch;
						memcpy(&buffer[2], params, sizeof(params));
						sendData(conn, buffer, sizeof(buffer));
						break;
		//	case 's':
		//	case 'S':
			case 'c':
			case 'C':
			case 'g':
			case 'G':
					params[0]=0;
					params[1]=0;
					memcpy(&buffer[2], params, sizeof(params));
					buffer[1] = ch;
					sendData(conn, buffer, sizeof(buffer));
					break;
			case 'q':
			case 'Q':
				quit=1;
				break;
			case 'w':	
			case 'a':
			case 'h':
			case 'k':
			case 's':
			case 'd':
			case 'u':
			case 'j':
					getFixedParams(params, ch);
					if (ch == 'w' || ch == 'u')
						buffer[1] = 'f';
					else if (ch == 'a' || ch == 'h')
						buffer[1] = 'l';
					else if (ch == 's' || ch == 'j')
						buffer[1] = 'b';
					else if (ch == 'd' || ch == 'k')
						buffer[1] = 'r';

					memcpy(&buffer[2], params, sizeof(params));
					sendData(conn, buffer, sizeof(buffer));
					break;
			default:
				printf("BAD COMMAND\n");
		}
	}

	printf("Exiting keyboard thread\n");

    /* TODO: Stop the client loop and call EXIT_THREAD */
	stopClient();
	EXIT_THREAD(conn);
    /* END TODO */
}

/* TODO: #define filenames for the client private key, certificatea,
   CA filename, etc. that you need to create a client */
//#define SERVER_NAME "172.20.10.10"
#define SERVER_NAME "192.168.43.88"
#define CA_CERT_FNAME "signing.pem"
#define PORT_NUM 5000
#define CLIENT_CERT_FNAME "laptop.crt"
#define CLIENT_KEY_FNAME "laptop.key"
#define SERVER_NAME_ON_CERT "alex.epp.com"
/* END TODO */

void connectToServer(const char *serverName, int portNum)
{
    /* TODO: Create a new client */
	createClient(SERVER_NAME, PORT_NUM, 1, CA_CERT_FNAME, SERVER_NAME_ON_CERT, 1, CLIENT_CERT_FNAME, CLIENT_KEY_FNAME, readerThread, writerThread);
    /* END TODO */
}

int main()
{
    networkActive = 1;

    /* TODO: Add in while loop to prevent main from exiting while the
    client loop is running */
    	// server name and portnumber here is hard coded here. No input parameters required when running the client server
	connectToServer(SERVER_NAME, PORT_NUM);
	while(client_is_running());

    /* END TODO */
	printf("\nMAIN exiting\n\n");
	printf("\nClient closed, have a great day!\n\n");
}
