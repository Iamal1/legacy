/*#include <cv.h>
#include <highgui.h>
using namespace std;
int main()
{
	IplImage * test;
	test = cvLoadImage("D:\\pic.jpg");//图片路径
	cvNamedWindow("test_demo", 1);
	cvShowImage("test_demo", test);
	cvWaitKey(0);
	cvDestroyWindow("test_demo");
	cvReleaseImage(&test);
	return 0;
}

#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;

int main()
{
	// 读入一张图片（游戏原画）  
	Mat img = imread("D:\\pic.jpg");
	// 创建一个名为 "游戏原画"窗口  
	namedWindow("游戏原画");
	// 在窗口中显示游戏原画  
	imshow("游戏原画", img);
	// 等待6000 ms后窗口自动关闭  
	waitKey(6000);
}
*/

#include <vector>
#include <stdio.h>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>  

using namespace cv;
using namespace std;

//surf算法detector
int main(int argc, char** argv)
{
	//【0】改变console字体颜色      
	system("color 2F");

	//【1】载入源图片并显示  
	Mat srcImage1 = imread("pic.jpg", 1);
	if (!srcImage1.data)//检测是否读取成功  
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定名称的图片存在~！ \n"); return false;
	}
	imshow("原始图1", srcImage1);

	//【2】定义需要用到的变量和类  
	int minHessian = 400;//定义SURF中的hessian阈值特征点检测算子  
	SurfFeatureDetector detector(minHessian);//定义一个SurfFeatureDetector（SURF） 特征检测类对象  
	std::vector<KeyPoint> keypoints_1, keypoints_2;//vector模板类是能够存放任意类型的动态数组，能够增加和压缩数据  

	//【3】调用detect函数检测出SURF特征关键点，保存在vector容器中  
	detector.detect(srcImage1, keypoints_1);

	//【4】绘制特征关键点  
	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints(srcImage1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(srcImage1, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//【5】显示效果图  
	imshow("特征点检测效果图1", img_keypoints_1);
	imshow("特征点检测效果图2", img_keypoints_2);

	waitKey(0);
	return 0;
}
/* laplace 等边缘检测

//【0】变量的定义  
Mat src, src_gray, dst, abs_dst;
int g_scale, g_delta;

static void on_Lap(int, void*);
void on_Lap(int, void*)
{
	//【5】使用Laplace函数  
	Laplacian(src_gray, dst, CV_16S, 3, g_scale, g_delta, BORDER_DEFAULT);
	//imshow("【cv16s图】图像Laplace变换", dst);//灰色?

	//【6】计算绝对值，并将结果转换成8位  
	convertScaleAbs(dst, abs_dst);

	//【7】显示效果图  
	imshow("【效果图】图像Laplace变换", abs_dst);
}
//边缘检测
int main()
{

	//【0】创建 grad_x 和 grad_y 矩阵  
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat srccanny;
	//【1】载入原始图    
	src = imread("pic.jpg");  //工程目录下应该有一张名为1.jpg的素材图  

	//【2】显示原始图   
	imshow("【原始图】", src);

	//【3】使用高斯滤波消除噪声  
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	//【4】转换为灰度图  
	cvtColor(src, src_gray, CV_RGB2GRAY);


	namedWindow("【效果图】图像Laplace变换", CV_WINDOW_AUTOSIZE);
	//5是最大值，在onlap里面需要稍微调整
	createTrackbar("参数值delta：", "【效果图】图像Laplace变换", &g_delta, 5, on_Lap);
	createTrackbar("缩放因子", "【效果图】图像Laplace变换", &g_scale, 5, on_Lap);
	on_Lap(0, 0);

	/*
	//【3】求 X方向梯度  
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	imshow("【效果图】 X方向Sobel", abs_grad_x);

	//【4】求Y方向梯度  
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("【效果图】Y方向Sobel", abs_grad_y);

	//【5】合并梯度(近似)  
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	imshow("【效果图】整体方向Sobel", dst);
	
	Canny(src, srccanny, 150, 100, 3);
	imshow("【效果图】Canny边缘检测", srccanny);
	waitKey(0);

	return 0;
}
*/

/*//创建alpha图
void createAlphaMat(Mat &mat)
{
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			Vec4b&rgba = mat.at<Vec4b>(i, j);
			rgba[0] = UCHAR_MAX;
			rgba[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) *UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) *UCHAR_MAX);
			rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
		}
	}
}

int main()
{
	//创建带alpha通道的Mat  
	Mat mat(480, 640, CV_8UC4);
	createAlphaMat(mat);

	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try{
		imwrite("透明Alpha值图.png", mat, compression_params);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "图像转换成PNG格式发生错误：%s\n", ex.what());
		return 1;
	}

	fprintf(stdout, "PNG图片文件的alpha数据保存完毕~\n");
	return 0;
	
}
***/


/***创建 trackbar
//-----------------------------------【全局变量声明部分】--------------------------------------  
//     描述：全局变量声明  
//-----------------------------------------------------------------------------------------------  
int g_nContrastValue; //对比度值  
int g_nBrightValue;  //亮度值  
Mat g_srcImage, g_dstImage;
//-----------------------------------【全局函数声明部分】--------------------------------------  
//     描述：全局函数声明  
//-----------------------------------------------------------------------------------------------  
static void ContrastAndBright(int, void *);


//-----------------------------【ContrastAndBright( )函数】------------------------------------  
//     描述：改变图像对比度和亮度值的回调函数  
//-----------------------------------------------------------------------------------------------  
static void ContrastAndBright(int, void *)
{

	//创建窗口  
	namedWindow("【原始图窗口】", 1);

	//三个for循环，执行运算 g_dstImage(i,j) =a*g_srcImage(i,j) + b  
	for (int y = 0; y < g_srcImage.rows; y++)
	{
		for (int x = 0; x < g_srcImage.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				g_dstImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>((g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y, x)[c]) + g_nBrightValue);
			}
		}
	}

	//显示图像  
	imshow("【原始图窗口】", g_srcImage);
	imshow("【效果图窗口】", g_dstImage);
}

//-----------------------------------【main( )函数】--------------------------------------------  
//     描述：控制台应用程序的入口函数，我们的程序从这里开始  
//-----------------------------------------------------------------------------------------------  

int main()
{
	//改变控制台前景色和背景色  
	system("color 5F");

	//读入用户提供的图像  
	g_srcImage = imread("pic.jpg");
	if (!g_srcImage.data) { printf("Oh，no，读取g_srcImage图片错误~！\n"); return false; }
	g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

	//设定对比度和亮度的初值  
	g_nContrastValue = 80;
	g_nBrightValue = 80;

	//创建窗口  
	namedWindow("【效果图窗口】", 1);

	//创建轨迹条  
	createTrackbar("对比度：", "【效果图窗口】", &g_nContrastValue, 300, ContrastAndBright);
	createTrackbar("亮   度：", "【效果图窗口】", &g_nBrightValue, 200, ContrastAndBright);

	//调用回调函数  
	ContrastAndBright(g_nContrastValue, 0);
	ContrastAndBright(g_nBrightValue, 0);

	//输出一些帮助信息  
	cout << endl << "\t嗯。好了，请调整滚动条观察图像效果~\n\n"
		<< "\t按下“q”键时，程序退出~!\n"
		<< "\n\n\t\t\t\tby浅墨";

	//按下“q”键时，程序退出  
	while (char(waitKey(1)) != 'q') {}
	return 0;
}
***/
