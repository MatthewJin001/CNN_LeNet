#include <CNN.h>
#include <maths.h>
#include <time.h>
#include <iostream>


using namespace std;


// CNN���磬ѵ��
void CNN::train(const vector<array_2D_double> &train_x, const vector<vector<double>> &train_y)
{
	cout << "begin to train" << endl;

	if (train_x.size() != train_y.size())
	{
		cout << "train_x size is not same as train_y size!" << endl << "stop CNN trainning!" << endl;
		return;
	}

	int m = train_x.size();// ѵ����������
	int numbatches = ceil(m / _batchsize);// "ѵ�����������һ��" ����Ȩֵ���µĴ���

	// ��ѵ�����������������ѭ��
	for (int I = 0; I < _epochs; I++)
	{
		// ��ʾ����
		cout << "epoch " << I+1 << "/" << _epochs << endl;

		clock_t tic = clock(); //��ȡ���뼶��Ŀ
		
		// ����ѵ������˳��ʵ��ϴ�ƵĹ���
		const vector<int> kk = randperm_vector(m);

		// *********************************************************************************************** //
		// ��"ѵ�����������һ��"����Ȩֵ���µĴ�����ѭ��

		double mse = 0;// ����ѵ�����������ʱ�ľ������

		// ����ѵ��һ��������µĴ���,
		// ��������ѵ��������Ϊ1000������������Ϊ10����������ѵ��һ�ξ͵�1000/10=100��
		for (int L = 0; L < numbatches; L++)
		{
			// ȡ������˳����batchsize�������Ͷ�Ӧ�ı�ǩ
			vector<array_2D_double> batch_train_x;
			vector<vector<double>> batch_train_y;

			for (int i = L*_batchsize; i < min((L + 1)*_batchsize, m); i++)
			{
				batch_train_x.push_back(train_x.at(kk.at(i)));
				batch_train_y.push_back(train_y.at(kk.at(i)));
			}
			// ��ʾ��ǰ���ڴ��������ͼƬ��ֻ��ʾ10�ţ���������ͼƬ������С��10ʱ�ᱨ����ʱ��Ҫ�޸���ʾ��������
			//string window_title = "Images from " + to_string(L*_batchsize) + " to " + to_string(min((L + 1)*_batchsize, m));
			//vector_array_2D_double_show_one_window(window_title, batch_train_x, CvSize(5, 2), CvSize(32, 32), 150);

			// �ڵ�ǰ������Ȩֵ�����������¼�����������(�������)
			feed_forward(batch_train_x);
		
			// �õ���������������ͨ����Ӧ��������ǩ��bp�㷨���õ���������Ȩֵ(���򴫲�) �ĵ�����
			back_propagation(batch_train_y);

			// �õ�����Ȩֵ�ĵ����󣬾�ͨ��Ȩֵ���·���ȥ����Ȩֵ
			update();

			mse += _err;
		}

		mse /= numbatches;
		_ERR.at(I) = mse; // ��¼��I��ѵ�����������ʱ�ľ������

		// *********************************************************************************************** //

		clock_t toc = clock(); //��ȡ���뼶��Ŀ
		cout << "time has elapsed: " << (double)(toc - tic) / 1000 << " seconds" << endl;
	}

	cout << "train has finished!" << endl;
}


// CNN���磬���ԣ����ش�����
double CNN::test(const vector<array_2D_double> &test_x, const vector<vector<double>> &test_y)
{
	cout << "begin to test" << endl;
	return 0;
}


