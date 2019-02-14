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
	//CvCapture : ���� ĸ�� �Լ��� ����ϱ� ���� ������ ���� 
	//cvCaptureFromCAM : ī�޶󿡼� ������ ĸ�� 
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

	//CvSize : �� ���� ������ ��� ���� width, height�� ������ ���� (������ ũ��) 
	CvSize sz = cvGetSize(cvQueryFrame(capture));
	cout << "Height & width of captured frame: " << sz.height << " x " << sz.width;
	//IplImage�� Mat�� ��� 
	//cvCreatImage�� IplImage ����ü�� �޸𸮸� �����Ͽ� �������͸� �Ѱ���.
	// IplImage *img = cvCreagteImage(size, depth, chennal);
	IplImage* src = cvCreateImage(sz, 8, 3);
	IplImage* gray = cvCreateImage(cvSize(270, 270), 8, 1);

	while (c != 27)
	{
		//cvQueryFrame : CvCapture ����ü �ȿ� �Ҵ�� �޸𸮸� �ҷ���. 
		src = cvQueryFrame(capture);
		//cvSetImageROI : ���� ó���Ϸ��� �κи� ����(���ɿ���)
		cvSetImageROI(src, cvRect(340, 100, 270, 270));
		//cvCvtColor : ���� ������ ȸ������ ������ �� �ʿ��Ѱ�. 
		cvCvtColor(src, gray, CV_BGR2GRAY);
		//cvSmooth : ������ ����� �ջ��� ���̱� ���� ��� 
		// �Է¿���, ��¿���, ������ ���, ? ? ? 
		cvSmooth(gray, gray, CV_BLUR, (12, 12), 0);
		//cvNamedWindow : ������ ����ϱ� ���� �����츦 ����� ���� ���Ǵ� �Լ�  (������ �̸�, �������� ũ������)
		cvNamedWindow("Blur", 1); 
		//cvShowImage : cvNamedWindow() �Լ��� ���� ������ �����쿡 ������ ����ϱ� ���� ����ϴ� �Լ� 
		// cvNamedWindow������ name�� �̸��� ���ƾ� ����� ������ ��µ�  (�������� �̸�, ����� ����)
		cvShowImage("Blur", gray);   // blur-not-clear
		// cvThreshold : ������ ����ȭ�� ó��. 
		// ���� �̹���, ��� ��� �̹���, �Ӱ谪, �Ӱ谪���� ū �ȼ� ���� ������ ������ ������ ����, ����ȭ ����� ���� 
		cvThreshold(gray, gray, 0, 255, (CV_THRESH_BINARY_INV + CV_THRESH_OTSU));
		cvNamedWindow("Threshold", 1); cvShowImage("Threshold", gray);  // black-white
		//CvMemStorage : �޸𸮰� ����Ǵ� �� 
		CvMemStorage* storage = cvCreateMemStorage();
		//CvSeq�� ���������� ����� 
		CvSeq* first_contour = NULL;
		CvSeq* maxitem = NULL;
		//cvFindContours : �̹������� contour�� ã�� �� contour(����)�� ������ ����
		// �̹���, CvMemStorage ����ü�� ������(������ contour������ ����Ǵ� ����), ù��° �ֿܰ� contour�� ������ ������ CvSeq ����ü�� ������,
		// ������ ����� ũ, contour ���� ��� (��� contour�� ������ �� 2�ܰ�� ����ȭ), ������ ���(����, ����, �밢������ �ɰ��� �����ϸ� �Լ��� ������ ���)
		//, ��� contour ����Ʈ �̵� 
		int cn = cvFindContours(gray, storage, &first_contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
		double area, max_area = 0.0;
		CvSeq* ptr = 0;
		//int maxn=0,n=0;
		if (cn > 0)
		{
			for (ptr = first_contour; ptr != NULL; ptr = ptr->h_next)
			{ //fabs ���밪 ��ȯ �Լ� //cvContourArea : �����ִ� �κ� or ��ü ��輱�� �����ϴ� ������ ���̸� ���
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
				//CvPoint�� x,y���� ���� 
				CvPoint pt0;
				//cvCreateMemStorage : ��ü�� �������� �����ϰ��� �� �� ��� 
				//�޸� ��ϵ��� ���� ����Ʈ�̸�, ���ӵ� �޸� ��ϵ��� ������ �ż��ϰ� �Ҵ� or ���� �� �� ���� 
				// 0 ���� �����ϸ� 64KB�� �⺻ ��� ũ�Ⱑ ��� 
				CvMemStorage* storage1 = cvCreateMemStorage();
				CvMemStorage* storage2 = cvCreateMemStorage(0);
				// ���ο� �޸𸮸� �����
				//cvCreateSeq (int seq_flags, int header_size, int elem_size, CvMemStorage* storage)
				CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage1);
				CvSeq* hull; // �������� ���� �߿� �ֿܰ��� ������ �̾���� ����? ����? �� �ǹ� 
				// hull ���ο� ��� ���� �����ϰ� �ܺο��� ���� �������� �ʾƾ��� 
				CvSeq* defects;
				for (int i = 0; i < maxitem->total; i++)
				{
					CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, maxitem, i);
					pt0.x = p->x;
					pt0.y = p->y;
					cvSeqPush(ptseq, &pt0);
				}
				// ���� ������ ������ �����ϴ� ������ ����� ��ü�� ã�´�. 
				hull = cvConvexHull2(ptseq, 0, CV_CLOCKWISE, 0);
				int hullcount = hull->total;
				//cvConvexityDefects : Convexhull �󿡼� ���� ��ü�� �����ϴ� ���� ������ �����
				// �� �հ��� ���̸� ��Ÿ���µ� ����� 
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
					
					//�������� �ִ� ������� �迭�� �ű�. 
					defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
					// Get defect set.
					// fprintf(stderr,"cvCvtSeqToArray\n");
					//�������� �ִ� ������� �迭�� �ű�.
					cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
					// Draw marks for all defects.

					int con = 0;
					for (int i = 0; i < nomdef; i++)
					{
						if (defectArray[i].depth > 20)
						{
							con = con + 1;
							// printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
							// cvLine : �̹����� ���� ����
							// cvLine(array, pt1, pt2, color, thickness, connectivity)
							// ���� �׸� ���� �̹���, ���� �� �� ���� ���� �� , ������ ���� ��, ���� ����, ���� �β�, �� ���� 
							cvLine(src, *(defectArray[i].start), *(defectArray[i].depth_point), CV_RGB(255, 255, 0), 1, CV_AA, 0);

							// �̹����� ���� �׸��� �Լ� 
							//cvCircle(array, center, ���� ������ ����, ���� ����, �� ��輱�� �β�, �� ��輱�� ����)
							cvCircle(src, *(defectArray[i].depth_point), 5, CV_RGB(0, 0, 255), 2, 8, 0);
							cvCircle(src, *(defectArray[i].start), 5, CV_RGB(0, 255, 0), 2, 8, 0);
							cvLine(src, *(defectArray[i].depth_point), *(defectArray[i].end), CV_RGB(0, 255, 255), 1, CV_AA, 0);
							// �ܰ����� �׸� 
							//cvDrawContours(img, contours, external_color, hole_color, max_level, thickness, line_type, offset)
							// (�ܰ����� �׷��� �Է� ����, �ܰ��� Ʈ���� ��Ʈ ���, �ܺ� �ܰ����� ����, ���� �ܰ����� ����, �̰����� �׸��� �̵��� �ִ� ����,
							//�ܰ����� ����� Ÿ��, �ܰ����� �׷��� ��ġ�� �����ϴ� �뵵�� ��� )
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