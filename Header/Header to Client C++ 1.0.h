// 클라이언트용 헤더파일

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
class CAM {
private:
	Mat frame;
	VideoCapture cap;
	int deviceID = 0;
	int apiID = cv::CAP_ANY;
	int height = 480;
	int width = 640;

public:
	// 기본 생성자. 캠을 불러오는 기능
	CAM() {
		cap.open(deviceID + apiID);
		if (!cap.isOpened()) {
			cerr << "ERROR! Unable to open camera\n";
		}
	}
	// 캠에서 프레임을 가져오는 함수
	Mat stream_capture()
	{
		cap.read(frame);
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
		}
		return frame;

	}
	// 프레임을 화면에 출력하는 함수
	void stream_show(Mat f) {
		imshow("Live", f);
	}
	// 프레임을 화면에 출력하는 함수2
	void stream_show2(Mat f) {
		imshow("Live2", f);
	}
	// 프레임을 문자열 형태로 변환하는 함수
	unsigned char* MtoC(Mat f) {
		unsigned char* buffer = f.data;
		return buffer;
	}
	// 문자열을 프레임 형태로 변환하는 함수
	Mat CtoM(unsigned char* c) {
		Mat frame = Mat(height, width, CV_8UC3);
		frame.data = c;
		return frame;
	}
};
