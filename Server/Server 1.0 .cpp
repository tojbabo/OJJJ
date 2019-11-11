#include<iostream>
using namespace std;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<pthread.h>

#define BUF_SIZE 65540
#define PACK_SIZE 4096
#define EPOLL_SIZE 5

#define PORT 9000

struct USER {
	int sock;
	int id;
	int port;
	int index;
	struct sockaddr_in adr;
};

void error_handling(const char*);
void serv_setting(struct sockaddr_in*, int);
void system_msg(char*,struct sockaddr_in);
void check_clients(int, int);
void* recv_face(void*);
void* send_face(void*);

struct USER users[EPOLL_SIZE];
char face[EPOLL_SIZE][BUF_SIZE];
int user_count = 0;
int s_sock;

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

	
	serv_setting(&s_adr, PORT);

	if (bind(s_sock, (struct sockaddr*)&s_adr, sizeof(s_adr)) == -1)
		error_handling("bind() error");

	if (listen(s_sock, 5) == -1)
		error_handling("listen() error");

	epfd = epoll_create(EPOLL_SIZE);														// 정해놓은 사이즈 만큼 epoll 생성
	ep_events = (epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);				// epoll에 맞는 배열 생성

	event.events = EPOLLIN;
	event.data.fd = s_sock;

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
			if (ep_events[i].data.fd == s_sock)												////// 이벤트가 발생한 소켓이 서버소켓일 경우
			{
				c_adr_sz = sizeof(c_adr);
				c_sock = accept(s_sock, (struct sockaddr*)&c_adr, &c_adr_sz);				// 연결이 발생했을때

				event.events = EPOLLIN;
				event.data.fd = c_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, c_sock, &event);

				printf("NEW CONNECT : %d\n", c_sock);

				memset(&users[user_count], 0, sizeof(struct USER));
				users[user_count].id = c_sock;
				users[user_count].port = PORT + (c_sock*10);
				users[user_count].index = user_count;

				sprintf(message, "ID:%d,PORT:%d", c_sock, users[user_count].port);
				printf("send message : %s\n", message);

				write(c_sock, message, strlen(message));

				if (pthread_create(&thread_recv, NULL, recv_face, (void*)&users[user_count]) != 0) {
					puts("pthread_create() error!");
					return -1;
				}

				check_clients(c_sock, users[user_count].port);
				user_count++;

			}
			else{																		////// 이벤트가 클라이언트 소켓에서 발생한 경우
				str_len = read(ep_events[i].data.fd, message, BUF_SIZE);
				if (str_len == 0) {														////// 연결해제가 발생했을 경우

																						///////
																						// 특정 클라이언트가 종료 되었을때
																						// 클라이언트들의 순서를 재배치하는 로직
																						///////
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d\n", ep_events[i].data.fd);
				}
				else{																	////// 메시지 도착한 경우
					message[str_len] = 0;
					printf("RECV MSG : %s <%d>\n", message,i);
					if (message[0] == '#') {
						int target_id;
						int target_port;
						sscanf(message, "#%d", &target_id);
						for (int j = 0; j < user_count; j++) {
							if (users[j].id == target_id) {
								target_port = ++users[j].port;
								break;
							}
						}
						
						sprintf(message, "#%d", target_port);
						write(ep_events[i].data.fd, message, strlen(message));
						printf("send data : %s <%d>\n", message,i);

						pthread_t t1;
						USER target_user;
						target_user.id = target_id;
						target_user.port = target_port;

						/////////////////////////////////////////////////////////// 영상 송신 쓰레드 생성부
						if (pthread_create(&t1, NULL, send_face, (void*)&target_user) != 0) {
							puts("pthread_create() error!");
							break;
						}
						////////////////////////////////////////////////////////////

					}
						
					//write(ep_events[i].data.fd,message,str_len);
					/*
					for (int j = 1; j < clients_num; j++) {
					write(clients[j], message, str_len);
					}*/
				}
			}
		}
	}
	close(s_sock);
	close(epfd);
	return 0;
}
void* recv_face(void* arg) {
	struct USER user;
	user = *((struct USER*)arg);
	
	sockaddr_in host;
	sockaddr_in client;
	serv_setting(&host, user.port);
	memset(&client, 0, sizeof(client));
	socklen_t adr_sz = sizeof(client);

	int sock;
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	if (bind(sock, (struct sockaddr*)&host, sizeof(host)) == -1)
		error_handling("bind error()");

	char buf[BUF_SIZE];
	int recv_len;
	int total_pack;
	char* longbuf;

	while (1) {
		do {
			recv_len = recvfrom(sock, buf, BUF_SIZE, 0,(struct sockaddr*)&client,&adr_sz);
		} while (recv_len > sizeof(int));
		total_pack = ((int*)buf)[0];

		//cout << "expecting length of packs:" << total_pack << endl;
		longbuf = new char[PACK_SIZE * total_pack];
		cout << total_pack << endl;

		for (int i = 0; i < total_pack; i++) {
			recv_len = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr*)&client, &adr_sz);
			if (recv_len != PACK_SIZE) {
				//cerr << "Received unexpected size pack:" << recv_len << endl;
				continue;
			}
			memcpy(&longbuf[i * PACK_SIZE], buf, PACK_SIZE);
		}
	}

}
/*void* recv_face(void* arg) {
	pthread_t thread_target;
	struct USER user;
	struct USER target_user;
	int sock;
	char buf[BUF_SIZE];
	int str_len;
	socklen_t adr_sz;
	struct sockaddr_in s_adr, c_adr;

	int target;

	user = *((struct USER*)arg);
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&target_user, 0, sizeof(struct USER));
	serv_setting(&s_adr, user.port);

	printf("recv from %d\n", user.id);

	if (bind(sock, (struct sockaddr*)&s_adr, sizeof(s_adr)) == -1)
		error_handling("bind() error");


	adr_sz = sizeof(c_adr);
	while (1) {
		
		str_len = recvfrom(sock, face[user.index], BUF_SIZE, 0, (struct sockaddr*)&c_adr, &adr_sz);

		printf("RECVING DATA : %s(%d), IP : %s, PORT : %d\n", face[user.index], str_len, inet_ntoa(c_adr.sin_addr), ntohs(c_adr.sin_port));
	

		if (str_len != 1024) {
			face[user.index][str_len] = 0;
			//sendto(sock, "hi", strlen("hi"), 0, (struct sockaddr*)&c_adr, sizeof(c_adr));   --> 여긴 됨
			target_user.sock = sock;
			target_user.adr = c_adr;
			target_user.index = user.index;
			sscanf(face[user.index], "%d", &target_user.id);
			sprintf(buf, "%d", ntohs(c_adr.sin_port));
			sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&c_adr, adr_sz);
			
			/////////////////////////////////////////////////////////// 영상 송신 쓰레드 생성부
			if (pthread_create(&thread_target, NULL, send_face, (void*)&target_user) != 0) {
				puts("pthread_create() error!");
				break;
			}
			////////////////////////////////////////////////////////////

		}
		else {
			str_len = sendto(sock, face[user.index], BUF_SIZE, 0, (struct sockaddr*)&c_adr, adr_sz);
			printf("send size : %d\n", str_len);
		}

		//sendto(sock, buf, sizeof(buf), 0,(struct sockaddr*)&c_adr,adr_sz);
	}

}*/

