#include <opencv2/opencv.hpp>
#include <iostream>  
#include <stdio.h>
#include <windows.h>


using namespace cv;
using namespace std;



int main(int, char**)
{
	int index = 0;
	char buf[256];
	Mat frameMat;


	VideoCapture videoCapture(0);
	if (!videoCapture.isOpened())
	{
		printf("첫번째 카메라를 열수 없습니다. \n");
	}


	namedWindow("웹캠 영상", WINDOW_AUTOSIZE);


	while (1)
	{

		//웹캡으로부터 한 프레임을 읽어옴  
		videoCapture.read(frameMat);

		//화면에 보여줌
		imshow("웹캠 영상", frameMat);

		//이미지로 저장  
		sprintf(buf, "c:/temp/img_%06d.jpg", index);
		cout << buf << endl;
		imwrite(buf, frameMat);

		index++;
		if (index == 999999) index = 0;
		_sleep(2000);


		if (waitKey(25) == 27) break; //ESC키 누르면 종료  
	}


	return 0;
}