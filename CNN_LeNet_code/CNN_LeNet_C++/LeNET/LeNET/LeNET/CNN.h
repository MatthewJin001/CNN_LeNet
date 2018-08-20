#pragma once

// opencv
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  

template <typename T>

class CNN
{
public:

	CNN(float alpha, float eta, int batchsize, int epochs)
		:alpha(alpha), eta(eta), batchsize(batchsize), epochs(epochs)
	{

	}
	
	//train();

	// ѧϰ��[0.1,3]
	float alpha;

	// ����ϵ��[0,0.95], >=1��������==0Ϊ���ù�����
	float eta;

	// ÿ����batchsize����������һ��delta����һ��Ȩֵ��ÿʮ��������ƽ�����е���
	int batchsize;

	// ѵ���������������
	int epochs;

private:
	
};
