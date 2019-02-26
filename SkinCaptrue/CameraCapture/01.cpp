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
		printf("ù��° ī�޶� ���� �����ϴ�. \n");
	}


	namedWindow("��ķ ����", WINDOW_AUTOSIZE);


	while (1)
	{

		//��ĸ���κ��� �� �������� �о��  
		videoCapture.read(frameMat);

		//ȭ�鿡 ������
		imshow("��ķ ����", frameMat);

		//�̹����� ����  
		sprintf(buf, "c:/temp/img_%06d.jpg", index);
		cout << buf << endl;
		imwrite(buf, frameMat);

		index++;
		if (index == 999999) index = 0;
		_sleep(2000);


		if (waitKey(25) == 27) break; //ESCŰ ������ ����  
	}


	return 0;
}