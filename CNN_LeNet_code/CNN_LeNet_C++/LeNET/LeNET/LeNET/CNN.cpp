#include <CNN.h>
#include <maths.h>
#include <time.h>
#include <iostream>


using namespace std;


// ��ʼ��CNN��
CNN::CNN(const vector<Layer> &layers, float alpha, float eta, int batchsize, int epochs, activation_function_type activ_func_type, down_sample_type down_samp_type)
	:_layers(layers), 
	_alpha(alpha), 
	_eta(eta), 
	_batchsize(batchsize), 
	_epochs(epochs), 
	_activation_func_type(activ_func_type), 
	_down_sample_type(down_samp_type)
{
	// ��������ṹ����CNN.layers, ��ʼ��һ��CNN����
	init();

	_ERR.assign(_epochs, 0);// �����ε����ľ�������ʼ��Ϊ0
	_err = 0;// ����ǰ�ֵĵ�ǰ���εľ�������ʼ��Ϊ0

	cout << "CNN has initialised!" << endl;
}


// CNN���磬ѵ��
void CNN::train(const Array3Dd &train_x, const Array2Dd &train_y)
{
	cout << "begin to train" << endl;

	if (train_x.size() != train_y.size())
	{
		cout << "train_x size is not same as train_y size!" << endl << "CNN.train() failed!" << endl;
		return;
	}

	int m = train_x.size();// ѵ����������������1000��
	int numbatches = ceil(m / _batchsize);// "ѵ�����������һ��" ����Ȩֵ���µĴ���������1000/10=100

	// ��ѵ�����������������ѭ��
	for (int I = 0; I < _epochs; I++)// ���������ظ�����25��
	{
		// ��ʾ����
		cout << "epoch " << I+1 << "/" << _epochs << endl;

		clock_t tic = clock(); //��ȡ���뼶��Ŀ
		
		// ����ѵ������˳��ʵ��ϴ�ƵĹ���
		const vector<int> kk = randperm_vector(m);

		// ********************************************************************************************* //
		// ��"ѵ�����������һ��"����Ȩֵ���µĴ�����ѭ��

		double mse = 0;// ����ѵ�����������ʱ�ľ������

		// ����ѵ��һ��������µĴ���,
		// ��������ѵ��������Ϊ1000������������Ϊ10����������ѵ��һ�ξ͵�1000/10=100��
		for (int L = 0; L < numbatches; L++)
		{
			// ȡ������˳����batchsize�������Ͷ�Ӧ�ı�ǩ
			Array3Dd batch_train_x;
			Array2Dd batch_train_y;

			for (int i = L*_batchsize; i < min((L + 1)*_batchsize, m); i++)// (L+1)*_batchsize�����һ��ѭ���п��ܻ����m
			{
				batch_train_x.push_back(train_x.at(kk.at(i)));
				batch_train_y.push_back(train_y.at(kk.at(i)));
			}
			// ��ʾ��ǰ���ڴ��������ͼƬ��ֻ��ʾ10�ţ���������ͼƬ������С��10ʱ�ᱨ����ʱ��Ҫ�޸���ʾ��������
			//string window_title = "Images from " + to_string(L*_batchsize) + " to " + to_string(min((L + 1)*_batchsize, m));
			//batch_train_x.show_specified_images_64FC1(window_title, CvSize(5, 2), CvSize(32, 32), 150);

			// �ڵ�ǰ������Ȩֵ�����������¼�����������(�������)
			clock_t tic_ff = clock();
			feed_forward(batch_train_x);
			clock_t toc_ff = clock();
			cout << "                          batches " << L+1 << " feedforward time: " << (double)(toc_ff - tic_ff) / 1000 << " seconds" << endl;
		
			// �õ���������������ͨ����Ӧ��������ǩ��bp�㷨���õ���������Ȩֵ(���򴫲�) �ĵ���
			clock_t tic_bp = clock();
			back_propagation(batch_train_y);
			clock_t toc_bp = clock();
			cout << "                          batches " << L + 1 << " back propagation time: " << (double)(toc_bp - tic_bp) / 1000 << " seconds" << endl;

			// �õ�����Ȩֵ�ĵ����󣬾�ͨ��Ȩֵ���·���ȥ����Ȩֵ
			clock_t tic_update = clock();
			update();
			clock_t toc_update = clock();
			cout << "                          batches " << L + 1 << " grade update time: " << (double)(toc_update - tic_update) / 1000 << " seconds" << endl;

			mse += _err;
		}

		mse /= numbatches;
		_ERR.at(I) = mse; // ��¼��I��ѵ�����������ʱ�ľ������

		// ********************************************************************************************* //

		clock_t toc = clock(); //��ȡ���뼶��Ŀ
		cout << "epochs " << I+1 << " time has elapsed: " << (double)(toc - tic) / 1000 << " seconds" << endl;
	}
	//*/
	cout << "train has finished!" << endl;
}


