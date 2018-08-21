#include <CNN.h>
#include <maths.h>
#include <time.h>


#include<iostream>
using namespace std;

void CNN::train(vector<Mat> train_x, vector<vector<double>> train_y)
{
	cout << "begin to train" << endl;

	if (train_x.size() != train_y.size())
	{
		cout << "train_x size is not same as train_y size!" << endl << "stop CNN trainning!" << endl;
		return;
	}

	int m = train_x.size();// ѵ����������
	int numbatches = ceil(m / _batchsize);// "ѵ�����������һ��" ����Ȩֵ���µĴ���

	for (int I = 0; I < _epochs; I++)
	{
		cout << "epoch " << I+1 << "/" << _epochs << endl;

		clock_t tic = clock(); //��ȡ���뼶��Ŀ
		



		clock_t toc = clock(); //��ȡ���뼶��Ŀ
		cout << "time has elapsed: " << (double)(toc - tic) / 1000 << " seconds" << endl;
	}
}


double CNN::test(vector<Mat> test_x, vector<vector<double>> test_y)
{
	cout << "begin to test" << endl;
	return 0;
}
