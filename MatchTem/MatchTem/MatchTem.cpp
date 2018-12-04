// MatchTem.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

Mat g_srcImage, g_tempalteImage, g_resultImage;
int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;

void on_matching(int, void*)
{
	//��1�����ֲ�������ʼ��
	Mat srcImage;
	g_srcImage.copyTo(srcImage);

	//��2����ʼ�����ڽ������ľ���
	int resultImage_cols = g_srcImage.cols - g_tempalteImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_tempalteImage.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//��3������ƥ��ͱ�׼��
	matchTemplate(g_srcImage, g_tempalteImage, g_resultImage, g_nMatchMethod);
	normalize(g_resultImage, g_resultImage, 0, 2, NORM_MINMAX, -1, Mat());

	//��4��ͨ������ minMaxLoc ��λ��ƥ���λ��
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);

	//��5�����ڷ��� SQDIFF �� SQDIFF_NORMED, ԽС����ֵ���Ÿ��ߵ�ƥ����. ������ķ���, ��ֵԽ��ƥ��Ч��Խ��
	//�˾�����OpenCV2��Ϊ��
	//if( g_nMatchMethod  == CV_TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED )
	//�˾�����OpenCV3��Ϊ��
	if (g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//��6�����Ƴ����Σ�����ʾ���ս��
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(g_resultImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow("ԭʼͼ", srcImage);
	imshow("Ч��ͼ", g_resultImage);

}


int _tmain(int argc, _TCHAR* argv[])
{
	g_srcImage = imread("Pic//Test1.bmp");
	if (!g_srcImage.data)
	{
		cout << "ԭʼͼ��ȡʧ��" << endl;
		return -1;
	}
	g_tempalteImage = imread("Pic//Match.jpg");
	if (!g_tempalteImage.data)
	{
		cout << "ģ��ͼ��ȡʧ��" << endl;
		return -1;
	}

	namedWindow("ԭʼͼ", CV_WINDOW_AUTOSIZE);
	namedWindow("Ч��ͼ", CV_WINDOW_AUTOSIZE);
	createTrackbar("����", "ԭʼͼ", &g_nMatchMethod, g_nMaxTrackbarNum, on_matching);

	on_matching(0, NULL);


	waitKey(0);

	return 0;
}

