//
#include <iostream>
#include <Main_LeNet.h>
#include <maths.h>
#include <CNN.h>

using namespace std;

int main()
{
	// load ����ͼƬ
	Mat train_x = imread("file/2.bmp",0);//��ȡ�Ҷ�ͼ

	vector<Mat> imgs(2);
	imgs[0] = imread("file/1.bmp", 0);
	imgs[1] = imread("file/3.bmp", 0);

	imgs[0].convertTo(imgs[0], CV_64FC1, 1 / 255.0);//����dstΪĿ��ͼ�� CV_64FC1ΪҪת��������
	imgs[1].convertTo(imgs[1], CV_64FC1, 1 / 255.0);//����dstΪĿ��ͼ�� CV_64FC1ΪҪת��������

	multi_image_64FC1_putin_one_window("Multiple Images", imgs, CvSize(1, 2), CvSize(32, 32), 5000);

	//return 0;

	// normalize ��һ���� ��0~255��uchar���ͱ�Ϊ0~1��double����
	train_x.convertTo(train_x, CV_64FC1, 1 / 255.0);//����dstΪĿ��ͼ�� CV_32FC3ΪҪת��������

	// ��ʾͼƬ   
	imshow("ͼƬ", train_x);
	// �ȴ�1000ms�󴰿��Զ��ر�    
	waitKey(2000);

	return 0;

	// ��ͼƬ�Ծ������ʽ��ʾ���������ڲ鿴ͼƬÿһ���ص�ֵ��
	show_image_64FC1_as_matrix_double(train_x);

	double train_y[10][1000] = { 0 };
	set_target_class_one2ten(train_y);
	// ��ͼƬ����ʽ�Ѿ�����ʾ����
	show_matrix_double_as_image_64FC1(train_y[0], 10, 1000, 6000);

	return 0;
	

	// �����ʼ������
	float alpha = 2;// ѧϰ��[0.1,3]
	float eta = 0.5f;// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	int batchsize = 10;// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int epochs = 25;// ѵ���������������

	// ��ʼ��CNN
	//CNN LeNet(alpha, eta, batchsize, epochs);


	return 0;
}


void set_target_class_one2ten(double target_class[][1000])
{
	int i, j;
	int segment_size = 1000 / 10;
	for (i = 0; i <= 9; i++)
	{
		for (j = i * segment_size; j < (i + 1) * segment_size; j++)
		{
			target_class[i][j] = 1.0;
		}
	}
}