// CNN���磬���ԣ����ش�����
double CNN::test(const Array3Dd &test_x, const Array2Dd &test_y)
{
	cout << "begin to test" << endl;

	feed_forward(test_x);

	// �õ�Ԥ��ֵ
	vector<int> h = _Y.max_index();

	vector<int> a = test_y.max_index();

	vector<int> bad = find(h - a);

	double er = (double)bad.size() / (double)test_y.size();

	return er;
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
			int fan_out = _layers.at(L).iChannel * pow(_layers.at(L).iSizeKer, 2);// ���� 4*5^2=4*25

			// "ǰһ������ͨ��", ��Ӧ"��������һ��ͨ��"�����ȨֵW(��ѵ������)����, ����������ƫ��
			int fan_in = _layers.at(L-1).iChannel * pow(_layers.at(L).iSizeKer, 2);

			// �Ե�ǰ���Ker��Ker_delta��ʼ��ά����������ֵ
			_layers.at(L).Ker.resize(_layers.at(L - 1).iChannel);
			_layers.at(L).Ker_delta.resize(_layers.at(L - 1).iChannel);

			_layers.at(L).Ker_grad.resize(_layers.at(L - 1).iChannel);

			// �Ա�������ͨ������ѭ��
			for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
			{
				_layers.at(L).Ker.at(I).resize(_layers.at(L).iChannel);
				_layers.at(L).Ker_delta.at(I).resize(_layers.at(L).iChannel);

				_layers.at(L).Ker_grad.at(I).resize(_layers.at(L).iChannel);// ���������ʼ����С������ʼ��ֵ��ֵ���ڷ��򴫲��и���

				// �Ա������ͨ������ѭ��
				for (int J = 0; J < _layers.at(L).iChannel; J++)
				{
					double maximum = (double)sqrt(6.0f / (fan_in + fan_out));

					// "ǰһ������ͨ��",��"��������ͨ��",��Բ��ȫ����,�����ȨֵW,���о��ȷֲ���ʼ��,��ΧΪ:[-1,1]*sqrt(6/(fan_in+fan_out))
					_layers.at(L).Ker[I][J].set_rand(_layers.at(L).iSizeKer, _layers.at(L).iSizeKer, -maximum, maximum);
					_layers.at(L).Ker_delta[I][J].set_zero_same_size_as(_layers.at(L).Ker[I][J]);
				}
			}

			// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
			_layers.at(L).B.assign(_layers.at(L).iChannel, 0);
			_layers.at(L).B_delta.assign(_layers.at(L).iChannel, 0);

			_layers.at(L).Delta.resize(_layers.at(L).iChannel);// ���ж�ͼ�Ĵ�С��ʼ����������ֵ

			cout << "convolutional layer " << L + 1 << " has initialised!" << endl;
		}

		// ======================================================================
		// ���´���Ե�3,5��(�²�����)��Ч

		if (_layers.at(L).type == 's')
		{
			_layers.at(L).iSizePic[0] = floor((_layers.at(L - 1).iSizePic[0] + _layers.at(L).iSample - 1) / _layers.at(L).iSample);
			_layers.at(L).iSizePic[1] = floor((_layers.at(L - 1).iSizePic[1] + _layers.at(L).iSample - 1) / _layers.at(L).iSample);
			_layers.at(L).iChannel = _layers.at(L - 1).iChannel;

			// ���´��������²�����ļ���

			// �Ա������ͨ������ƫ�ý���1ֵ��ʼ��
			_layers.at(L).Beta.assign(_layers.at(L).iChannel, 1);
			_layers.at(L).Beta_delta.assign(_layers.at(L).iChannel, 0);

			// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
			_layers.at(L).B.assign(_layers.at(L).iChannel, 0);
			_layers.at(L).B_delta.assign(_layers.at(L).iChannel, 0);

			_layers.at(L).Delta.resize(_layers.at(L).iChannel);// ���ж�ͼ�Ĵ�С��ʼ����������ֵ

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

				// ��ǰ��ȫ����������� = ��һ��ÿ��ͨ�������ظ��� * ��һ������ͨ����
				int fvnum = _layers.at(L - 1).iSizePic[0] * _layers.at(L - 1).iSizePic[1] * _layers.at(L - 1).iChannel;
				// ��ǰ�����������
				int onum = _layers.at(L).iChannel;

				double maximum = (double)sqrt(6.0f / (onum + fvnum));
				// ��ʼ����ǰ������һ�������Ȩֵ
				_layers.at(L).W.set_rand(fvnum, onum, -maximum, maximum);// ע����W[I��][J��],IΪ��ǰ��ȫ�������������JΪ��ǰ����Ŀ
				_layers.at(L).W_delta.set_zero_same_size_as(_layers.at(L).W);

				// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
				_layers.at(L).B.assign(onum, 0);
				_layers.at(L).B_delta.assign(onum, 0);
			}
			else if (_layers.at(L - 1).type == 'f')
			{
				// ------------------------------------------------------------------
				// ���´���Ե�7��(ȫ���Ӳ�)��Ч�� �Ե�8��Ҳ��Ч�ɣ�

				// ��ǰ��ȫ����������� = ��һ������ͨ����
				int fvnum = _layers.at(L - 1).iChannel;
				// ��ǰ�����������
				int onum = _layers.at(L).iChannel;

				double maximum = (double)sqrt(6.0f / (onum + fvnum));
				// ��ʼ����ǰ������һ�������Ȩֵ
				_layers.at(L).W.set_rand(fvnum, onum, -maximum, maximum);// ע����W[��I][��J],IΪ��һ�����Ŀ��JΪ��ǰ����Ŀ
				_layers.at(L).W_delta.set_zero_same_size_as(_layers.at(L).W);

				// �Ա������ͨ������ƫ�ý���0ֵ��ʼ��
				_layers.at(L).B.assign(onum, 0);
				_layers.at(L).B_delta.assign(onum, 0);
			}

			cout << "fully connected layer " << L + 1 << " has initialised!" << endl;
		}
	}
}


