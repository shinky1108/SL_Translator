#include <iostream>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <windows.h>
//캡쳐를 위해 사용하는 헤더파일


using namespace std;
using namespace cv;

int main() {
	Mat frame, tmpImg;
	Mat handImg;


	int index = 0;
	char buf[256];
	Mat frameMat;
	//캡쳐 Mat, 파일 이름 버퍼

	VideoCapture video(0);

	//namedWindow("change_image", CV_WINDOW_AUTOSIZE);
	//namedWindow("original_image", CV_WINDOW_AUTOSIZE);

	//namedWindow("capture_image", WINDOW_AUTOSIZE);
	//namedWindow에 등록

	while (true) {
		video >> tmpImg;

		cvtColor(tmpImg, frame, CV_BGR2YCrCb);
		inRange(frame, Scalar(0, 133, 50), Scalar(255, 173, 127), frame);
		handImg = (frame.size(), CV_8UC3, Scalar(0));

		frameMat = (frame.size(), CV_8UC3, Scalar(0));
		//살색, 침식시킨 프레임을 frameMat에 담는다.


		add(tmpImg, Scalar(0), handImg, frame);
		add(tmpImg, Scalar(0), frameMat, frame);


		//**콘솔창 띄우기
		imshow("Original_image", tmpImg);
		imshow("Gray_image", frame); //gray스케일
		imshow("SkinDetection_image", handImg); //skin detection 콘솔창
		imshow("Capture_image", frameMat); //captrue콘솔창

		sprintf(buf, "c:/temp/img_%06d.jpg", index);
		cout << buf << endl;
		imwrite(buf, frameMat);
		//캡쳐 실행

		index++;
		if (index == 999999) index = 0;
		_sleep(50);

		if (waitKey(27) == 27) {
			break;
			// esc는 frame창에서 누르기.
		}

	}
	video.release();
	tmpImg.release();
	frame.release();
	handImg.release();
	frameMat.release();
	//캡쳐 프레임 Mat구조체 소멸
	destroyAllWindows();

	return 0;
}