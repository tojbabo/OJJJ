// 캠을 통해 찍은 영상을 서버로 전송후 다시 받아서 출력하는 클라이언트
// UDP 기반이며 서버의 IP주소는 수정해 주어야한다.
// 캠을 찍기전부터 서버를 통해 수신한 후 출력할 때 까지의 시간을 체크한다.

#include <iostream>
#include <stdio.h>
#include <winsock2.h> // 윈속 헤더 포함 
#include <windows.h> 
#include<time.h>

#include"CV2.h"

#pragma comment (lib,"ws2_32.lib") // 윈속 라이브러리 링크
#define BUF 1024 // 버퍼 사이즈

using namespace std;

void main(void)
{
	///////////////////////
	CAM cam;
	Mat frame;
	uchar* char_temp;
	Mat mat_temp;
	
	/////////////////////// CV2 변수
	///////////////////////
	WSADATA wsaData; // 윈속 데이터 구조체
	SOCKET ClientSocket; // 소켓 선언
	SOCKADDR_IN ToServer; // 서버로 보내는 주소정보 구조체
	SOCKADDR_IN FromServer; // 서버에서 받는 주소정보 구조체

	int Send_Size;
	int FromServer_Size;
	int Recv_Size;
	ULONG   ServerPort = 9000; // 서버 포트번호

	char Buffer[BUF] = {"hi there\n"};
	char* buf;
	/////////////////////// UDP 변수
	clock_t begin, end;

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
	{
		cout << "WinSock 초기화부분에서 문제 발생 " << endl;
		WSACleanup();
		exit(0);
	}

	memset(&ToServer, 0, sizeof(ToServer));
	memset(&FromServer, 0, sizeof(FromServer));

	ToServer.sin_family = AF_INET;
	ToServer.sin_addr.s_addr = inet_addr("34.209.17.103");
	ToServer.sin_port = htons(atoi("9000")); 

	ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "소켓을 생성할수 없습니다." << endl;
		closesocket(ClientSocket);
		WSACleanup();
		exit(0);
	}

	/////////////////////// 데이터 송수신
	while(1)
	{
		begin = clock();
		frame = cam.stream_capture();
		cam.stream_show(frame);
		char_temp = cam.MtoC(frame);
		buf = (char*)char_temp;
		//- 패킷송신
		Send_Size = sendto(ClientSocket, buf, BUF, 0,(struct sockaddr*) &ToServer, sizeof(ToServer));
		
		// 패킷송신시 에러처리
		/*
		if (Send_Size != BUF)
		{
			cout << "sendto() error!" << endl;
			exit(0);
		}
		*/
		FromServer_Size = sizeof(FromServer);
		memset(Buffer, 0, BUF);

		// 패킷수신
		Recv_Size = recvfrom(ClientSocket, buf, BUF, 0,(struct sockaddr*) &FromServer, &FromServer_Size);
		mat_temp = cam.CtoM((uchar*)buf);
		cam.stream_show2(mat_temp);
		end = clock();
		cout << "시간 차이 : " << (end - begin) << endl;

		// 패킷수신시 에러처리
		/*
		if (Recv_Size < 0)
		{
			cout << "recvfrom() error!" << endl;
			exit(0);
		}
		*/
		// 수신한 패킷에서 IP / Data 출력
		//cout << "Recv from " << inet_ntoa(FromServer.sin_addr) << endl;
		//cout << "Data " << buf << endl;

		if (waitKey(5) >= 0)
			break;
	}
	/////////////////////// 데이터 송수신
	puts("done!");
	closesocket(ClientSocket); //소켓 닫기
	WSACleanup();
}
