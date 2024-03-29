/*
  RemoteShellcodeLauncher.c 
	Author: Vivek Ramachandran
	Email: vivek@securitytube.net 
	
	License: Use as you please for both commercial/non-commercial with/without attribution 
	
	Website: http://securitytube.net 
 
	Infosec Training: http://SecurityTube-Training.com 
	Checkout our Assembly Language and Shellcoding course! 
 
	Disclaimer: Written in 30 mins by salvaging my old code, might be prone to error. Please fix yourself :)
 
	Compile: gcc RemoteShellcodeLauncher.c -z execstack -fno-stack-protector -o RemoteShellcodeLauncher
 
*/
 
		
 
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<error.h>
#include<strings.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
 
#define ERROR	-1
#define MAX_DATA	2048	
#define MAX_SHELLCODE_LEN	4096
 
char shellcode[MAX_SHELLCODE_LEN];
 
main(int argc, char **argv)
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;
	int new;
	int sockaddr_len = sizeof(struct sockaddr_in);
	int data_len, shellcode_len;
	char data[MAX_DATA];
	int (*fptr)();
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("server socket: ");
		exit(-1);
	}
		
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);
			
	if((bind(sock, (struct sockaddr *)&server, sockaddr_len)) == ERROR)
	{
		perror("bind : ");
		exit(-1);
	}
	
	if((listen(sock, 1)) == ERROR)
	{
		perror("listen");
		exit(-1);
	}
	
	if((new = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == ERROR)
	{
		perror("accept");
		exit(-1);
	}
		
 
	data_len = shellcode_len = 0;
				
	do
	{
		data_len = recv(new, data, MAX_DATA, 0);
			
		if(data_len)
		{
			memcpy(&shellcode[shellcode_len], data, data_len);
			shellcode_len += data_len;
			if (shellcode_len > MAX_SHELLCODE_LEN)
			{
				printf("Received shellcode length exceeds MAX_SHELLCODE_LEN: exiting!\n");
				exit(-1);
			}
 
		}
			 
	}while(data_len);
 
		
	close(new);
	close(sock);
 
	if(shellcode_len)
	{
 
		printf("Shellcode size: %d\n", (int)strlen(shellcode));
		printf("Executing ...\n");
		fptr = (int(*)())shellcode;
		(int)(*fptr)();
				
	}
		
	return 0;		
}