/*#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>

//#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
//#include <opencv2/opencv.hpp>   // Include OpenCV API
//#include "../cv-helpers.hpp"    // Helper functions for conversions between RealSense and 

using namespace std;
using namespace cv;

int main()
{
	int c = 0;
	//CvCapture : 비디오 캡쳐 함수를 사용하기 위한 데이터 구조 
	//cvCaptureFromCAM : 카메라에서 프레임 캡쳐 
	// https://dsnight.tistory.com/2
	CvCapture* capture = cvCaptureFromCAM(2);
	if (!cvQueryFrame(capture))
	{
		cout << "Video camera capture status: OK" << endl;
	}
	else
	{
		cout << "Video capture failed, please check the camera." << endl;
	}

	//CvSize : 두 개의 정수형 멤버 변수 width, height를 가지고 있음 (영상의 크기) 
	CvSize sz = cvGetSize(cvQueryFrame(capture));
	cout << "Height & width of captured frame: " << sz.height << " x " << sz.width;
	//IplImage와 Mat은 비슷 
	//cvCreatImage는 IplImage 구조체의 메모리를 생성하여 그포인터를 넘겨줌.
	// IplImage *img = cvCreagteImage(size, depth, chennal);
	IplImage* src = cvCreateImage(sz, 8, 3);
	IplImage* gray = cvCreateImage(cvSize(270, 270), 8, 1);

	while (c != 27)
	{
		//cvQueryFrame : CvCapture 구조체 안에 할당된 메모리를 불러옴. 
		src = cvQueryFrame(capture);
		//cvSetImageROI : 내가 처리하려는 부분만 설정(관심영역)
		cvSetImageROI(src, cvRect(340, 100, 270, 270));
		//cvCvtColor : 색상 영역을 회색으로 변조할 때 필요한것. 
		cvCvtColor(src, gray, CV_BGR2GRAY);
		//cvSmooth : 영상의 노이즈나 손상을 줄이기 위해 사용 
		// 입력영상, 출력영상, 스무딩 방법, ? ? ? 
		cvSmooth(gray, gray, CV_BLUR, (12, 12), 0);
		//cvNamedWindow : 영상을 출력하기 위한 윈도우를 만들기 위해 사용되는 함수  (윈도우 이름, 윈도우의 크기조절)
		cvNamedWindow("Blur", 1); 
		//cvShowImage : cvNamedWindow() 함수에 의해 지정된 윈도우에 영상을 출력하기 위해 사용하는 함수 
		// cvNamedWindow에서의 name과 이름이 같아야 제대로 영상이 출력됨  (윈도우의 이름, 출력할 영상)
		cvShowImage("Blur", gray);   // blur-not-clear
		// cvThreshold : 영상의 이진화를 처리. 
		// 원본 이미지, 결과 출력 이미지, 임계값, 임계값보다 큰 픽셀 값을 몇으로 설정할 것인지 정함, 이진화 방식을 결정 
		cvThreshold(gray, gray, 0, 255, (CV_THRESH_BINARY_INV + CV_THRESH_OTSU));
		cvNamedWindow("Threshold", 1); cvShowImage("Threshold", gray);  // black-white
		//CvMemStorage : 메모리가 저장되는 곳 
		CvMemStorage* storage = cvCreateMemStorage();
		//CvSeq도 마찬가지로 저장소 
		CvSeq* first_contour = NULL;
		CvSeq* maxitem = NULL;
		//cvFindContours : 이미지에서 contour를 찾은 후 contour(윤곽)의 갯수를 리턴
		// 이미지, CvMemStorage 구조체의 포인터(실제로 contour정보가 저장되는 공간), 첫번째 최외곽 contour의 정보를 저장할 CvSeq 구조체의 포인터,
		// 시퀀스 헤더의 크, contour 검출 방법 (모든 contour를 검출한 후 2단계로 계층화), 생략산 방법(가로, 세로, 대각선으로 쪼개서 압축하며 함수는 끝점만 출력)
		//, 모든 contour 포인트 이동 
		int cn = cvFindContours(gray, storage, &first_contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
		double area, max_area = 0.0;
		CvSeq* ptr = 0;
		//int maxn=0,n=0;
		if (cn > 0)
		{
			for (ptr = first_contour; ptr != NULL; ptr = ptr->h_next)
			{ //fabs 절대값 변환 함수 //cvContourArea : 관심있는 부분 or 전체 경계선이 포함하는 영역의 넓이를 계산
				area = fabs(cvContourArea(ptr, CV_WHOLE_SEQ, 0));
				if (area > max_area)
				{
					max_area = area;
					maxitem = ptr;
					//maxn=n;
				}
				// n++;
			}
			if (max_area > 1000)
			{
				//CvPoint는 x,y값을 갖음 
				CvPoint pt0;
				//cvCreateMemStorage : 객체를 동적으로 생성하고자 할 때 사용 
				//메모리 블록들의 연결 리스트이며, 연속된 메모리 블록들의 집합을 신속하게 할당 or 해제 할 수 있음 
				// 0 으로 설정하면 64KB의 기본 블록 크기가 사용 
				CvMemStorage* storage1 = cvCreateMemStorage();
				CvMemStorage* storage2 = cvCreateMemStorage(0);
				// 새로운 메모리를 만들고
				//cvCreateSeq (int seq_flags, int header_size, int elem_size, CvMemStorage* storage)
				CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage1);
				CvSeq* hull; // 여러개의 점들 중에 최외곽의 점들을 이어놓은 집합? 공간? 을 의미 
				// hull 내부에 모든 점이 존재하고 외부에는 점이 존재하지 않아야해 
				CvSeq* defects;
				for (int i = 0; i < maxitem->total; i++)
				{
					CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, maxitem, i);
					pt0.x = p->x;
					pt0.y = p->y;
					cvSeqPush(ptseq, &pt0);
				}
				// 여러 점들의 집합을 포함하는 볼록한 모양의 입체을 찾는다. 
				hull = cvConvexHull2(ptseq, 0, CV_CLOCKWISE, 0);
				int hullcount = hull->total;
				//cvConvexityDefects : Convexhull 상에서 실제 개체가 차지하는 영역 사이의 빈공간
				// 즉 손가락 사이를 나타내는데 사용함 
				defects = cvConvexityDefects(ptseq, hull, storage2);
				// pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
				// printf("** : %d :**",hullcount);
				CvConvexityDefect* defectArray;
				// int j=0;
				for (int i = 1; i <= hullcount; i++)
				{
					CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, hull, i);
					cvLine(src, pt0, pt, CV_RGB(255, 0, 0), 1, CV_AA, 0);
					pt0 = pt;
				}
				for (; defects; defects = defects->h_next)
				{
					int nomdef = defects->total; // defect amount
												 // outlet_float( m_nomdef, nomdef );
												 // printf(" defect no %d \n",nomdef);
					if (nomdef == 0)
						continue;
					// Alloc memory for defect set.
					// fprintf(stderr,"malloc\n");
					
					//시퀀스에 있는 내용들을 배열로 옮김. 
					defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
					// Get defect set.
					// fprintf(stderr,"cvCvtSeqToArray\n");
					//시퀀스에 있는 내용들을 배열로 옮김.
					cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
					// Draw marks for all defects.

					int con = 0;
					for (int i = 0; i < nomdef; i++)
					{
						if (defectArray[i].depth > 20)
						{
							con = con + 1;
							// printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
							// cvLine : 이미지에 선을 그음
							// cvLine(array, pt1, pt2, color, thickness, connectivity)
							// 선을 그릴 원본 이미지, 선의 양 끝 점중 한쪽 점 , 나머지 한쪽 점, 선의 색깔, 선의 두께, 선 종류 
							cvLine(src, *(defectArray[i].start), *(defectArray[i].depth_point), CV_RGB(255, 255, 0), 1, CV_AA, 0);

							// 이미지에 원을 그리는 함수 
							//cvCircle(array, center, 원의 반지름 길이, 원의 색깔, 원 경계선의 두께, 원 경계선의 종류)
							cvCircle(src, *(defectArray[i].depth_point), 5, CV_RGB(0, 0, 255), 2, 8, 0);
							cvCircle(src, *(defectArray[i].start), 5, CV_RGB(0, 255, 0), 2, 8, 0);
							cvLine(src, *(defectArray[i].depth_point), *(defectArray[i].end), CV_RGB(0, 255, 255), 1, CV_AA, 0);
							// 외곽선을 그림 
							//cvDrawContours(img, contours, external_color, hole_color, max_level, thickness, line_type, offset)
							// (외곽선이 그려질 입력 영상, 외곽선 트리의 루트 노드, 외부 외곽선의 색상, 내부 외곽선의 색상, 이곽선을 그릴때 이동할 최대 깊이,
							//외고가선의 굵기와 타입, 외곽선이 그려질 위치를 지정하는 용도로 사용 )
							cvDrawContours(src, defects, CV_RGB(0, 0, 0), CV_RGB(255, 0, 0), -1, CV_FILLED, 8);
						}
					}
					// cout<<con<<"\n";
					char txt[40] = "";
					if (con == 1)
					{
						char txt1[] = "Hi , This is Udit";
						strcat(txt, txt1);
					}
					else if (con == 2)
					{
						char txt1[] = "3 Musketeers";
						strcat(txt, txt1);
					}
					else if (con == 3)
					{
						char txt1[] = "Fanatastic 4";
						strcat(txt, txt1);
					}
					else if (con == 4)
					{
						char txt1[] = "It's 5";
						strcat(txt, txt1);
					}
					else
					{
						char txt1[] = "Jarvis is busy :P"; // Jarvis can't recognise you
						strcat(txt, txt1);
					}
					cvNamedWindow("contour", 1); cvShowImage("contour", src);
					cvResetImageROI(src);
					CvFont font;
					cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 5, CV_AA);
					cvPutText(src, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));
					// j++;  
					// Free memory.
					free(defectArray);
				}
				cvReleaseMemStorage(&storage1);
				cvReleaseMemStorage(&storage2);
			}
		}
		cvReleaseMemStorage(&storage);
		cvNamedWindow("threshold", 1); cvShowImage("threshold", src);
		c = cvWaitKey(100);
	}
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
}*/