#include <iostream>
#include <maths.h>
#include <maths_image.h>

using namespace cv;

Mat matrix_double_to_Mat_64FC1(double *array, int row, int col)
{
	Mat img(row, col, CV_64FC1);
	double *ptmp = NULL;
	for (int i = 0; i < row; i++)
	{
		ptmp = img.ptr<double>(i);

		for (int j = 0; j < col; ++j)
		{
			ptmp[j] = *array++;
		}
	}
	return img;
}


// ��ͼƬ����ʽ�Ѿ�����ʾ����
void show_matrix_double_as_image_64FC1(double *array, int row, int col, int time_msec)
{
	Mat image = matrix_double_to_Mat_64FC1(array, row, col);

	// ��ʾͼƬ   
	imshow("ͼƬ", image);
	// �ȴ�time_msec�󴰿��Զ��ر�    
	waitKey(time_msec);
}
