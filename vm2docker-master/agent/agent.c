#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#include "cmds.h"

void process_client(int clientfd, struct sockaddr_in * client_addr);
bool process_cmd(char *cmd, int clientfd);

int main(int argc, char *argv[]) {
    // verify that user is running as root
    uid_t uid=getuid(), euid=geteuid();
    if (euid != 0 && uid != 0) {
        printf("The agent must be run as root. Please try again\n");
        exit(errno);
    }

	int sockfd;
	struct sockaddr_in self;

	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("Socket");
		exit(errno);
	}

	memset(&self, 0, sizeof(self));
	self.sin_family = AF_INET;
	char *port_str;
	if (argc == 2) {
	    port_str = argv[1];
	} else {
	    port_str = getenv("AGENT_PORT");
	}
	int port;
	if (port_str == NULL) {
	    port = DEFAULT_AGENT_PORT;
	} else {
	    port = atoi(port_str);
	}
	printf("Starting agent on port %d\n", port);
	self.sin_port = htons(port);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 ) {
		perror("socket--bind");
		exit(errno);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 ) {
		perror("socket--listen");
		exit(errno);
	}

	/*---Forever... ---*/
	while (true) {	
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

		// Only accept one client at a time, this is desired behavior
		process_client(clientfd, &client_addr);
		
		/*---Close data connection---*/
		close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}


#define CMD_BUFFER 1024

void process_client(int clientfd, struct sockaddr_in * client_addr) {
	char buffer[CMD_BUFFER]; // maximum command length is 1024 bytes, this should be fine for our uses

	printf("%s:%d connected\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

	while (true) {
	    // TODO: consider a ring buffer here also that reads indefinitely until the "/r/n" sequence

		// accept messages indefinitely until the client closes the connection
		ssize_t msg_sz = recv(clientfd, buffer, CMD_BUFFER - 1, 0);
		// write a null character at the end of the msg to terminate the string
		*(buffer + msg_sz) = '\0';
		if (msg_sz == 0 || !process_cmd(buffer, clientfd)) {
			printf("%s:%d connection closed\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
			break;
		}
	}
}

/*
 returns whether or not to process another command after this one


for now, my commands seem to only have at most one argument, so I am going to stray away from complicated libraries until absolutely necessary 

*/
bool process_cmd(char *cmd, int clientfd) {
	printf("CMD: %s", cmd);
	// take the new line character out of the end of the cmd
	char *newline = strchr(cmd, '\n');
	if (newline == NULL) {
		printf("expected new line!;\n");
		return false;
	}
	*newline = '\0';

	// check the previous character for \r, null it if it is
	if (*(newline - 1) == '\r') {
		*(newline - 1) = '\0';
	}

	char *space = strchr(cmd, ' ');
	// arg may be null if no spaces are found

	char *arg = NULL;

	if (space != NULL) {
		arg = space + 1;
		*space = '\0'; // terminate the string so that the cmd string is legit
	}
	
	if (strcmp(cmd, EXIT_CMD) == 0) {
	    char * empty = "";
	    send(clientfd, empty, 1, 0);
		return false;
	} else if (strcmp(cmd, GET_DEPS_CMD) == 0) {
	    if (arg == NULL) {
	        printf("expected argument with get dependencies\n");
	        return false;
	    } else {
	        get_dependencies(arg, clientfd);
	    }
	} else if (strcmp(cmd, GET_FS_CMD) == 0) {
		get_filesystem(arg, clientfd);
	} else if(strcmp(cmd, GET_INSTALLED_CMD) == 0) {
		get_installed(clientfd);
	} else if(strcmp(cmd, GET_BOUND_SOCKETS_CMD) == 0) {
	    get_bound_sockets(clientfd);
	} else if(strcmp(cmd, GET_PS_CMD) == 0) {
	    get_ps(clientfd);
	} else if(strcmp(cmd, GET_PROC_CMD) == 0) {
	    if (arg == NULL) {
	        printf("expected argument with get_active_processes\n");
	        return false;
	    } else {
	        get_active_processes(arg, clientfd);
	    }
	} else if(strcmp(cmd, GET_PID_CMD) == 0) {
	    get_pid(clientfd);
	} else {
		char * unknown = UNKNOWN_CMD;
		int len = strlen(unknown);
		send(clientfd, unknown, len + 1, 0);
	}

	
	return true;
}