// CNN����,�������(�������㷨,������convn�÷�,�����������ӳ��)
void CNN::feed_forward(const Array3Dd &train_x)
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
				Array3Dd z;
				bool conv_first_time = true;

				// 1.���
				for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
				{
					// �ر�ע��:
					// _layers.at(L - 1).X(I)Ϊ_batchsize������, Ϊ��ά����
					// _layers.at(L).Ker[I][J]Ϊ��ά����˾���
					// ��������˺���convn, ʵ�ֶ�����������ͬʱ����
					// convn����ά������˴��ǹؼ�
					if (conv_first_time)
					{
						z = convolution(_layers.at(L - 1).X.at(I), _layers.at(L).Ker.at(I).at(J), "valid");
						conv_first_time = false;
					}
					else
					{
						z.add(convolution(_layers.at(L - 1).X.at(I), _layers.at(L).Ker.at(I).at(J), "valid"));
					}
				}

				// 2.ƫ��(��)
				_layers.at(L).X.at(J) = z + _layers.at(L).B.at(J);
				
				// 3.sigmoidӳ��
				_layers.at(L).X.at(J) = activation_function(_layers.at(L).X.at(J), _activation_func_type);
			}
		}

		// ======================================================================
		// ���´���Ե�3,5��(�²�����)��Ч

		if (_layers.at(L).type == 's')
		{
			// �ر�ע��:
			// �²�������漰�������� : (1)�²���, (2)ƫ��(�˺ͼ�)
			// ����û��"sigmoidӳ��"

			// �Ե�ǰ�²�����������������ʼ��
			_layers.at(L).X_down.resize(_layers.at(L).iChannel);// ��Ϊ��ǰ��ÿһ��ͨ������һ������ͼ
			_layers.at(L).X.resize(_layers.at(L).iChannel);// ��Ϊ��ǰ��ÿһ��ͨ������һ�����ͼ

			// �Ա������ͨ������ѭ��(�������ͨ�������)
			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// ͼƬ�²�������, ���в�������ΪiSample
				// ���´��������²�����ļ���

				// (1)�²���
				_layers.at(L).X_down.at(J) = down_sample(_layers.at(L - 1).X.at(J), _layers.at(L).iSample, MeanPooling);

				// (2)ƫ��(�˺ͼ�)
				_layers.at(L).X.at(J) = _layers.at(L).X_down.at(J) * _layers.at(L).Beta.at(J) + _layers.at(L).B.at(J);
			}
		}

		// ======================================================================
		// ���´���Ե�6,7,8�㣨ȫ���Ӳ㣩��Ч

		if (_layers.at(L).type == 'f')
		{
			if ((_layers.at(L - 1).type == 's') || (_layers.at(L - 1).type == 'c') || (_layers.at(L - 1).type == 'i'))
			{
				// ------------------------------------------------------------------
				// ���´���Ե�6��(����ȫ���Ӳ�)��Ч

				// �ر�ע��:
				// ȫ����������漰���������� : (1)��Ȩ, (2)ƫ��(��), (3)sigmoidӳ��

				_layers.at(L - 1).X_vector.clear();

				// ��ǰһ�����ͨ������ѭ��
				// �������ڱ��������һά������ǰһ�������ͨ�����ͼ�ϲ�Ϊһ��һά�������Ա���㣩
				for (int J = 0; J < _layers.at(L - 1).iChannel; J++)
				{
					// ��j������map�Ĵ�С(ʵ����ÿ��j�����)
					int sa_page = _layers.at(L - 1).X.at(J).size();
					int sa_col = _layers.at(L - 1).X.at(J).at(0).size();
					int sa_row = _layers.at(L - 1).X.at(J).at(0).at(0).size();

					// �����е�����map����һ��������������һά���Ƕ�Ӧ������������ÿ������һ�У�ÿ��Ϊ��Ӧ�������������˴��ǳ����
					// �Ὣÿ��ͼ��İ�������������ʽ��ȡΪ1�У�Ȼ���ٽ���ͬ��������������������
					_layers.at(L - 1).X_vector.append_along_row( _layers.at(L - 1).X.at(J).reshape_to_Array2D() );
				}

				// ����������������ֵ��sigmoid(W*X + b)��ע����ͬʱ������batchsize�����������ֵ

				int col_batchsize = _layers.at(L - 1).X_vector.size();
				// in = W*X + B (1)��Ȩ, (2)ƫ��(��)
				Array2Dd fcl_map_in = _layers.at(L).W.product(_layers.at(L - 1).X_vector) + Array2Dd::repmat(_layers.at(L).B, 1, col_batchsize);
				// out = activ(in) (3)sigmoidӳ��
				_layers.at(L).X_vector = activation_function(fcl_map_in, _activation_func_type);

				// �ر�ע��:
				// ȫ����������漰���������� : (1)��Ȩ, (2)ƫ��(��), (3)sigmoidӳ��
			}
			else if (_layers.at(L - 1).type == 'f')
			{
				// ------------------------------------------------------------------
				// ���´���Ե�7,8��(ȫ���Ӳ�)��Ч

				// �ر�ע��:
				// ȫ����������漰���������� : (1)��Ȩ, (2)ƫ��(��), (3)sigmoidӳ��

				// ����������������ֵ��sigmoid(W*X + b)��ע����ͬʱ������batchsize�����������ֵ

				int col_batchsize = _layers.at(L - 1).X_vector.size();
				// in = W*X + B (1)��Ȩ, (2)ƫ��(��)
				Array2Dd fcl_map_in = _layers.at(L).W.product(_layers.at(L - 1).X_vector) + Array2Dd::repmat(_layers.at(L).B, 1, col_batchsize);
				// out = activ(in) (3)sigmoidӳ��
				_layers.at(L).X_vector = activation_function(fcl_map_in, _activation_func_type);

				// �ر�ע��:
				// ȫ����������漰���������� : (1)��Ȩ, (2)ƫ��(��), (3)sigmoidӳ��
			}
		}
	}

	// �����һ�㣨ȫ���Ӳ㣩��������ι��_Y����Ϊ����������
	_Y = _layers.at(n-1).X_vector;
}


