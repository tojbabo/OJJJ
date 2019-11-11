#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<pthread.h>

#define BUF_SIZE 1024
#define EPOLL_SIZE 5

#define PORT 9000

void error_handling(char *message);
void check_clients(int);
void* recv_pic(void*);
void* send_pic(void*);

int s_sock;
int clients[EPOLL_SIZE] = {0};
char msgs[EPOLL_SIZE][BUF_SIZE];
int clients_num = 1;
int nums[EPOLL_SIZE];

int main() {
	pthread_t thread_recv;
	pthread_t thread_send;
	char* target;
	char* target_from;
	int arg[3] = { 0 };

	int c_sock;
	char message[BUF_SIZE];
	int str_len, i;
	struct sockaddr_in s_adr, c_adr;
	socklen_t c_adr_sz;

	struct epoll_event *ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	s_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (s_sock == -1)
		error_handling("socket() error");

	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family = AF_INET;
	s_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_adr.sin_port = htons(PORT);

	if (bind(s_sock, (struct sockaddr*)&s_adr, sizeof(s_adr)) == -1)
		error_handling("bind() error");

	if (listen(s_sock, 5) == -1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);													// 정해놓은 사이즈 만큼 epoll 생성
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);							// epoll에 맞는 배열 생성

	event.events = EPOLLIN;
	event.data.fd = s_sock;
	clients[0] = s_sock;																// 연결된 클라이언트 리스트, 0번은 서버소켓
	epoll_ctl(epfd, EPOLL_CTL_ADD, s_sock, &event);
	while (1) {	//////////////////////////////////////////////////////// 서버 반복부
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);							// event_cnt : 이벤트가 발생한 소켓 수
																						// ep_events : 이벤트가 발생한 소켓들이 저장된 배열
		if (event_cnt == -1)
		{
			puts("epoll_wait() error");
			break;
		}

		for (i = 0; i < event_cnt; i++)
		{	//////////////////////////////////////////////////////////// 이벤트 확인부
			if (ep_events[i].data.fd == s_sock)											////// 이벤트가 발생한 소켓이 서버소켓일 경우
			{
				c_adr_sz = sizeof(c_adr);
				c_sock = accept(s_sock, (struct sockaddr*)&c_adr, &c_adr_sz);				// 연결이 발생했을때

				printf("NEW CONNECT : %d\n", c_sock);

				str_len = read(c_sock, message, BUF_SIZE);
				message[str_len] = 0;
				printf("%s\n", message);


				if (!strcmp(message, "FC")) {
					// 기본 연결
					write(c_sock, "FC", strlen("FC"));
					sleep(1);
					sprintf(message, "%d", c_sock);
					write(c_sock, message, BUF_SIZE);
					puts("FIRST CONNECTING");
					event.events = EPOLLIN;
					event.data.fd = c_sock;
					epoll_ctl(epfd, EPOLL_CTL_ADD, c_sock, &event);							// 해당 클라이언트 소켓을 epoll에 추가

					clients[clients_num] = c_sock;											// 클라이언트가 연결되면 클라이언트 배열에 추가
					clients_num++;															// 개수 변경

				}
				else if ((target = strstr(message, "FSC")) != NULL) {
					// 데이터(얼굴 영상) 전송 연결
					write(c_sock, "FSC", strlen("FSC"));
					puts("FACE SEND CONNECTING");
					printf("CONNECT FROM %d\n", atoi(target+4));
					arg[0] = c_sock;
					arg[1] = atoi(target+4);
					/////////////////////////////////////////////////////////// 영상 수신 쓰레드 생성부
					if (pthread_create(&thread_recv, NULL, recv_pic, (void*)arg) != 0) {
						puts("pthread_create() error!");
						return -1;
					}
					///////////////////////////////////////////////////////////
					check_clients(arg[1]);
				}
				else if ((target = strstr(message, "TRC")) != NULL) {
					write(c_sock, "TRC", strlen("TRC"));
					puts("TARGET RECEIVE CONNETING");
					target_from = strchr(message, '-') + 1;
					printf("%d to %d\n", atoi(target+4), atoi(target_from));
					arg[0] = c_sock;
					arg[1] = atoi(target+4);
					arg[2] = atoi(target_from);
					/////////////////////////////////////////////////////////// 영상 송신 쓰레드 생성부
					if (pthread_create(&thread_send, NULL, send_pic, (void*)arg) != 0) {
						puts("pthread_create() error!");
						return -1;
					}
					///////////////////////////////////////////////////////////
				}
				else {
					printf("%d is closed", c_sock);
					close(c_sock);
				}
			}
			else																		////// 이벤트가 클라이언트 소켓에서 발생한 경우
			{
				str_len = read(ep_events[i].data.fd, message, BUF_SIZE);
				if (str_len == 0) {														////// 연결해제가 발생했을 경우

					///////////////////////////////////////////////////////				// 연결된 클라이언트에서 하나가 빠질때 배열을 정리하는 부분
					clients_num--;
					for (int j = 1;; j++) {
						if (clients[j] == ep_events[i].data.fd) {						// 해제된 클라이언트를 찾은 후 해당 기점을 기준으로 앞으로 한칸씩 당김
							for (int k = j; k != EPOLL_SIZE - 1; k++) {
								clients[k] = clients[k + 1];
								if (clients[k + 1] == 0)
									break;
							}
							break;
						}
					}
					///////////////////////////////////////////////////////

					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d\n", ep_events[i].data.fd);
				}
				else
				{																		////// 메시지 도착한 경우
					//write(ep_events[i].data.fd,message,str_len);
					for (int j = 1; j < clients_num; j++) {
						write(clients[j], message, str_len);
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
void check_clients(int sock) {															// 새로운 클라이언트 및 기존의 클라이언트를 통지
	char buf[BUF_SIZE];
	for (int i = 1; i < clients_num-1; i++) {											// 기존의 클라이언트 들에게는 새로운 클라이언트의 아이디를 알려주고
																						// 새 클라이언트 에게는 기존의 클라이언트의 아이디를 알려준다.
		sprintf(buf, "$CONNECT:%d\n", sock);
		printf("%s", buf);
		write(clients[i], buf, sizeof(buf));
		sprintf(buf, "$CONNECT:%d\n", clients[i]);
		printf("%s", buf);
		write(sock, buf, sizeof(buf));
	}
}
void* recv_pic(void* arg) {																// 특정 클라이언트의 데이터(얼굴 영상)를 지속적으로 수신
	int sock_pic_recv;
	char msg[BUF_SIZE];
	char data[BUF_SIZE];
	int str_len;
	sock_pic_recv = ((int*)arg)[0];														// 특정 클라이언트와 연결된 소켓
	int who = ((int*)arg)[1];															// 특정 클라이언트의 아이디 (기본 연결 소켓 번호)
	int temp;																			// 클라이언트의 아이디가 저장된 배열의 인덱스 번호

	int num=0;

	for (int i = 0; i < clients_num; i++) {												// 클라이언트 배열에서
		if (clients[i] == who) {														// 특정클라이언트의 아이디를 찾은 후 해당 인덱스를 기억
			temp = i;
			break;
		}
	}

	puts("recv thread create ok");
	while (1) {
		str_len = read(sock_pic_recv, msg, BUF_SIZE);
		msg[str_len] = 0;
		sprintf(data, "%d : %s", who, msg);
		if (str_len == 0) {
			close(sock_pic_recv);
			break;
		}
		/////////////////////////////////////////////////////////////////// 테스트용, 특정 클라이언트로 부터 메시지가 도착했을때 카운트를 체크함
		num++;
		nums[temp] = num;
		///////////////////////////////////////////////////////////////////

		printf("%s\n",data);
	}
}
void* send_pic(void* arg) {
	int sock_pic_send;
	char msg[BUF_SIZE];
	int str_len;
	sock_pic_send = ((int*)arg)[0];														// 특정 클라이언트와 연결된 소켓
	int who = ((int*)arg)[1];															// 특정 클라이언트의 아이디 (기본 연결 소켓 번호)
	int from = ((int*)arg)[2];															// 전송받을 대상 클라이언트의 아이디
	int temp,temp_from;																	// 클라이언트의 아이디가 저장된 배열의 인덱스 번호

	int num = 0;

	for (int i = 0; i < clients_num; i++) {												// 클라이언트 배열에서
		if (clients[i] == who) {														// 특정클라이언트의 아이디를 찾은 후 해당 인덱스를 기억
			temp = i;
		}
		else if(clients[i] == from) {
			temp_from = i;
		}
	}

	puts("send thread create ok");
	while (1) {
		sleep(5);
		sprintf(msg, "%d", nums[temp_from]);
		write(sock_pic_send, msg, BUF_SIZE);
		printf("%s\n", msg);
	}
}
