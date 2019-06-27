// 기본적인 c++ 클라이어트 코드 ( c코드 )
// 서버의 ip와 port의 값은 가변적이다
// 문자열 전송후 서버로 부터의 답변을 읽어서 출력하는 코드

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData; 
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)														// 윈도우에서만 존재하는 것으로 이게 있어야 socket이 가능한듯
		ErrorHandling("WSASTartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);															// 소켓 생성
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));																// 연결할 서버 설정
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("52.12.171.43");
	servAddr.sin_port = htons(atoi("9000"));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)						// 서버에 연결 요청
		ErrorHandling("connect() error!");


	while (1) {																							// 입력 후 읽기
		printf("input : ");
		scanf("%s", message);
		send(hSocket, message, strlen(message),0);
		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		printf("Message form server : %s \n", message);

		if (!strcmp(message, "1"))
			break;
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
