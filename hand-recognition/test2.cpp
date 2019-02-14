/*#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
	cv::Mat image = cv::imread("messi.jpg");
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	// �Է� ���� ���� �κ� ����/��� ���̺��� �����ϴ� ���
	// ���� ��ü ���θ� �����ϴ� ���� ���簢���� ����
	cv::Rect rectangle(10, 10, image.cols, image.rows);
	// ��� ���簢�� ����
	// ���簢�� ���� ȭ�Ҵ� ������� ���̺�

	// �Է� ����� ��ü ���� ���� �ܿ� cv::grabCut �Լ��� ȣ���� ��
	// �� �˰��� ���� ���� ���� �����ϴ� �� ����� ���ǰ� �ʿ�
	cv::Mat result; // ���� (4�ڱ� ������ ��)
	cv::Mat bgModel, fgModel; // �� (�ʱ� ���)
	cv::grabCut(image,    // �Է� ����
		result,    // ���� ���
		rectangle,   // ������ �����ϴ� ���簢��
		bgModel, fgModel, // ��
		10,     // �ݺ� Ƚ��
		cv::GC_INIT_WITH_RECT); // ���簢�� ���
								// cv::CC_INT_WITH_RECT �÷��׸� �̿��� ��� ���簢�� ��带 ����ϵ��� ����

								// cv::GC_PR_FGD ���濡 ���� ���� �ִ� ȭ��(���簢�� ������ ȭ�� �ʱⰪ)
								// cv::GC_PR_FGD�� ������ ���� ���� ȭ�Ҹ� ������ ������ ���� ������ ����
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	// ������ ���ɼ��� �ִ� ȭ�Ҹ� ��ũ�� ���� ��������
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	// ��� ���� ����
	image.copyTo(foreground, result);
	// ��� ȭ�Ҵ� ������� ����

	cv::rectangle(image, rectangle, cv::Scalar(255, 255, 255), 1);

	cv::namedWindow("Result");
	cv::imshow("Result", result);

	cv::namedWindow("Foreground");
	cv::imshow("Foreground", foreground);

	cv::namedWindow("Rectangle");
	cv::imshow("Rectangle", image);

	cv::waitKey(0);

	return 0;
}*/