#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>

using namespace cv;

int main() {
	Mat img;                // 동영상 파일 재생을 위한 행렬 선언
	VideoCapture cap(2);    // 캠으로부터 영상을 받아온다 (혹은 1번)

	
	int count = 0;
	char savefile[200];        // 이미지 파일 이름을 200자 이내로 제한하기 위한 char 변수 선언

	while (1) {
		clock_t start = clock();
		cap >> img;
		imshow("image", img);      // 영상 출력    
		resize(img, img, Size(640, 480), 0, 0, INTER_CUBIC);    // 사진으로 저장할 때는 크기를 downsizing해서 저장한다
		sprintf(savefile, "image%d.jpg", count);
		clock_t end = clock();
		if ((end-start)%3==0) {
			imwrite(savefile, img);        // img를 파일로 저장한다.
			count++;
		}
		if (waitKey(100) == 27)  break; // esc를 누르면 종료한다
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