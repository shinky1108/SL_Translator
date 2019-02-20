#include <iostream>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <windows.h>
//ĸ�ĸ� ���� ����ϴ� �������


using namespace std;
using namespace cv;

int main() {
	Mat frame, tmpImg;
	Mat handImg;


	int index = 0;
	char buf[256];
	Mat frameMat;
	//ĸ�� Mat, ���� �̸� ����

	VideoCapture video(0);

	//namedWindow("change_image", CV_WINDOW_AUTOSIZE);
	//namedWindow("original_image", CV_WINDOW_AUTOSIZE);

	//namedWindow("capture_image", WINDOW_AUTOSIZE);
	//namedWindow�� ���

	while (true) {
		video >> tmpImg;

		cvtColor(tmpImg, frame, CV_BGR2YCrCb);
		inRange(frame, Scalar(0, 133, 50), Scalar(255, 173, 127), frame);
		handImg = (frame.size(), CV_8UC3, Scalar(0));

		frameMat = (frame.size(), CV_8UC3, Scalar(0));
		//���, ħ�Ľ�Ų �������� frameMat�� ��´�.


		add(tmpImg, Scalar(0), handImg, frame);
		add(tmpImg, Scalar(0), frameMat, frame);


		//**�ܼ�â ����
		imshow("Original_image", tmpImg);
		imshow("Gray_image", frame); //gray������
		imshow("SkinDetection_image", handImg); //skin detection �ܼ�â
		imshow("Capture_image", frameMat); //captrue�ܼ�â

		sprintf(buf, "c:/temp/img_%06d.jpg", index);
		cout << buf << endl;
		imwrite(buf, frameMat);
		//ĸ�� ����

		index++;
		if (index == 999999) index = 0;
		_sleep(50);

		if (waitKey(27) == 27) {
			break;
			// esc�� frameâ���� ������.
		}

	}
	video.release();
	tmpImg.release();
	frame.release();
	handImg.release();
	frameMat.release();
	//ĸ�� ������ Mat����ü �Ҹ�
	destroyAllWindows();

	return 0;
}