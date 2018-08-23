#pragma once

// opencv
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>
#include <vector>
#include <string>
#include <maths.h>

using namespace std;
using namespace cv;

typedef struct{
	// ��ǰ������
	char type;// ����㣺i������㣺c���������㣺s��ȫ���Ӳ㣺f

	// ��ǰ���ͨ����Ŀ
	int iChannel;

	// ��������
	int iSample;// ֻ��Խ������㣬������˲���������

	// ��ǰ�������ͼƬ��С
	int iSizePic[2];

	// ��ǰ��ľ���˴�С[row col]
	int iSizeKer;// ֻ��Ծ���㣬������˲���������

	// ��ǰ������
	vector<array_3D_double> X;// ע����_batchsize���������ͬʱ�������Բ���2D������3D��ά��Ϊ[_batchsize, iSizePic[0], iSizePic[1]]

	// ǰһ��ͨ���Ե�ǰ��ͨ���ľ����
	vector<vector<array_2D_double>> Ker;// Ker[I][J], IΪǰһ��ͨ������JΪ��ǰ��ͨ������ֻ��Ծ���㣬������˲���������

	// ǰһ��ͨ���Ե�ǰ��ͨ���ľ���˵�ƫ��
	vector<vector<array_2D_double>> Ker_delta;// Ker_delta[I][J], IΪǰһ��ͨ������JΪ��ǰ��ͨ������ֻ��Ծ���㣬������˲���������

	// ��ǰ������һ�������Ȩֵ
	array_2D_double W;// ֻ���ȫ���Ӳ㣬������˲���������

	// ������
	array_2D_double W_delta;// ֻ���ȫ���Ӳ㣬������˲���������

	// ��ǰ�����ͨ���ļ���ƫ��
	vector<double> B;

	// ������
	vector<double> B_delta;

	// ��ǰ�����ͨ���ĳ���ƫ��
	vector<double> Beta;// ֻ����²����㣬������˲���������

	// ������
	vector<double> Beta_delta;// ֻ����²����㣬������˲���������

} Layer;


class CNN
{
public:

	// ��ʼ��CNN��
	CNN(vector<Layer> layers, float alpha, float eta, int batchsize, int epochs)
		:_layers(layers), _alpha(alpha), _eta(eta), _batchsize(batchsize), _epochs(epochs)
	{
		// ��������ṹ����CNN.layers, ��ʼ��һ��CNN����
		init();

		_ERR.assign(_epochs, 0);// �����ε����ľ�������ʼ��Ϊ0
		_err = 0;// ����ǰ�ֵĵ�ǰ���εľ�������ʼ��Ϊ0

		cout << "CNN has initialised!" << endl;
	}
	
	// CNN���磬ѵ��
	void train(const vector<array_2D_double> &train_x, const vector<vector<double>> &train_y);

	// CNN���磬���ԣ����ش�����
	double test(const vector<array_2D_double> &test_x, const vector<vector<double>> &test_y);


	////////////////////// ����Ҫ���� /////////////////////////////////////////////

	// ���ص�������
	int get_epochs() { return _epochs; }

	// �������ε����ľ������
	vector<double> get_ERR() { return _ERR; }

private:
	
	// ��������ṹ����CNN.layers, ��ʼ��һ��CNN����
	void init();

	// CNN���磬�������(�������㷨,������convn�÷�,�����������ӳ��)
	void feed_forward(const vector<array_2D_double> &train_x);

	// CNN���磬���򴫲�(�������㷨)
	void back_propagation(const vector<vector<double>> &train_y);

	// CNN���磬������������Ȩֵ����(���ӹ�����)
	void update(void);

	vector<Layer> _layers;

	// ѧϰ��[0.1,3]
	float _alpha;

	// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	float _eta;

	// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int _batchsize;

	// ѵ���������������
	int _epochs;

	// ���ε����ľ������
	vector<double> _ERR;

	// ��ǰ�ֵĵ�ǰ���εľ������
	double _err;
};
