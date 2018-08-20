#include <stdio.h>
#include <iostream>
#include <Main_LeNet.h>
#include <maths.h>
#include <CNN.h>

using namespace std;

int main()
{
	/*
	// ����һ��ͼƬ
	Mat img = imread("file/lena.jpg");

	// ��ʾͼƬ   
	imshow("ͼƬ", img);
	// �ȴ�1000ms�󴰿��Զ��ر�    
	waitKey(1000);
	
	
	VideoCapture cap(0);
	Mat frame;
	while (1)
	{
		cap >> frame;
		imshow("��������ͷ", frame);
		waitKey(10);
	}
	*/

	// load ����ͼƬ
	Mat train_x = imread("file/lena.jpg");
	uchar train_y[10][1000] = { 0 };
	set_target_class_one2ten(train_y);
	print_matrix((unsigned char*)train_y, 10, 1000);
	show_matrix_as_image((uchar*)train_y, 10, 1000, 6000);


	// ��ʾͼƬ   
	imshow("ͼƬ", train_x);
	// �ȴ�1000ms�󴰿��Զ��ر�    
	waitKey(1000);

	// �����ʼ������
	float alpha = 2;// ѧϰ��[0.1,3]
	float eta = 0.5f;// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	int batchsize = 10;// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int epochs = 25;// ѵ���������������

	// ��ʼ��CNN
	//CNN LeNet(alpha, eta, batchsize, epochs);


	return 0;
}


void set_target_class_one2ten(unsigned char target_class[][1000])
{
	int i, j;
	int segment_size = 1000 / 10;
	for (i = 0; i <= 9; i++)
	{
		for (j = i * segment_size; j < (i + 1) * segment_size; j++)
		{
			target_class[i][j] = 255;
		}
	}
}

