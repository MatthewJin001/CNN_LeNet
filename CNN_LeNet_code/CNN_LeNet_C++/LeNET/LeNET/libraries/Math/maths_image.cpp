
#include <maths.h>
#include <maths_image.h>


Mat mat2x2_to_Mat(uchar **array, int row, int col)
{
	Mat img(row, col, CV_8UC1);
	uchar *ptmp = NULL;
	for (int i = 0; i <row; ++i)
	{
		ptmp = img.ptr<uchar>(i);

		for (int j = 0; j < col; ++j)
		{
			ptmp[j] = array[i][j];
		}
	}

	return img;
}


void show_mat2x2_as_image(uchar **array, int row, int col, int time_msec)
{
	Mat image = mat2x2_to_Mat(array, row, col);

	// ��ʾͼƬ   
	imshow("ͼƬ", image);
	// �ȴ�time_msec�󴰿��Զ��ر�    
	waitKey(time_msec);
}
