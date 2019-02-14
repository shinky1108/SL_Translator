#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

using namespace cv;

int main() {
	Mat img;                // ������ ���� ����� ���� ��� ����
	VideoCapture cap(2);    // ķ���κ��� ������ �޾ƿ´� (Ȥ�� 1��)

	
	int count = 0;
	char savefile[200];        // �̹��� ���� �̸��� 200�� �̳��� �����ϱ� ���� char ���� ����

	while (1) {
		clock_t start = clock();
		cap >> img;
		imshow("image", img);      // ���� ���    
		resize(img, img, Size(640, 480), 0, 0, INTER_CUBIC);    // �������� ������ ���� ũ�⸦ downsizing�ؼ� �����Ѵ�
		sprintf(savefile, "image%d.jpg", count);
		clock_t end = clock();
		if ((end-start)%3==0) {
			imwrite(savefile, img);        // img�� ���Ϸ� �����Ѵ�.
			count++;
		}
		if (waitKey(100) == 27)  break; // esc�� ������ �����Ѵ�
	}
	return 0;
}

/*#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	// Open another image
	Mat image;
	image = cv::imread("messi.jpg");

	Mat image2 = image.clone();

	// define bounding rectangle
	cv::Rect rectangle(10, 10, image.cols, image.rows);

	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel, fgModel; // the models (internally used)

							  // GrabCut segmentation
	cv::grabCut(image,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground
		bgModel, fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle
	cout << "oks pa dito" << endl;
	// Get the pixels marked as likely foreground
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	//cv::Mat background(image.size(),CV_8UC3,cv::Scalar(255,255,255));
	image.copyTo(foreground, result); // bg pixels not copied

									  // draw rectangle on original image
	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);

	imwrite("img_1.jpg", image);

	imwrite("Foreground.jpg", foreground);
	Mat background = image2 - foreground;
	imwrite("Background.jpg", background);

	return 0;
} */