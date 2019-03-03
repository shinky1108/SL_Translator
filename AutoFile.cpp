#include <iostream>
#include <io.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {

	Mat frame;
	Mat handImg;
	Mat tmpImg;
	string path = "C:\\test\\sstt\\*.*";
	char savefile[200];
	char image[100] = "C:\\test\\sstt\\";
	struct _finddata_t fd;
	intptr_t handle;
	int i = 0;

	if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
		cout << "No file in directory!" << endl;
	_findnext(handle, &fd);
	_findnext(handle, &fd);

	do
	{
		char image[100] = "C:\\test\\sstt\\";
		strcat(image, fd.name);
		tmpImg = imread(image, 1);
		cvtColor(tmpImg, frame, COLOR_BGR2YCrCb);
		inRange(frame, Scalar(0, 133, 50), Scalar(255, 173, 127), frame);
		handImg = (frame.size(), CV_8UC3, Scalar(0));
		add(tmpImg, Scalar(0), handImg, frame);
		cout << image << endl;
		sprintf(savefile, "C:\\test\\sstt\\test\\image%d.jpg", i);
		imwrite(savefile, handImg);
		i++;
	} while (_findnext(handle, &fd) == 0);
	_findclose(handle);
}
