/*#include <cv.h>
#include <highgui.h>
using namespace std;
int main()
{
	IplImage * test;
	test = cvLoadImage("D:\\pic.jpg");//ͼƬ·��
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
	// ����һ��ͼƬ����Ϸԭ����  
	Mat img = imread("D:\\pic.jpg");
	// ����һ����Ϊ "��Ϸԭ��"����  
	namedWindow("��Ϸԭ��");
	// �ڴ�������ʾ��Ϸԭ��  
	imshow("��Ϸԭ��", img);
	// �ȴ�6000 ms�󴰿��Զ��ر�  
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

//surf�㷨detector
int main(int argc, char** argv)
{
	//��0���ı�console������ɫ      
	system("color 2F");

	//��1������ԴͼƬ����ʾ  
	Mat srcImage1 = imread("pic.jpg", 1);
	if (!srcImage1.data)//����Ƿ��ȡ�ɹ�  
	{
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ�����Ƶ�ͼƬ����~�� \n"); return false;
	}
	imshow("ԭʼͼ1", srcImage1);

	//��2��������Ҫ�õ��ı�������  
	int minHessian = 400;//����SURF�е�hessian��ֵ������������  
	SurfFeatureDetector detector(minHessian);//����һ��SurfFeatureDetector��SURF�� ������������  
	std::vector<KeyPoint> keypoints_1, keypoints_2;//vectorģ�������ܹ�����������͵Ķ�̬���飬�ܹ����Ӻ�ѹ������  

	//��3������detect��������SURF�����ؼ��㣬������vector������  
	detector.detect(srcImage1, keypoints_1);

	//��4�����������ؼ���  
	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints(srcImage1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(srcImage1, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	//��5����ʾЧ��ͼ  
	imshow("��������Ч��ͼ1", img_keypoints_1);
	imshow("��������Ч��ͼ2", img_keypoints_2);

	waitKey(0);
	return 0;
}
/* laplace �ȱ�Ե���

//��0�������Ķ���  
Mat src, src_gray, dst, abs_dst;
int g_scale, g_delta;

static void on_Lap(int, void*);
void on_Lap(int, void*)
{
	//��5��ʹ��Laplace����  
	Laplacian(src_gray, dst, CV_16S, 3, g_scale, g_delta, BORDER_DEFAULT);
	//imshow("��cv16sͼ��ͼ��Laplace�任", dst);//��ɫ?

	//��6���������ֵ���������ת����8λ  
	convertScaleAbs(dst, abs_dst);

	//��7����ʾЧ��ͼ  
	imshow("��Ч��ͼ��ͼ��Laplace�任", abs_dst);
}
//��Ե���
int main()
{

	//��0������ grad_x �� grad_y ����  
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat srccanny;
	//��1������ԭʼͼ    
	src = imread("pic.jpg");  //����Ŀ¼��Ӧ����һ����Ϊ1.jpg���ز�ͼ  

	//��2����ʾԭʼͼ   
	imshow("��ԭʼͼ��", src);

	//��3��ʹ�ø�˹�˲���������  
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	//��4��ת��Ϊ�Ҷ�ͼ  
	cvtColor(src, src_gray, CV_RGB2GRAY);


	namedWindow("��Ч��ͼ��ͼ��Laplace�任", CV_WINDOW_AUTOSIZE);
	//5�����ֵ����onlap������Ҫ��΢����
	createTrackbar("����ֵdelta��", "��Ч��ͼ��ͼ��Laplace�任", &g_delta, 5, on_Lap);
	createTrackbar("��������", "��Ч��ͼ��ͼ��Laplace�任", &g_scale, 5, on_Lap);
	on_Lap(0, 0);

	/*
	//��3���� X�����ݶ�  
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	imshow("��Ч��ͼ�� X����Sobel", abs_grad_x);

	//��4����Y�����ݶ�  
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("��Ч��ͼ��Y����Sobel", abs_grad_y);

	//��5���ϲ��ݶ�(����)  
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	imshow("��Ч��ͼ�����巽��Sobel", dst);
	
	Canny(src, srccanny, 150, 100, 3);
	imshow("��Ч��ͼ��Canny��Ե���", srccanny);
	waitKey(0);

	return 0;
}
*/

/*//����alphaͼ
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
	//������alphaͨ����Mat  
	Mat mat(480, 640, CV_8UC4);
	createAlphaMat(mat);

	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try{
		imwrite("͸��Alphaֵͼ.png", mat, compression_params);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "ͼ��ת����PNG��ʽ��������%s\n", ex.what());
		return 1;
	}

	fprintf(stdout, "PNGͼƬ�ļ���alpha���ݱ������~\n");
	return 0;
	
}
***/


/***���� trackbar
//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------  
//     ������ȫ�ֱ�������  
//-----------------------------------------------------------------------------------------------  
int g_nContrastValue; //�Աȶ�ֵ  
int g_nBrightValue;  //����ֵ  
Mat g_srcImage, g_dstImage;
//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------  
//     ������ȫ�ֺ�������  
//-----------------------------------------------------------------------------------------------  
static void ContrastAndBright(int, void *);


//-----------------------------��ContrastAndBright( )������------------------------------------  
//     �������ı�ͼ��ԱȶȺ�����ֵ�Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void ContrastAndBright(int, void *)
{

	//��������  
	namedWindow("��ԭʼͼ���ڡ�", 1);

	//����forѭ����ִ������ g_dstImage(i,j) =a*g_srcImage(i,j) + b  
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

	//��ʾͼ��  
	imshow("��ԭʼͼ���ڡ�", g_srcImage);
	imshow("��Ч��ͼ���ڡ�", g_dstImage);
}

//-----------------------------------��main( )������--------------------------------------------  
//     ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ  
//-----------------------------------------------------------------------------------------------  

int main()
{
	//�ı����̨ǰ��ɫ�ͱ���ɫ  
	system("color 5F");

	//�����û��ṩ��ͼ��  
	g_srcImage = imread("pic.jpg");
	if (!g_srcImage.data) { printf("Oh��no����ȡg_srcImageͼƬ����~��\n"); return false; }
	g_dstImage = Mat::zeros(g_srcImage.size(), g_srcImage.type());

	//�趨�ԱȶȺ����ȵĳ�ֵ  
	g_nContrastValue = 80;
	g_nBrightValue = 80;

	//��������  
	namedWindow("��Ч��ͼ���ڡ�", 1);

	//�����켣��  
	createTrackbar("�Աȶȣ�", "��Ч��ͼ���ڡ�", &g_nContrastValue, 300, ContrastAndBright);
	createTrackbar("��   �ȣ�", "��Ч��ͼ���ڡ�", &g_nBrightValue, 200, ContrastAndBright);

	//���ûص�����  
	ContrastAndBright(g_nContrastValue, 0);
	ContrastAndBright(g_nBrightValue, 0);

	//���һЩ������Ϣ  
	cout << endl << "\t�š����ˣ�������������۲�ͼ��Ч��~\n\n"
		<< "\t���¡�q����ʱ�������˳�~!\n"
		<< "\n\n\t\t\t\tbyǳī";

	//���¡�q����ʱ�������˳�  
	while (char(waitKey(1)) != 'q') {}
	return 0;
}
***/
