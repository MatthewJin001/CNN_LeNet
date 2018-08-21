#pragma once

// opencv
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class CNN
{
public:

	CNN(float alpha, float eta, int batchsize, int epochs)
		:_alpha(alpha), _eta(eta), _batchsize(batchsize), _epochs(epochs)
	{
		cout << "already init CNN" << endl;

		_ERR.assign(_epochs, 0);
	}
	
	// ѵ��
	void train(vector<Mat> train_x, vector<vector<double>> train_y);

	// ���ش�����
	double test(vector<Mat> test_x, vector<vector<double>> test_y);


	//  ����Ҫ�ĺ���

	// ���ص�������
	int get_epochs() { return _epochs; }

	// �������ε����ľ������
	vector<double> get_ERR() { return _ERR; }

private:
	

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

};