// ��������ṹ����CNN.layers, ��ʼ��һ��CNN����
void CNN::init()
{
	// CNN�������
	int n = _layers.size();

	cout << "input layer " << 1 << " has initialised!" << endl;

	// ��CNN���������ѭ��
	for (int L = 1; L < n; L++)// ע��:����ʵ���Ͽ��Դӵ�2�㿪ʼ������L��ʼֵ��1����0
	{
		// ======================================================================
		// ���´�����Ե�2, 4��(�����)��Ч

		if (_layers.at(L).type == 'c')
		{
			// ��ǰһ��ͼ��������,�ͱ������˳߶�,���㱾��ͼ��������,��ά����
			_layers.at(L).iSizePic[0] = _layers.at(L - 1).iSizePic[0] - _layers.at(L).iSizeKer + 1;
			_layers.at(L).iSizePic[1] = _layers.at(L - 1).iSizePic[1] - _layers.at(L).iSizeKer + 1;

			// "ǰһ������һ��ͨ��", ��Ӧ"��������ͨ��"�����ȨֵW(��ѵ������)����, ����������ƫ��
			int fan_out = _layers.at(L).iChannel * pow(_layers.at(L).iSizeKer, 2);

			// "ǰһ������ͨ��", ��Ӧ"��������һ��ͨ��"�����ȨֵW(��ѵ������)����, ����������ƫ��
			int fan_in = _layers.at(L-1).iChannel * pow(_layers.at(L).iSizeKer, 2);

			// �Ե�ǰ���Ker��Ker_delta��ʼ��ά����������ֵ
			_layers.at(L).Ker.resize(_layers.at(L - 1).iChannel);
			_layers.at(L).Ker_delta.resize(_layers.at(L - 1).iChannel);

			// �Ա�������ͨ������ѭ��
			for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
			{
				_layers.at(L).Ker.at(I).resize(_layers.at(L).iChannel);
				_layers.at(L).Ker_delta.at(I).resize(_layers.at(L).iChannel);

				// �Ա������ͨ������ѭ��
				for (int J = 0; J < _layers.at(L).iChannel; J++)
				{
					double maximum = (double)sqrt(6.0f / (fan_in + fan_out));

					// "ǰһ������ͨ��",��"��������ͨ��",��Բ��ȫ����,�����ȨֵW,���о��ȷֲ���ʼ��,��ΧΪ:[-1,1]*sqrt(6/(fan_in+fan_out))
					_layers.at(L).Ker[I][J] = rand_array_2D_double(_layers.at(L).iSizeKer, _layers.at(L).iSizeKer, -maximum, maximum);
					_layers.at(L).Ker_delta[I][J] = get_zero_array_2D_double_same_size_as(_layers.at(L).Ker[I][J]);
				}
			}

			// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
			_layers.at(L).B.assign(_layers.at(L).iChannel, 0);
			_layers.at(L).B_delta.assign(_layers.at(L).iChannel, 0);

			cout << "convolutional layer " << L + 1 << " has initialised!" << endl;
		}

		// ======================================================================
		// ���´���Ե�3,5��(�²�����)��Ч

		if (_layers.at(L).type == 's')
		{
			_layers.at(L).iSizePic[0] = floor(_layers.at(L - 1).iSizePic[0] / _layers.at(L).iSample);
			_layers.at(L).iSizePic[1] = floor(_layers.at(L - 1).iSizePic[1] / _layers.at(L).iSample);
			_layers.at(L).iChannel = _layers.at(L - 1).iChannel;

			// ���´��������²�����ļ���

			// �Ա������ͨ������ƫ�ý���1ֵ��ʼ��
			_layers.at(L).Beta.assign(_layers.at(L).iChannel, 1);
			_layers.at(L).Beta_delta.assign(_layers.at(L).iChannel, 0);

			// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
			_layers.at(L).B.assign(_layers.at(L).iChannel, 0);
			_layers.at(L).B_delta.assign(_layers.at(L).iChannel, 0);

			cout << "subsampling layer " << L + 1 << " has initialised!" << endl;
		}

		// ======================================================================
		// ������ȫ���Ӳ��ǰ���£����������ǰһ�����²����㣬ǰһ���Ǿ���㣬ǰһ���������

		if (_layers.at(L).type == 'f')
		{
			if (_layers.at(L - 1).type == 's' || _layers.at(L - 1).type == 'c' || _layers.at(L - 1).type == 'i')
			{
				// ------------------------------------------------------------------
				// ���´���Ե�6��(����ȫ���Ӳ�)��Ч

				// ��һ��ÿ��ͨ�������ظ��� * ��һ������ͨ���� = ��ǰ��ȫ�����������
				int fvnum = _layers.at(L - 1).iSizePic[0] * _layers.at(L - 1).iSizePic[1] * _layers.at(L - 1).iChannel;
				// ��ǰ�����������
				int onum = _layers.at(L).iChannel;

				double maximum = (double)sqrt(6.0f / (onum + fvnum));
				// ��ʼ����ǰ������һ�������Ȩֵ
				_layers.at(L).W = rand_array_2D_double(fvnum, onum, -maximum, maximum);// ע����W[I][J],IΪ��һ�����Ŀ��JΪ��ǰ����Ŀ
				_layers.at(L).W_delta = get_zero_array_2D_double_same_size_as(_layers.at(L).W);

				// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
				_layers.at(L).B.assign(onum, 0);
				_layers.at(L).B_delta.assign(onum, 0);
			}
			else if (_layers.at(L - 1).type == 'f')
			{
				// ------------------------------------------------------------------
				// ���´���Ե�7��(ȫ���Ӳ�)��Ч�� �Ե�8��Ҳ��Ч�ɣ�

				// ��һ������ͨ���� = ��ǰ��ȫ�����������
				int fvnum = _layers.at(L - 1).iChannel;
				// ��ǰ�����������
				int onum = _layers.at(L - 1).iChannel;

				double maximum = (double)sqrt(6.0f / (onum + fvnum));
				// ��ʼ����ǰ������һ�������Ȩֵ
				_layers.at(L).W = rand_array_2D_double(fvnum, onum, -maximum, maximum);// ע����W[I][J],IΪ��һ�����Ŀ��JΪ��ǰ����Ŀ
				_layers.at(L).W_delta = get_zero_array_2D_double_same_size_as(_layers.at(L).W);

				// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
				_layers.at(L).B.assign(onum, 0);
				_layers.at(L).B_delta.assign(onum, 0);
			}

			cout << "fully connected layer " << L + 1 << " has initialised!" << endl;
		}
	}
}


