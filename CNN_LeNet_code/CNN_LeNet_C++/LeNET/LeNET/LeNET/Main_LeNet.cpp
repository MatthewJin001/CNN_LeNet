// ������main���
#include <iostream>
#include <Main_LeNet.h>
#include <maths.h>
#include <CNN.h>
#include <string>
#include <time.h>


void test()
{
	;
}


int main()
{
	//test();

	//*

	// ****************************** ����ѵ���� ***************************************************** //

	clock_t tic_LeNet = clock();// LeNet���м�ʱ

	// ����ѵ����������ͼƬ
	vector<Mat> train_x_Mat;
	// �ļ���ȡ��ַ
	string file_addr = "../../../MnistData/TrainImg";
	// ������ȡͼƬ
	read_batch_images(file_addr, "bmp", 1, 1000, train_x_Mat);
	// תΪ�Ҷ�ͼ
	images_convert_to_64FC1(train_x_Mat);

	// ��MatͼƬ��ʽתΪCNN���õ�Array3Dd��ʽ
	Array3Dd train_x(train_x_Mat);
	// ��һ��
	train_x.normalize();
	// ��ʾ����ȡ��ͼƬ������Ҫι��CNN��ѵ������
	train_x.show_specified_images_64FC1("Multiple Images", CvSize(26, 18), CvSize(32, 32), 1000);

	// ����ѵ������������ǩ
	Array2Dd train_y;
	train_y.class_0_to_9(1000);
	train_y.show_image_64FC1(700);

	// ****************************** �������Լ� ***************************************************** //

	// �������Լ�������ͼƬ
	Array3Dd test_x = train_x;

	// �������Լ���������ǩ
	Array2Dd test_y = train_y;

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
	double alpha = 2;// ѧϰ��[0.1,3]
	double eta = 0.5f;// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
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

	cout << "error history is �� " << endl;
	print(LeNet.get_ERR());

	// ���Ƴ����������ʷ����
	show_curve_image(get_vector_n2m(0, LeNet.get_epochs()-1), LeNet.get_ERR() * 50.0, 25, 30000);

	clock_t toc_LeNet = clock();// LeNet���м�ʱ

	cout << "LeNet total time: " << (double)(toc_LeNet - tic_LeNet) / 1000 << " seconds" << endl;
	//*/
	return 0;
}
