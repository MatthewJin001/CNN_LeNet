// ������main���
#include <iostream>
#include <Main_LeNet.h>
#include <maths.h>
#include <CNN.h>
#include <string>


void test()
{

}

int main()
{
	test();
	
	// ******************************* ����ѵ���� **************************************************** //

	// ����ѵ����������ͼƬ
	vector<Mat> train_x;
	string file_addr = "../../../MnistData/TrainImg";
	read_batch_images(file_addr, "bmp", 1, 1000, train_x);
	images_convert_to_64FC1(train_x);
	multi_images_64FC1_show_one_window("Multiple Images", train_x, CvSize(26, 18), CvSize(32, 32), 1000);
	
	// ����ѵ������������ǩ
	vector<vector<double>> train_y;
	set_target_class_one2ten(train_y,1000);
	show_vector_vector_double_as_image_64FC1(train_y, 1000);

	// ****************************** �������Լ� ***************************************************** //

	// �������Լ�������ͼƬ
	vector<Mat> test_x = train_x;

	// �������Լ���������ǩ
	vector<vector<double>> test_y;
	test_y.assign(train_y.begin(), train_y.end());

	// ****************************** ��ʼ��CNN ****************************************************** //

	// �����ʼ������
	float alpha = 2;// ѧϰ��[0.1,3]
	float eta = 0.5f;// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	int batchsize = 10;// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int epochs = 25;// ѵ���������������

	// ��ʼ��CNN
	CNN LeNet(alpha, eta, batchsize, epochs);

	// ****************************** CNNѵ�� ******************************************************** //

	LeNet.train(train_x, train_y);

	// ****************************** CNN���� ******************************************************** //

	double error_rate = LeNet.test(test_x, test_y);

	// ���Խ�����
	cout << "error rate is �� " << error_rate * 100 << "%" << endl;

	// ���Ƴ����������ʷ����
	show_curve_image(get_vector_double_n2m(0, LeNet.get_epochs()-1), LeNet.get_ERR(), 25, 1000);

	//*/
	return 0;
}


void set_target_class_one2ten(vector<vector<double>> &target_class, int length)
{
	int segment_size = length / 10;
	int i, j;
	vector<double> one_hot;

	for (i = 0; i <= 8; i++)
	{
		one_hot.assign(10, 0);
		one_hot.at(i) = 1;

		for (j = 0; j < segment_size; j++)
		{
			target_class.push_back(one_hot);
		}
	}

	// ��ֹlength��10������������length=1003�����������10Ϊ��900��1003��������900��1000��
	one_hot.assign(10, 0);
	one_hot.at(i) = 1;
	for (j = i * segment_size; j < length; j++)
	{
		target_class.push_back(one_hot);
	}
}


/*
Mat train_x1 = imread("file/2.bmp",0);//��ȡ�Ҷ�ͼ
// normalize ��һ���� ��0~255��uchar���ͱ�Ϊ0~1��double����
train_x1.convertTo(train_x1, CV_64FC1, 1 / 255.0);//����dstΪĿ��ͼ�� CV_32FC3ΪҪת��������
// ��ʾͼƬ
imshow("ͼƬ", train_x1);
// �ȴ�1000ms�󴰿��Զ��ر�
waitKey(2000);
// ��ͼƬ�Ծ������ʽ��ʾ���������ڲ鿴ͼƬÿһ���ص�ֵ��
show_image_64FC1_as_matrix_double(train_x1);

double train_y[10][1000] = { 0 };
set_target_class_one2ten(train_y);
// ��ͼƬ����ʽ�Ѿ�����ʾ����
show_matrix_double_as_image_64FC1(train_y[0], 10, 1000, 6000);
*/