// CNN����,���򴫲�(�������㷨)
void CNN::back_propagation(const Array2Dd &train_y)
{
	// CNN�������
	int n = _layers.size();

	// ������: Ԥ��ֵ-����ֵ
	Array2Dd E = _layers.at(n - 1).X_vector - train_y;

	// �����������(�в�)
	// ע�⣬������Ҫ˵���£������Ӧ�Ĺ�ʽ�� delta = (y - t).*f'(u),��������Ϊʲô��f'(x)�أ�
	// ��Ϊ������ʵ��sigmoid�󵼣�f'(u) = x*(1-x)����������ľ���x�ˡ�
	// ���У�u��ʾ��ǰ�����룬x��ʾ��ǰ�������
	_layers.at(n - 1).Delta_vector = E * derivation(_layers.at(n - 1).X_vector, _activation_func_type);

	// ���ۺ����Ǿ������,�Ѷ���������ƽ��
	_err = 0.5 * E.pow(2).sum() / E.size();// ��ǰ�ֵĵ�ǰ���εľ������

	// ************** ������(�в�)�ķ��򴫲� ******************************

	int tmp;
	if (_layers.at(1).type == 'f')
	{
		// ���ڶ������ȫ���Ӳ�ʱ,�൱������ͼƬ����һ������ʸ���γɵ�BP����,
		// ���ǵ��������net.layers{1}.X_vector,����L�����ޱ��뵽0
		tmp = 0;
	}
	else
	{
		// �������L������1�Ϳ���
		tmp = 1;
	}

	for (int L = (n - 2); L >= tmp; L--)
	{
		// =====================================================================
		// ���´���ԡ���һ�㡱Ϊ��ȫ���Ӳ㡱ʱ��Ч

		if (_layers.at(L + 1).type == 'f')
		{
			if (_layers.at(L).type == 'f')
			// ------------------------------------------------------------------
			// ���´���Ե�6(����ȫ���Ӳ�),7��(ȫ���Ӳ�)��Ч
			{
				// ���͵�BP���������������������(�в�)�ķ��򴫲���ʽ
				// delta_{L} = W_{L+1}^T * delta_{L+1} .* f'(X_{L})
				_layers.at(L).Delta_vector = _layers.at(L + 1).W.transpose().product(_layers.at(L + 1).Delta_vector) * derivation(_layers.at(L).X_vector, _activation_func_type);
				// ��Ϊ�ο�����L=6�������ڶ��㣩ʱ����ʽ��ά����������ʾ��
				// _layers.at(L).Delta = [84, 10] [�� ��]
				// _layers.at(L + 1).W = [10, 84]
				// _layers.at(L + 1).W.transpose() = [84 10]
				// _layers.at(L + 1).Delta = [10 10]
				// _layers.at(L).X_vector = [84 10]
			}
			else if (_layers.at(L).type == 's' || _layers.at(L).type == 'c' || _layers.at(L).type == 'i')
			// ------------------------------------------------------------------
			// ���´���Ե�5��(��������)��Ч���䡰��һ�㡱Ϊ����ȫ���Ӳ�
			{
				// ÿ�����ͨ��ͼ��ߴ�(��άʸ��,  ����ά�����������������������ά�ǳߴ�)
				int SizePic_col = _layers.at(L).X.at(0).at(0).size();
				int SizePic_row = _layers.at(L).X.at(0).at(0).at(0).size();

				// ���ͼ�����ظ���
				int fvnum = SizePic_col * SizePic_row;

				// ���͵�BP���������������������(�в�)�ķ��򴫲���ʽ

				// ����ǰ���ǽ������㣬�������
				_layers.at(L).Delta_vector = _layers.at(L + 1).W.transpose().product(_layers.at(L + 1).Delta_vector);
				// ��Ϊ�ο�����L=4���ڶ����������㣬��һ��Ϊȫ���Ӳ㣩ʱ����ʽ��ά����������ʾ��
				// _layers.at(L).Delta_vec = [100 10]
				// _layers.at(L + 1).W = [120 100]
				// _layers.at(L + 1).W.transpose() = [100 120]
				// _layers.at(L + 1).Delta_vec = [120 10]

				// ����ǰ���Ǿ����
				if (_layers.at(L).type == 'c')
				{
					// ���ھ������ڼ����������Ҫ��˵�ǰ�㼤����ĵ��������ǵ�ǰ���������
					_layers.at(L).Delta_vector.dot_product(derivation(_layers.at(L).X_vector, _activation_func_type));
				}

				// �˴�Ҳ���������
				// �������ʸ��������(�в�), ÿһ��Ϊһ������, reshape��ͨ����ʾ(ʸ����ȫ����->ͨ����ȫ����)
				_layers.at(L).Delta = Array3Dd::reshape_from_Array2D(_layers.at(L).Delta_vector, _layers.at(L).iChannel, SizePic_col, SizePic_row);
			}
		}

		// =====================================================================
		// ���´���ԡ���һ�㡱Ϊ���������㡱ʱ��Ч
		// �ο����ϣ�http://www.cnblogs.com/tornadomeet/p/3468450.html

		if (_layers.at(L + 1).type == 's')
		{
			// һ�㡰����㡱����һ���ǡ��������㡱

			// �Ա������ͨ������ѭ��
			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// ���㵼��
				Array3Dd deriv_J = derivation(_layers.at(L).X.at(J), _activation_func_type);

				// ��һ����ϲ��������Ƶ�ԭ���ĳߴ�
				Array3Dd next_layer_delta_J = up_sample(_layers.at(L + 1).Delta.at(J), _layers.at(L + 1).iSample, _down_sample_type);

				// ���棺�²�������ϲ����꣬���ߵĳߴ粻һ�£����統�ߴ�Ϊ����5ʱ������ΪLeNet5�Ĵ˴���СΪż�������Դ�������ʱ������

				// ���´��������²�����ļ���
				// �������_layers.at(L + 1).Beta.at(J)������W��������Ϊ��һ����²������Delta����û�г��ϳ���ƫ��Beta��Ϊʲô����Delta�Ķ��壩
				_layers.at(L).Delta.at(J) = deriv_J * next_layer_delta_J * ((1.0 / (double)pow(_layers.at(L + 1).iSample, 2)) * _layers.at(L + 1).Beta.at(J));
			}
		}

		// =====================================================================
		// ���´���ԡ���һ�㡱Ϊ������㡱ʱ��Ч������Ĭ���ǽ�������㣨����㲻�㣩
		// �ο����ϣ�http://www.cnblogs.com/tornadomeet/p/3468450.html

		if (_layers.at(L + 1).type == 'c')
		{
			// �Ա������ͨ������ѭ��
			for (int I = 0; I < _layers.at(L).iChannel; I++)
			{
				Array3Dd z = _layers.at(L).X.at(0);
				z.clear();

				// ����һ�����ͨ������ѭ��
				for (int J = 0; J < _layers.at(L + 1).iChannel; J++)
				{
					// ��ǰ��������(�в�)net.layers{ L }.Delta{ J }����
					z = z + convolution(_layers.at(L + 1).Delta.at(J), _layers.at(L + 1).Ker.at(I).at(J).flip_xy(), "full");
				}

				// ��Ϊ����Ĭ���ǽ������㣬���Բ����ڼ����������f'(u)=1������ʡ�Գ�f'(u)
				_layers.at(L).Delta.at(I) = z;
			}
		}

		// =====================================================================
	}

	// ****************** ��ѵ���������ݶ� **************************************

	// �����롶Notes on Convolutional Neural Networks���в�ͬ��
	// ����ġ��Ӳ�������û�в�����Ҳû�м������
	// �������Ӳ�������û����Ҫ���Ĳ�����

	// ��CNN���������ѭ��(ע��:����ʵ���Ͽ��Դӵ�2�㿪ʼ)
	for (int L = 1; L < n; L++)
	{
		// =====================================================================
		// ���´������ڵ�2,4��(�����)�ļ���

		if (_layers.at(L).type == 'c')
		{
			_layers.at(L).B_grad.resize(_layers.at(L).iChannel);

			// �Ա������ͨ������ѭ��
			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// ����һ�����ͨ������ѭ��
				for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
				{
					// �ر�ע��:
					// (1)�ȼ۹�ϵ rot180(conv2(a, rot180(b), 'valid')) = conv2(rot180(a), b, 'valid')
					// (2)��ndims(a) = ndims(b) = 3, ��convn(filpall(a), b, 'valid')��ʾ����ά����ͬʱ�������
					// (3)��size(a, 3) = size(b, 3), ����ʽ�������άΪ1, ��ʾ����ѵ�������ĵ��Ӻ�(�������㷨), ���Ҫ����������ƽ��

					_layers.at(L).Ker_grad.at(I).at(J) = convolution(_layers.at(L - 1).X.at(I).flip_xy(), _layers.at(L).Delta.at(J), "valid") * (1.0 / (double)_layers.at(L).Delta.at(J).size());
				}

				// ������net.layers{ L }.Delta{ J }�ĵ���, ���Ҫ����������ƽ��
				_layers.at(L).B_grad.at(J) =_layers.at(L).Delta.at(J).sum() / (double)_layers.at(L).Delta.at(J).size();
			}
		}

		// =====================================================================
		// ���´������ڵ�3,5��(�²�����)�ļ���

		if (_layers.at(L).type == 's')
		{
			_layers.at(L).Beta_grad.resize(_layers.at(L).iChannel);
			_layers.at(L).B_grad.resize(_layers.at(L).iChannel);

			int batch_num = _layers.at(L).Delta.at(0).size();

			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// ������²������Beta���ݶ���⣬������W��Ȼ����ƽ��
				// Ϊʲô��ȫ������أ���ΪBeta��BӰ����ȫ����Ԫ�ذ�������ȻҪ�����
				_layers.at(L).Beta_grad.at(J) = sum_vector(_layers.at(L).Delta.at(J).reshape_to_vector() * (_layers.at(L).X_down.at(J).reshape_to_vector())) * (1.0 / (double)_layers.at(L).Delta.at(J).size());

				// ������net.layers{L}.Delta{J}�ĵ���,���Ҫ����������ƽ��
				_layers.at(L).B_grad.at(J) = sum_vector(_layers.at(L).Delta.at(J).reshape_to_vector()) * (1.0 / (double)_layers.at(L).Delta.at(J).size());
			}
		}

		// =====================================================================
		// ���´������ڵ�6,7,8��(ȫ���Ӳ�)�ļ���

		if (_layers.at(L).type == 'f')
		{
			// Ȩֵ�����ݶ�, ���Ҫ����������ƽ��������������batch�����ã���һ��batch��ƽ��
			_layers.at(L).W_grad = _layers.at(L).Delta_vector.product(_layers.at(L - 1).X_vector.transpose()) * (1.0 / _layers.at(L).Delta_vector.size());

			// �����������(�в�)����ƫ��(����)���ݶ�, ����ҲҪ����������ƽ��������������batch�����ã���һ��batch��ƽ��
			_layers.at(L).B_grad = _layers.at(L).Delta_vector.mean();
		}

		// =====================================================================
	}
}


