#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include <arpa/inet.h>

#define PERM S_IRUSR|S_IWUSR
#define MYPORT 8666
#define BACKLOG 10
#define WELCOME "|----------Welcome to the chat room! ----------|"

void itoa( int i,char *string)
{
  int power,j;
  j=i;
  for(power=1;j>=10;j/=10)
    power*=10;
  for(;power>0;power/=10)
  {
    *string++='0'+i/power;
    i%=power;
   }
  *string='\0';
}

void get_cur_time(char *time_str)
{
  time_t timep;
  struct tm *p_curtime;
  char *time_tmp;
  time_tmp=(char *)malloc(2);
  memset(time_tmp,0,2);

  memset(time_str,0,20);
  time(&timep);
  p_curtime = localtime(&timep);
  strcat(time_str," (");
  itoa(p_curtime->tm_hour,time_tmp);
  strcat(time_str,time_tmp);
  strcat(time_str,":");
  itoa(p_curtime->tm_min,time_tmp);
  strcat(time_str,time_tmp);
  strcat(time_str,":");
  itoa(p_curtime->tm_sec,time_tmp);
  strcat(time_str,time_tmp);
  strcat(time_str,") ");
  free(time_tmp);
}

key_t shm_create()
{
  key_t shmid;
 
  if((shmid=shmget(IPC_PRIVATE,1024,PERM)) == -1)
  {
  fprintf(stderr,"Create Share Memory Error:%s\n\a" ,strerror(errno));
  exit(1);
  }
  return shmid;
}

int bindPort(unsigned short int port)
{
  int sockfd;
  struct sockaddr_in my_addr;
  sockfd = socket(AF_INET,SOCK_STREAM,0);
  my_addr.sin_family = AF_INET; 
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero),0);

  if(bind(sockfd,(struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
  {
  perror("bind");
  exit(1);
  }
  printf("bing success!\n");
  return sockfd;
}

int main( int argc, char *argv[])
{
  int sockfd,clientfd,sin_size,recvbytes;
  pid_t pid,ppid; 
  char*buf, *r_addr, *w_addr, *temp, *time_str;
  struct sockaddr_in their_addr;
  key_t shmid;

  shmid = shm_create(); 

  temp = (char *)malloc(255);
  time_str=(char *)malloc(20);
  sockfd = bindPort(MYPORT); 
  while(1)
  {
  if(listen(sockfd,BACKLOG) == -1)
  {
    perror("listen");
    exit(1);
  }
  printf("listening......\n");
  if((clientfd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size)) == -1)
  {
    perror("accept");
    exit(1);
  }
  printf("accept from:%s\n", inet_ntoa(their_addr.sin_addr));
  send(clientfd,WELCOME,strlen(WELCOME),0);
  buf = (char *)malloc(255);

  ppid = fork(); 
  if(ppid == 0)
  {
  pid = fork();
  while(1)
  {
    if(pid > 0)
     {
        memset(buf,0,255);
          if((recvbytes = recv(clientfd,buf,255,0)) <= 0)
          {
              perror("recv1");
              close(clientfd);
              raise(SIGKILL);
              exit(1);
           }
           w_addr = shmat(shmid, 0, 0);
           memset(w_addr, '\0' , 1024);
           strncpy(w_addr, buf, 1024);
           get_cur_time(time_str);
           strcat(buf,time_str);
           printf(" %s\n",buf);
     }
     else if(pid == 0)
     {
         sleep(1);
         r_addr = shmat(shmid, 0, 0);
         if(strcmp(temp,r_addr) != 0)
         {
            strcpy(temp,r_addr);
            get_cur_time(time_str);
            strcat(r_addr,time_str);
            if(send(clientfd,r_addr,strlen(r_addr),0) == -1)
            {
               perror("send");
            }
            memset(r_addr, '\0', 1024);
            strcpy(r_addr,temp);
          }
      }
     else
       perror("fork");
     }
    }
  }
  printf("------------------------------\n");
  free(buf);
  close(sockfd);
  close(clientfd);
  return 0;
}
