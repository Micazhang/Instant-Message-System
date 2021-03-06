#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<termios.h>

int main(int argc,char *argv[])
{
	struct sockaddr_in clientaddr; 
	pid_t pid;
	int clientfd,sendbytes,recvbytes;
	struct hostent *host;
	char *buf, *buf_r;

        struct termios term;
        long vdisable;
        vdisable=fpathconf(STDIN_FILENO,_PC_VDISABLE);
        tcgetattr(STDIN_FILENO,&term);
        term.c_cc[VINTR] = vdisable;
        term.c_cc[VINTR] = 4;
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&term);

	if(argc<4)
	{
		printf("usage:\n");
		printf("%s host port name\n",argv[0]);
		exit(1);
	}
	host=gethostbyname(argv[1]);
	if((clientfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket\n");
		exit(1);
	}
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons((uint16_t)atoi(argv[2]));
	clientaddr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(clientaddr.sin_zero),0);
	if(connect(clientfd,(struct sockaddr *)&clientaddr,sizeof(struct sockaddr_in))==-1)
	{
		perror("connect\n");
		exit(1);
	}
	buf=(char *)malloc(120);
	memset(buf,0,120);
	buf_r=(char *)malloc(100);
	if(recv(clientfd,buf,100,0)==-1)
	{
		perror("recv:");
		exit(1);
	}
	printf("\n%s\n",buf);
	pid=fork();
	while(1)
	{
		if(pid>0){
			strcpy(buf,argv[3]);
			strcat(buf,":");
			memset(buf_r,0,100);
			fgets(buf_r,100,stdin);
			strncat(buf,buf_r,strlen(buf_r)-1);
			if((sendbytes=send(clientfd,buf,strlen(buf),0))==-1)
			{
			   perror("send\n");
			   exit(1);
			}
		}
		else if(pid == 0)
		{
		   memset(buf,0,100);
			if(recv(clientfd,buf,100,0)<=0)
			{
			   perror("recv:");
			   close(clientfd);
			   raise(SIGSTOP);
			   exit(1);
			}
			printf("%s\n",buf);
		}
		else
			perror("fork");
	}
	close(clientfd);
	return 0;
}


