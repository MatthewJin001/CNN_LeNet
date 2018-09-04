#pragma once

// opencv
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream>
#include <vector>
#include <string>
#include <maths.h>
#include <Array.h>


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

	// ��ǰ����������ȫ���Ӳ������ȫ���Ӳ����ΪX_vector��
	vector<Array3Dd> X;// ע����_batchsize���������ͬʱ�������Բ���2D������3D��ά��Ϊ[_batchsize, iSizePic[0], iSizePic[1]]

	// ǰһ��ͨ���Ե�ǰ��ͨ���ľ����
	vector<vector<Array2Dd>> Ker;// Ker[I][J], IΪǰһ��ͨ������JΪ��ǰ��ͨ������ֻ��Ծ���㣬������˲���������

	// ǰһ��ͨ���Ե�ǰ��ͨ���ľ���˵�ƫ��
	vector<vector<Array2Dd>> Ker_delta;// Ker_delta[I][J], IΪǰһ��ͨ������JΪ��ǰ��ͨ������ֻ��Ծ���㣬������˲���������

	// ��ǰ������һ�������Ȩֵ
	Array2Dd W;// ֻ���ȫ���Ӳ㣬������˲���������
			   // ע����W[I��][J��],IΪ��ǰ��ȫ�������������JΪ��ǰ����Ŀ
			   // ��ô��⣺�ò���J��������ǵ�Ȼ��J���ˣ�ÿ�������I������ǰ������룬�Ǿ���I����

	// ������
	Array2Dd W_delta;// ֻ���ȫ���Ӳ㣬������˲���������
					 // ע����W_delta[I��][J��],IΪ��ǰ��ȫ�������������JΪ��ǰ����Ŀ

	// ��ǰ�����ͨ���ļ���ƫ��
	vector<double> B;

	// ������
	vector<double> B_delta;

	// ��ǰ�����ͨ���ĳ���ƫ��
	vector<double> Beta;// ֻ����²����㣬������˲���������

	// ������
	vector<double> Beta_delta;// ֻ����²����㣬������˲���������

	// �²���������
	vector<Array3Dd> X_down;// ע����_batchsize���������ͬʱ�������Բ���2D������3D��ά��Ϊ[��_batchsize, ��iChannel*iSizePic[0]*iSizePic[1]]
							// ֻ����²����㣬������˲���������

	// ȫ���Ӳ�������������һ��Ϊȫ���Ӳ�ĵ�ǰ���������ͼ��ϳɵ�һ������
	Array2Dd X_vector;// ע����_batchsize���������ͬʱ�������Բ���һά����������2D��ά��Ϊ[��_batchsize, ��iChannel]
					// ֻ���ȫ���Ӳ��������Լ���һ��Ϊȫ���Ӳ�ķ�ȫ���Ӳ㣨����LeNet�ĵڶ����������㣩��������˲���������

	// ��ǰ���������(�в�)������ǰ������������ƫ��
	vector<Array3Dd> Delta;// ע����_batchsize���������ͬʱ�������Բ���2D������3D��ά��Ϊ[_batchsize, iSizePic[0], iSizePic[1]]
						   // ע��ֻ��Է�ȫ���Ӳ�

	// ��ǰ���������(�в�)������ǰ������������ƫ��
	Array2Dd Delta_vec;// ����һά������������_batchsize�У������Ƕ�ά�ġ�
					   // ע��ֻ���ȫ���Ӳ㣬�Լ��²�Ϊȫ���Ӳ�Ľ������㡢����㡢�������з���ͼ����Ĳ�

} Layer;


class CNN
{
public:

	// ��ʼ��CNN��
	CNN(const vector<Layer> &layers, float alpha, float eta, int batchsize, int epochs, activation_function_type activ_func_type, down_sample_type down_samp_type);
	
	// CNN���磬ѵ��
	void train(const Array3Dd &train_x, const Array2Dd &train_y);

	// CNN���磬���ԣ����ش�����
	double test(const Array3Dd &test_x, const Array2Dd &test_y);


	////////////////////// ����Ҫ���� /////////////////////////////////////////////

	// ���ص�������
	int get_epochs() { return _epochs; }

	// �������ε����ľ������
	vector<double> get_ERR() { return _ERR; }

private:
	
	// ��������ṹ����CNN.layers, ��ʼ��һ��CNN����
	void init();

	// CNN���磬�������(�������㷨,������convn�÷�,�����������ӳ��)
	void feed_forward(const Array3Dd &train_x);

	// CNN���磬���򴫲�(�������㷨)
	void back_propagation(const Array2Dd &train_y);

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

	// ���������
	activation_function_type _activation_func_type;

	// ���������ػ�������
	down_sample_type _down_sample_type;

	// ���ε����ľ������
	vector<double> _ERR;

	// ��ǰ�ֵĵ�ǰ���εľ������
	double _err;

	// �������������������һ����������ͼ
	Array2Dd _Y;// ע����_batchsize���������ͬʱ����������2D��ά��Ϊ[��_batchsize, ��iChannel����10��]
};
