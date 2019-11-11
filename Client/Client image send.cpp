#include <WinSock2.h> // 윈속 헤더 포함 
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()

using namespace std;

#include "opencv2/opencv.hpp"
using namespace cv;

// 이미지 사이즈 변환을 위해 사용
#define FRAME_HEIGHT 720
#define FRAME_WIDTH 1280

#define FRAME_INTERVAL (1000/30)

// udp 통신에 사용될 패킷 크기
#define PACK_SIZE 4096 //udp pack size; note that OSX limits < 8100 bytes
#define ENCODE_QUALITY 80

int main() {
	WSADATA wsadata;
	SOCKADDR_IN serv;
	SOCKET sock;
	if (WSAStartup(0x202, &wsadata) == SOCKET_ERROR)
		exit(0);

	memset(&serv, 0, sizeof(serv));

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(atoi("9000"));

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	connect(sock, (struct sockaddr*)&serv, sizeof(serv));



	// mat 데이터를 jpeg로 변환시 설정될 값
	int jpegqual = ENCODE_QUALITY; // Compression Parameter

	Mat frame, temp;

	vector < uchar > encoded;
	VideoCapture cap(0); // Grab the camera


	namedWindow("send", WINDOW_AUTOSIZE);
	if (!cap.isOpened()) {
		cerr << "OpenCV Failed to open camera";
		exit(1);
	}

	clock_t last_cycle = clock();
	while (1) {
		cap >> frame;
		if (frame.size().width == 0)continue;//simple integrity check; skip erroneous data...
		// 이미지 사이즈 변환
		resize(frame, temp, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);

		// jpg로 변환된 데이터가 저장될 변수
		vector < int > compression_params;
		// 변수 설정
		compression_params.push_back(IMWRITE_JPEG_QUALITY);
		compression_params.push_back(jpegqual);
		// 데이터 변환
		imencode(".jpg", temp, encoded, compression_params);
		imshow("send", temp);
		// 전송될 패킷의 숫자 설정
		int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

		int ibuf[1];
		ibuf[0] = total_pack;

		// 전송에 앞서 패킷 수 통지
		send(sock, (char*)ibuf, sizeof(int), 0);

		// 통지한 패킷 수 만큼 데이터 전송
		for (int i = 0; i < total_pack; i++)
			send(sock, (char*)&encoded[i*PACK_SIZE], PACK_SIZE, 0);
		waitKey(FRAME_INTERVAL);
	}


	return 0;
}