// CNN����,�������(�������㷨,������convn�÷�,�����������ӳ��)
void CNN::feed_forward(const vector<array_2D_double> &train_x)
{
	// CNN�������
	int n = _layers.size();

	_layers.at(0).X.resize(1);
	_layers.at(0).X.at(0) = train_x;

	for (int L = 1; L < n; L++)
	{
		// ======================================================================
		// ���´�����Ե�2,4��(�����)��Ч

		if (_layers.at(L).type == 'c')
		{
			// �ر�ע��:
			// ������漰���������� : (1)���, (2)ƫ��(��), (3)sigmoidӳ��

			// �Ե�ǰ����������ʼ��
			_layers.at(L).X.resize(_layers.at(L).iChannel);// ��Ϊ��ǰ��ÿһ��ͨ������һ�����ͼ

			// �Ա������ͨ������ѭ��
			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// �Ե�ǰ���J��ͨ���Ķ���һ������о��֮��z�����г�ʼ��(batchsize������ͬʱ����)
				vector<array_2D_double> z = create_vector_array_2D_double(
												_layers.at(L - 1).X.at(0).size(), 
												_layers.at(L - 1).X.at(0).at(0).size() - _layers.at(L).iSizeKer + 1, 
												_layers.at(L - 1).X.at(0).at(0).at(0).size() - _layers.at(L).iSizeKer + 1, 
												0);

				// 1.���
				for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
				{
					// �ر�ע��:
					// _layers.at(L - 1).X(I)Ϊ_batchsize������, Ϊ��ά����
					// _layers.at(L).Ker[I][J]Ϊ��ά����˾���
					// ��������˺���convn, ʵ�ֶ�����������ͬʱ����
					// convn����ά������˴��ǹؼ�
					z = add_A_B_vector_array_2D_double(z, convolution_n_dim(_layers.at(L - 1).X.at(I), _layers.at(L).Ker.at(I).at(J)));
				}

				// 2.ƫ��(��)
				_layers.at(L).X.at(J) = add_vector_array_2D_double_and_num_double(z, _layers.at(L).B.at(J));
				
				// 3.sigmoidӳ��
				_layers.at(L).X.at(J) = activation_function(_layers.at(L).X.at(J), _activ_func_type);
			}
		}

		// ======================================================================
		// ���´���Ե�3,5��(�²�����)��Ч

		if (_layers.at(L).type == 's')
		{

		}

		// ======================================================================
		// ���´�����Ե�2,4��(�����)��Ч

		if (_layers.at(L).type == 'f')
		{

		}
	}

	//net.y=
}


// CNN����,���򴫲�(�������㷨)
void CNN::back_propagation(const vector<vector<double>> &train_y)
{
	;
}


// CNN����,������������Ȩֵ����(���ӹ�����)
void CNN::update(void)
{
	;
}
