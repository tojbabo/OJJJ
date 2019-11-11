#include<WinSock2.h>
#define FRAME_HEIGHT 720
#define FRAME_WIDTH 1280
#define FRAME_INTERVAL (1000/30)
#define PACK_SIZE 4096 //udp pack size; note that OSX limits < 8100 bytes
#define ENCODE_QUALITY 80
#include "opencv2/opencv.hpp"

#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()

#define BUF_LEN 65540 // Larger than maximum UDP packet size

using namespace cv;
using namespace std;

int main() {
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serv_adr;
	SOCKADDR_IN cnt_adr;
	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
		exit(0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	memset(&cnt_adr, 0, sizeof(cnt_adr));

	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_adr.sin_port = htons(9000);
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (bind(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
		exit(1);

	char buffer[BUF_LEN]; // Buffer for echo string
	int recvMsgSize; // Size of received message

	clock_t last_cycle = clock();

	while (1) {
		// Block until receive message from a client
		do {
			// 클라이언트로 부터 총 전송받을 패킷수를 통지받음
			recvMsgSize = recv(sock, buffer, BUF_LEN, 0);
		} while (recvMsgSize > sizeof(int));
		// 패킷 수 저장
		int total_pack = ((int *)buffer)[0];

		cout << "expecting length of packs:" << total_pack << endl;
		// 패킷의 크기와 수만큼 변수 생성
		char * longbuf = new char[PACK_SIZE * total_pack];

		// 패킷의 수 만큼 데이터 리시브
		for (int i = 0; i < total_pack; i++) {
			recvMsgSize = recv(sock, buffer, BUF_LEN, 0);
			// 패킷 사이즈보다 작은 데이터를 수신시 무시
			if (recvMsgSize != PACK_SIZE) {
				cerr << "Received unexpected size pack:" << recvMsgSize << endl;
				continue;
			}
			// 연속해서 리시브한 데이터를 하나의 변수로 통합
			memcpy(&longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
		}

		// 전체적으로 리시브한 데이터를 이미지 데이터로 변환
		Mat rawData = Mat(1, PACK_SIZE * total_pack, CV_8UC1, longbuf);
		Mat frame = imdecode(rawData, IMREAD_COLOR);
		if (frame.size().width == 0) {
			cerr << "decode failure!" << endl;
			continue;
		}
		// 출력
		imshow("recv", frame);
		free(longbuf);

		waitKey(1);
		clock_t next_cycle = clock();
		double duration = (next_cycle - last_cycle) / (double)CLOCKS_PER_SEC;
		cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

		cout << next_cycle - last_cycle;
		last_cycle = next_cycle;
	}

	return 0;
}