// CNN����,������������Ȩֵ����(���ӹ�����)
void CNN::update(void)
{
	// CNN�������
	int n = _layers.size();

	// ��Ȩֵ��ƫ�õĸ��²��ö����������ݶ��½����ĸĽ�����ֹ�����𵴣����ο����ף�https://blog.csdn.net/qq_37053885/article/details/81605365
	
	for (int L = 1; L < n; L++)
	{
		// ======================================================================
		// ���´������ھ����ļ���
		if (_layers.at(L).type == 'c')
		{
			// �Ա������ͨ������ѭ��
			for (int J = 0; J < _layers.at(L).iChannel; J++)
			{
				// ����һ�����ͨ������ѭ��
				for (int I = 0; I < _layers.at(L - 1).iChannel; I++)
				{
					// ����ûʲô��˵�ģ�������ͨ��Ȩֵ���µĹ�ʽ��W_new = W_old - alpha * de / dW������Ȩֵ������
					// net.layers{ L }.Ker_delta{ I }{J} = net.eta * net.layers{ L }.Ker_delta{ I }{J} -net.alpha * net.layers{ L }.Ker_grad{ I }{J};
					// net.layers{ L }.Ker{ I }{J} = net.layers{ L }.Ker{ I }{J} +net.layers{ L }.Ker_delta{ I }{J};
					_layers.at(L).Ker_delta.at(I).at(J) = _layers.at(L).Ker_delta.at(I).at(J) * _eta - _layers.at(L).Ker_grad.at(I).at(J) * _alpha;
					_layers.at(L).Ker.at(I).at(J) = _layers.at(L).Ker.at(I).at(J) + _layers.at(L).Ker_delta.at(I).at(J);
				}
			}

			// ����һ��ͨ�������Ӧһ������ƫ��net.layers{ L }.B{ J }
			_layers.at(L).B_delta = _layers.at(L).B_delta * _eta - _layers.at(L).B_grad * _alpha;
			_layers.at(L).B = _layers.at(L).B + _layers.at(L).B_delta;
		}

		// ======================================================================
		// ���´��������²�����ļ���
		if (_layers.at(L).type == 's')
		{
			// ����һ��ͨ�������Ӧһ������ƫ��net.layers{ L }.B{ J }
			_layers.at(L).B_delta = _layers.at(L).B_delta * _eta - _layers.at(L).B_grad * _alpha;
			_layers.at(L).B = _layers.at(L).B + _layers.at(L).B_delta;

			// ����һ��ͨ�������Ӧһ������ƫ��net.layers{ L }.Beta{ J }
			_layers.at(L).Beta_delta = _layers.at(L).Beta_delta * _eta - _layers.at(L).Beta_grad * _alpha;
			_layers.at(L).Beta = _layers.at(L).Beta + _layers.at(L).Beta_delta;
		}

		// ======================================================================
		// ���´�������ȫ���Ӳ�ļ���
		if (_layers.at(L).type == 'f')
		{
			// ����һ��ͨ�������Ӧһ����Ȩϵ��net.layers{ L }.W
			_layers.at(L).W_delta = _layers.at(L).W_delta * _eta -  _layers.at(L).W_grad * _alpha;
			_layers.at(L).W = _layers.at(L).W + _layers.at(L).W_delta;

			// ����һ��ͨ�������Ӧһ������ƫ��net.layers{ L }.B{ J }
			_layers.at(L).B_delta = _layers.at(L).B_delta * _eta - _layers.at(L).B_grad * _alpha;
			_layers.at(L).B = _layers.at(L).B + _layers.at(L).B_delta;
		}
	}
}
