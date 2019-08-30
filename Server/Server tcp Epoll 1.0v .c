#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 5

#define PORT 9000

void error_handling(char *message);
int main(){

	int clinets[EPOLL_SIZE] = {0};
	int clinet_num = 1;

	int s_sock, c_sock;
	char message[BUF_SIZE];
	int str_len, i;
	struct sockaddr_in s_adr, c_adr;
	socklen_t c_adr_sz;

	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd, event_cnt;
	s_sock = socket(PF_INET, SOCK_STREAM,0);
	if(s_sock == -1)
		error_handling("socket() error");

	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family=AF_INET;
	s_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	s_adr.sin_port = htons(PORT);

	if(bind(s_sock, (struct sockaddr*)&s_adr, sizeof(s_adr))==-1)
		error_handling("bind() error");

	if(listen(s_sock,5)==-1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);													// 정해놓은 사이즈 만큼 epoll 생성
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);							// epoll에 맞는 배열 생성

	event.events=EPOLLIN;																 
	event.data.fd=s_sock;			
	clinets[0] = s_sock;																// 연결된 클라이언트 리스트, 0번은 서버소켓
	epoll_ctl(epfd,EPOLL_CTL_ADD,s_sock,&event);
	while(1){	//////////////////////////////////////////////////////// 서버 반복부
		event_cnt = epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);							// event_cnt : 이벤트가 발생한 소켓 수
																						// ep_events : 이벤트가 발생한 소켓들이 저장된 배열
		if(event_cnt == -1)
		{
			puts("epoll_wait() error");
			break;
		}

		puts("return epoll_wait");
		for(i = 0 ; i<event_cnt;i++)
		{	//////////////////////////////////////////////////////////// 이벤트 확인부
			if(ep_events[i].data.fd==s_sock)											////// 이벤트가 발생한 소켓이 서버소켓일 경우
			{
				c_adr_sz = sizeof(c_adr);
				c_sock = accept(s_sock,(struct sockaddr*)&c_adr,&c_adr_sz);
				event.events = EPOLLIN;
				event.data.fd= c_sock;
				epoll_ctl(epfd,EPOLL_CTL_ADD,c_sock,&event);							// 해당 클라이언트 소켓을 epoll에 추가

				clinets[clinet_num] = c_sock;											// 클라이언트가 연결되면 클라이언트 배열에 추가
				clinet_num++;															// 개수 변경

				printf("connected client: %d \n",c_sock);
			}
			else																		////// 이벤트가 클라이언트 소켓에서 발생한 경우
			{
				str_len = read(ep_events[i].data.fd,message,BUF_SIZE);
				if(str_len ==0){														////// 연결해제가 발생했을 경우

					///////////////////////////////////////////////////////				// 연결된 클라이언트에서 하나가 빠질때 배열을 정리하는 부분
					clinet_num--;
					for (int j = 1;; j++) {
						if (clinets[j] == ep_events[i].data.fd) {						// 해제된 클라이언트를 찾은 후 해당 기점을 기준으로 앞으로 한칸씩 당김
							for (int k = j; k != EPOLL_SIZE - 1; k++) {
								clinets[k] = clinets[k + 1];
								if (clinets[k + 1] == 0)
									break;
							}
							break;
						}
					}
					///////////////////////////////////////////////////////

					epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d\n",ep_events[i].data.fd);
				}
				else
				{																		////// 메시지 도착한 경우
					//write(ep_events[i].data.fd,message,str_len);
					for (int j = 1; j < clinet_num; j++) {
						write(clinets[j], message, str_len);
					}
				}
			}
		}
	}
	close(s_sock);
	close(epfd);		
	return 0;
}
void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

