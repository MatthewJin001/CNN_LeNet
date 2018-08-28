// ������main���
#include <iostream>
#include <Main_LeNet.h>
#include <maths.h>
#include <CNN.h>
#include <string>


void test()
{
	Mat train_x1 = imread("file/2.bmp", 0);//��ȡ�Ҷ�ͼ
										   // normalize ��һ���� ��0~255��uchar���ͱ�Ϊ0~1��double����
	train_x1.convertTo(train_x1, CV_64FC1, 1 / 255.0);//����dstΪĿ��ͼ�� CV_32FC3ΪҪת��������

	Array2Dd xxx, yyy;

	xxx.create(3, 2, 2);
	yyy.create(3, 2, 2.2);

	Array3Dd zzz;
	zzz.push_back(xxx);
	zzz.push_back(yyy);

	zzz.at(0).print();
	zzz.at(1).print();

	return;
}


int main()
{
	test();

	/*

	// ****************************** ����ѵ���� ***************************************************** //

	// ����ѵ����������ͼƬ
	vector<Mat> train_x_Mat;
	// �ļ���ȡ��ַ
	string file_addr = "../../../MnistData/TrainImg";
	// ������ȡͼƬ
	read_batch_images(file_addr, "bmp", 1, 1000, train_x_Mat);
	// תΪ�Ҷ�ͼ
	images_convert_to_64FC1(train_x_Mat);
	// ��MatͼƬ��ʽתΪCNN��array2D��ʽ
	vector<array2D> train_x = vector_image_64FC1_to_vector_array2D(train_x_Mat);
	// ��һ��
	normalize_vector_array2D_from_0_to_1(train_x);
	// ��ʾ����ȡ��ͼƬ������Ҫι��CNN��ѵ������
	vector_array2D_show_one_window("Multiple Images", train_x, CvSize(26, 18), CvSize(32, 32), 700);

	// ����ѵ������������ǩ
	vector<vector<double>> train_y;
	set_target_class_one2ten(train_y,1000);
	show_vector_vector_double_as_image_64FC1(train_y, 700);

	// ****************************** �������Լ� ***************************************************** //

	// �������Լ�������ͼƬ
	vector<array2D> test_x = train_x;

	// �������Լ���������ǩ
	vector<vector<double>> test_y = train_y;

	// ****************************** ��ʼ��CNN ****************************************************** //

	// CNN����ṹ����
	vector<Layer> layers;

	Layer input_layer_1;// ��һ�㣺�����
	input_layer_1.type = 'i'; input_layer_1.iChannel = 1; input_layer_1.iSizePic[0] = 32; input_layer_1.iSizePic[1] = 32;
	layers.push_back(input_layer_1);

	Layer convolutional_layer_2;// �ڶ��㣺�����
	convolutional_layer_2.type = 'c'; convolutional_layer_2.iChannel = 2; convolutional_layer_2.iSizeKer = 5;
	layers.push_back(convolutional_layer_2);

	Layer subsampling_layer_3;// �����㣺��������
	subsampling_layer_3.type = 's'; subsampling_layer_3.iSample = 2;
	layers.push_back(subsampling_layer_3);

	Layer convolutional_layer_4;// ���Ĳ㣺�����
	convolutional_layer_4.type = 'c'; convolutional_layer_4.iChannel = 4; convolutional_layer_4.iSizeKer = 5;
	layers.push_back(convolutional_layer_4);

	Layer subsampling_layer_5;// ����㣺��������
	subsampling_layer_5.type = 's'; subsampling_layer_5.iSample = 2;
	layers.push_back(subsampling_layer_5);

	Layer fully_connected_layer_6;// �����㣺ȫ���Ӳ�
	fully_connected_layer_6.type = 'f'; fully_connected_layer_6.iChannel = 120;
	layers.push_back(fully_connected_layer_6);

	Layer fully_connected_layer_7;// ���߲㣺ȫ���Ӳ�
	fully_connected_layer_7.type = 'f'; fully_connected_layer_7.iChannel = 84;
	layers.push_back(fully_connected_layer_7);

	Layer fully_connected_layer_8;// �ڰ˲㣺ȫ���Ӳ㣨����㣩
	fully_connected_layer_8.type = 'f'; fully_connected_layer_8.iChannel = 10;
	layers.push_back(fully_connected_layer_8);

	// �����ʼ������
	float alpha = 2;// ѧϰ��[0.1,3]
	float eta = 0.5f;// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	int batchsize = 10;// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int epochs = 25;// ѵ���������������
	activation_function_type activ_func_type = SoftMax;// ���������
	down_sample_type down_samp_type = MeanPooling;// ���������ػ�������

	// ��������ṹ����CNN.layers����ʼ��һ��CNN����
	CNN LeNet(layers, alpha, eta, batchsize, epochs, activ_func_type, down_samp_type);

	// ****************************** CNNѵ�� ******************************************************** //

	// CNN����ѵ��
	LeNet.train(train_x, train_y);

	// ****************************** CNN���� ******************************************************** //

	// CNN������ԣ��ò�������������
	double error_rate = LeNet.test(test_x, test_y);

	// ���Խ�����
	cout << "error rate is �� " << error_rate * 100 << "%" << endl;

	// ���Ƴ����������ʷ����
	show_curve_image(get_vector_double_n2m(0, LeNet.get_epochs()-1), LeNet.get_ERR(), 25, 1000);

	//*/
	return 0;
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
