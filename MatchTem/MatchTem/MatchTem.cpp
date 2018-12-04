// MatchTem.cpp : 定义控制台应用程序的入口点。
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
	//【1】给局部变量初始化
	Mat srcImage;
	g_srcImage.copyTo(srcImage);

	//【2】初始化用于结果输出的矩阵
	int resultImage_cols = g_srcImage.cols - g_tempalteImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_tempalteImage.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//【3】进行匹配和标准化
	matchTemplate(g_srcImage, g_tempalteImage, g_resultImage, g_nMatchMethod);
	normalize(g_resultImage, g_resultImage, 0, 2, NORM_MINMAX, -1, Mat());

	//【4】通过函数 minMaxLoc 定位最匹配的位置
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);

	//【5】对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值有着更高的匹配结果. 而其余的方法, 数值越大匹配效果越好
	//此句代码的OpenCV2版为：
	//if( g_nMatchMethod  == CV_TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED )
	//此句代码的OpenCV3版为：
	if (g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//【6】绘制出矩形，并显示最终结果
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(g_resultImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow("原始图", srcImage);
	imshow("效果图", g_resultImage);

}


int _tmain(int argc, _TCHAR* argv[])
{
	g_srcImage = imread("Pic//Test1.bmp");
	if (!g_srcImage.data)
	{
		cout << "原始图读取失败" << endl;
		return -1;
	}
	g_tempalteImage = imread("Pic//Match.jpg");
	if (!g_tempalteImage.data)
	{
		cout << "模板图读取失败" << endl;
		return -1;
	}

	namedWindow("原始图", CV_WINDOW_AUTOSIZE);
	namedWindow("效果图", CV_WINDOW_AUTOSIZE);
	createTrackbar("方法", "原始图", &g_nMatchMethod, g_nMaxTrackbarNum, on_matching);

	on_matching(0, NULL);


	waitKey(0);

	return 0;
}

