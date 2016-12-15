#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

#define MAX_BUFFER_SIZE 1000

int startServer(int portNum) {
	int sfd;
	struct sockaddr_in server_addr;
	printf("Starting the server... port:%d\n", portNum);
	sfd = socket(PF_INET, SOCK_DGRAM, 0);

	if (sfd < 0) {
		printf("Socket open failure\n");
		exit(0);
	}
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	server_addr.sin_port = htons(portNum);

	/**binding socket to the required port**/
	if (bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("binding error");
		exit(0);
	}
	printf("**Server Binding set to :%d\n", server_addr.sin_addr.s_addr);
	printf("**Server Binding set to port:%d\n", server_addr.sin_port);
	printf("**Server Binding set to family:%d\n", server_addr.sin_family);
	printf("UDP server started successfully\n");
	return sfd;
}


int start_processing(int sfd) {
	// printf("***processing processHttpRequests for %d\n", clientSockFd);
	char buffer[MAX_BUFFER_SIZE];
	struct sockaddr_in client_address;
	socklen_t addr_size;
	char *finalHttpResponse;
	int bytes_rcvd = 0;
	int bytes_to_send = 0;
	char append_string[8]; 
	strncpy(append_string, ":abcdef", 7);


	// char test_str[12];
	// strncpy(test_str, "sup", 3);
	// // printf("test_str: %s\n", test_str);
	// strncpy(&test_str[0]+3, append_string, 7);
	// printf("test_str new: %s\n", test_str);



	while (1) {
		bzero(buffer, MAX_BUFFER_SIZE);
		printf("Waiting for THE REQUEST\n");
		addr_size = sizeof(client_address);
		/* Read from the socket for new HTTP Requests */
		while ((bytes_rcvd = recvfrom(sfd, buffer, MAX_BUFFER_SIZE, 0,
				(struct sockaddr *) &client_address, &addr_size)) <= 0)
			;
		printf("**Request received is::%s\n", buffer);

		/* Parse the HTTP request */
		// httpRequestObj = parseHttpRequest(buffer, httpRequestObj);

		// printf("After processGetRequest::\n");

		/*Create the HTTP response by reading the requested file*/
		// finalHttpResponse = formulateHttpResponse(clientSockFd, httpRequestObj);

		/*Send the HTTP response back to the client*/

		strncpy(&buffer[0]+bytes_rcvd, append_string, strlen(append_string));

		printf("To Send:: %s",buffer);
		bytes_to_send = bytes_rcvd+strlen(append_string);

		sendto(sfd,
					buffer , bytes_to_send,
					0, &client_address, sizeof(client_address));

		printf("Sent back::%s\n", buffer);
		// sendHttpResponse(clientSockFd, client_address, finalHttpResponse);

	}
}



int main() {

	int port_no = 8009;
	int sfd=-1;
	sfd = startServer(port_no);
	start_processing(sfd);
	return 1;


}



// create_socket(int port_no, char *ip_addr) {
// 	struct hostent *server;
// 	struct sockaddr_in server_address;
// 	int sfd;
// 	sfd = socket(AF_INET, SOCK_DGRAM, 0);
// 	if (sfd < 0)
// 		printf("ERROR opening socket\n");
// 	server = gethostbyname(serverAddress);
// 	if (server == NULL) {
// 		fprintf(stderr, "ERROR, no such host\n");
// 		exit(0);
// 	}
// 	bzero((char *) &server_address, sizeof(server_address));
// 	server_address.sin_family = AF_INET;
// 	bcopy((char *) server->h_addr,
// 	(char *)&server_address.sin_addr.s_addr,
// 	server->h_length);
// 	server_address.sin_port = htons(port_no);

// 	return sfd;
// }
