// 자세한 내용은 생략
// 기본 서버 베이스 및 테스트 용
// TCP/IP 기반으로 
// 에코잉 기능 갖고있음
// network programming echo server 기본 코드

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int main(int argc, char *argv[]){

	int s_sock, c_sock;
	char message[BUF_SIZE];
	int str_len, i;
	struct sockaddr_in s_adr, c_adr;
	socklen_t c_adr_sz;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	s_sock = socket(PF_INET, SOCK_STREAM,0);
	if(s_sock == -1)
		error_handling("socket() error");

	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family=AF_INET;
	s_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	s_adr.sin_port = htons(atoi(argv[1]));

	if(bind(s_sock, (struct sockaddr*)&s_adr, sizeof(s_adr))==-1)
		error_handling("bind() error");

	if(listen(s_sock,5)==-1)
		error_handling("listen() error");

	c_adr_sz = sizeof(c_adr);

	for(i=0;i<5;i++){
		c_sock = accept(s_sock, (struct sockaddr*)&c_adr,&c_adr_sz);
		if(c_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n",i+1);

		while((str_len=read(c_sock,message,BUF_SIZE))!=0)
			write(c_sock, message,str_len);

		close(c_sock);
	}
	close(s_sock);
	return 0;
}
void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