void* send_face(void* arg) {
	char msg[BUFSIZ];

	struct USER target;
	target = *((struct USER*)arg);

	int t_port = target.port;
	int t_id = target.id;
	int index;
	
	for (int i = 0; i < user_count; i++) {
		if (t_id == users[i].id) {
			index = i;
			break;
		}
	}

	int sock;
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	cout << "send setting ok!" << endl;
	sockaddr_in adr;
	serv_setting(&adr, t_port);
	if (bind(sock, (struct sockaddr*)&adr, sizeof(adr)) == -1)
		error_handling("bind error()");

	cout << "send binding ok !" << endl;
	sockaddr_in t_adr;
	memset(&t_adr, 0, sizeof(t_adr));
	socklen_t adr_sz = sizeof(t_adr);
	int len;

	
	while (1) {
		cout << "wait for receiving!" << endl;
		len = recvfrom(sock, msg, BUFSIZ, 0, (struct sockaddr*)&t_adr, &adr_sz);
		msg[len] = 0;
		printf("receive data : %s <%s, %d>\n", msg,inet_ntoa(t_adr.sin_addr),ntohs(t_adr.sin_port));
		if (!strcmp(msg, "ok"))
			break;
	}

	printf("good\n");


}


void check_clients(int sock, int new_port) {												// 새로운 클라이언트 및 기존의 클라이언트를 통지
	char buf[BUF_SIZE];
	for (int i = 0; i < user_count; i++) {												// 기존의 클라이언트 들에게는 새로운 클라이언트의 아이디를 알려주고
																						// 새 클라이언트 에게는 기존의 클라이언트의 아이디를 알려준다.
		sprintf(buf, "$NEW:%d,PORT:%d\n", sock, new_port);
		printf("%s", buf);
		write(users[i].id, buf, strlen(buf));
		sprintf(buf, "$NEW:%d,PORT:%d\n", users[i].id, users[i].port);
		printf("%s", buf);
		write(sock, buf, strlen(buf));
	}
}
void serv_setting(struct sockaddr_in* adr, int port) {
	memset(adr, 0, sizeof(*adr));
	adr->sin_family = AF_INET;
	adr->sin_addr.s_addr = htonl(INADDR_ANY);
	adr->sin_port = htons(port);
}
void error_handling(const char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
void system_msg(char* msg,struct sockaddr_in adr) {
	char buf[BUF_SIZE];
	printf("SYSTEM MEG : %s\n", msg);
	printf("recv meg to : %s<%s:%d>\n", msg, inet_ntoa(adr.sin_addr), ntohs(adr.sin_port));
	sprintf(buf, "%s, hi", msg);

	int sock;
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&adr, sizeof(adr));
	printf("send meg to : %s<%s:%d>\n",buf,inet_ntoa(adr.sin_addr),ntohs(adr.sin_port));
} 
