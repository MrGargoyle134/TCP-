#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctime>

#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#endif  /* _COLORS_ */


static const char* const Linux =
"        #####           ###            ###   \n"
"       #######         ##                  ## \n"
"       ##O#O##        ##      ##    ##      ## \n"
"       #VVVVV#        ##    ##;;#  #;;##    ## \n"
"     ##  VVV  ##       #####;;;;;##;;;;;#####  \n"
"    #           #        ###       |     ###  \n"
"   #             #         #     O\\ /|O #  # \n"
"   #    TCP++    #        #  #       \\  ## # \n"
"  QQ#            QQ      # ##     '   \\  ## # \n"
"QQQQQQ#       #QQQQQQ   #  #        o  |o # # \n"
"QQQQQQQ#     #QQQQQQQ    ##       \\_____  ## \n"
"  QQQQQ#######QQQQQ        #              #  \n"
"                            ####;;;;;;;###   \n"
"                                 ;;;;;      \n"
"                                  ;;;       \n"
"                                   ;        \n";
std::string AdrIPt;
int PORT;

class Server
{
	public:
		void SetAdr()
		{
			std::cout << "Put your IP adresse: ";
			std::cin >> AdrIPt;
		}
		void SetPort()
		{
			std::cout << "Please select a PORT number: ";
			std::cin >> PORT;
		}
};

void Intro()
{
    printf("+==============================================================================+\n");
    printf("Welcome to TCP++ v1.2, a ");
    std::cout << BOLD(FBLU("Linux/GNU TCP"));
    printf(" server. \n\n");
    puts  (Linux);
    printf("+==============================================================================+\n");
}


int main()
{

    Server server;
	
    server.SetAdr();
    server.SetPort();
	
    //Introduction
    Intro();


	//Create a socket
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // Socket AF_INET
	if(sockfd < 0) // Check if can't connect
	{
		std::cerr << BOLD(FRED("[ERROR]")) << ", can't connect\n";  // Error, can't connect
		exit(1); // EXIT_FAILURE
	}

	//Server
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(AdrIPt.c_str());

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0) // Check binding
	{
		std::cerr << BOLD(FRED("[ERROR]")) << ", binding.\n";
		exit(1);
	}
        std::cout << "IP: " << AdrIPt << std::endl;
	std::cout << "Port: " << PORT << std::endl;

	if(listen(sockfd, 10) == 0)
	{
		std::cout << "Listening....\n";
	}
	else
	{
		std::cerr << BOLD(FRED("[ERROR]")) << ", binding.\n";
	}

	char hostname[1024]; // Client's Hostname
	hostname[1023] = '\0';


	for(;;)
	{
			//Time
   		   time_t now = time(0);
		   tm *ltm = localtime(&now);

		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
			exit(1);
		}
		printf("[%02d:%02d:%02d]: New client from %s: %d!\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec,inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0)
		{
			close(sockfd);

			for(;;) // Infinite loop
			{
				gethostname(hostname,1023);
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "$exit") == 0) // If a client disconnect
				{
printf("[%02d:%02d:%02d]: Disconnected client from %s:%d\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else
				{
					printf("[%02d:%02d:%02d] %s: %s\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, hostname, buffer); // Printf message
					send(newSocket, buffer, strlen(buffer), 0); // Send the message
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket); // Close the socket


    return 0; // End of the program
}
