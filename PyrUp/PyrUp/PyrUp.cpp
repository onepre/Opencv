// PyrUp.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	Mat srcImage = imread("Pic//Test2.jpg");
	if (srcImage.empty())
	{
		return -1;
	}
	Mat temImage, dstImage;
	temImage = srcImage;
	imshow("��ԭʼͼ��", srcImage);
	//pyrUp(temImage, dstImage, Size(temImage.cols * 2, temImage.rows * 2));
	pyrDown(temImage, dstImage, Size(temImage.cols / 2, temImage.cols / 2));
	imshow("��Ч��ͼ��", dstImage);
	waitKey(0);
	return 0;
}

