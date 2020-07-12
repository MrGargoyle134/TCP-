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


using namespace std;


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


int main()
{
    string AdrIPt;
    int PORT = 53514;
    cout << "Put your IP adresse: ";
    cin >> AdrIPt;
    cout << "Please select a PORT number: ";
    cin >> PORT;
	
    //Introduction
    printf("+==============================================================================+\n");
    printf("Welcome to TCP++ v1.2, a ");
    cout << BOLD(FBLU("Linux/GNU TCP"));
    printf(" server. \n\n");
    puts  (Linux);
    printf("+==============================================================================+\n");

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		cout << BOLD(FRED("[ERROR]")) << ", can't connect\n"; 
		exit(1);
	}

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(AdrIPt.c_str());

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		cout << BOLD(FRED("[ERROR]")) << ", binding.\n";
		exit(1);
	}
        cout << "IP: " << AdrIPt << endl;
	printf("Port: %d\n", PORT);

	if(listen(sockfd, 10) == 0){
		cout << "Listening....\n";
	}else{
		cout << BOLD(FRED("[ERROR]")) << ", binding.\n";
	}

	char hostname[1024];
	hostname[1023] = '\0';


	for(;;){
			//Time
   time_t now = time(0);
   tm *ltm = localtime(&now);
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("[%d:%d:%d]: New client from %s: %d!\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec,inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			for(;;){
				gethostname(hostname,1023);
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, "$exit") == 0){
printf("[%d:%d:%d]: Disconnected client from %s:%d\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					cout <<"["<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<"] "<< hostname << BOLD(FWHT(": ")) << buffer << endl;
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


    return 0;
}
