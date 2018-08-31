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
			cout << "                          batches " << L+1 << " feedforward time has elapsed: " << (double)(toc_ff - tic_ff) / 1000 << " seconds" << endl;
		
			// �õ���������������ͨ����Ӧ��������ǩ��bp�㷨���õ���������Ȩֵ(���򴫲�) �ĵ���
			clock_t tic_bp = clock();
			back_propagation(batch_train_y);
			clock_t toc_bp = clock();
			cout << "                          batches " << L + 1 << " back propagation time has elapsed: " << (double)(toc_bp - tic_bp) / 1000 << " seconds" << endl;

			// �õ�����Ȩֵ�ĵ����󣬾�ͨ��Ȩֵ���·���ȥ����Ȩֵ
			update();

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
			int fan_out = _layers.at(L).iChannel * pow(_layers.at(L).iSizeKer, 2);// ���� 4*5^2=4*25

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
					_layers.at(L).Ker[I][J].set_rand(_layers.at(L).iSizeKer, _layers.at(L).iSizeKer, -maximum, maximum);
					_layers.at(L).Ker_delta[I][J].set_zero_same_size_as(_layers.at(L).Ker[I][J]);
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
						z = convolution_n_dim(_layers.at(L - 1).X.at(I), _layers.at(L).Ker.at(I).at(J));
						conv_first_time = false;
					}
					else
					{
						z.add(convolution_n_dim(_layers.at(L - 1).X.at(I), _layers.at(L).Ker.at(I).at(J)));
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

				_layers.at(L - 1).X_Array.clear();

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
					_layers.at(L - 1).X_Array.append_along_row( _layers.at(L - 1).X.at(J).reshape_to_Array2D() );
				}

				// ����������������ֵ��sigmoid(W*X + b)��ע����ͬʱ������batchsize�����������ֵ

				int col_batchsize = _layers.at(L - 1).X_Array.size();
				// in = W*X + B (1)��Ȩ, (2)ƫ��(��)
				Array2Dd fcl_map_in = _layers.at(L).W.product(_layers.at(L - 1).X_Array) + Array2Dd::repmat(_layers.at(L).B, 1, col_batchsize);
				// out = activ(in) (3)sigmoidӳ��
				_layers.at(L).X_fcl = activation_function(fcl_map_in, _activation_func_type);

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

				int col_batchsize = _layers.at(L - 1).X_fcl.size();
				// in = W*X + B (1)��Ȩ, (2)ƫ��(��)
				Array2Dd fcl_map_in = _layers.at(L).W.product(_layers.at(L - 1).X_fcl) + Array2Dd::repmat(_layers.at(L).B, 1, col_batchsize);
				// out = activ(in) (3)sigmoidӳ��
				_layers.at(L).X_fcl = activation_function(fcl_map_in, _activation_func_type);

				// �ر�ע��:
				// ȫ����������漰���������� : (1)��Ȩ, (2)ƫ��(��), (3)sigmoidӳ��
			}
		}
	}

	// �����һ�㣨ȫ���Ӳ㣩��������ι��_Y����Ϊ����������
	_Y = _layers.at(n-1).X_fcl;
}


// CNN����,���򴫲�(�������㷨)
void CNN::back_propagation(const Array2Dd &train_y)
{
	// CNN�������
	int n = _layers.size();

	// ������: Ԥ��ֵ-����ֵ
	Array2Dd E = _layers.at(n - 1).X_fcl - train_y;

	// �����������(�в�)
	// ע�⣬������Ҫ˵���£������Ӧ�Ĺ�ʽ�� delta = (y - t).*f'(u),��������Ϊʲô��f'(x)�أ�
	// ��Ϊ������ʵ��sigmoid�󵼣�f'(u) = x*(1-x)����������ľ���x�ˡ�
	// ���У�u��ʾ��ǰ�����룬x��ʾ��ǰ�������
	_layers.at(n - 1).Delta_fcl = E * derivation(_layers.at(n - 1).X_fcl, _activation_func_type);

	// ���ۺ����Ǿ������,�Ѷ���������ƽ��
	_err = 0.5 * E.pow(2).sum() / E.size();// ��ǰ�ֵĵ�ǰ���εľ������

	// ************** ������(�в�)�ķ��򴫲� ******************************

	int tmp;
	if (_layers.at(1).type == 'f')
	{
		// ���ڶ������ȫ���Ӳ�ʱ,�൱������ͼƬ����һ������ʸ���γɵ�BP����,
		// ���ǵ��������net.layers{1}.X_Array,����L�����ޱ��뵽0
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
				_layers.at(L).Delta_fcl = _layers.at(L + 1).W.transpose().product(_layers.at(L + 1).Delta_fcl) * derivation(_layers.at(L).X_fcl, _activation_func_type);
				// ��Ϊ�ο�����L=6�������ڶ��㣩ʱ����ʽ��ά����������ʾ��
				// _layers.at(L).Delta = [84, 10] [�� ��]
				// _layers.at(L + 1).W = [10, 84]
				// _layers.at(L + 1).W.transpose() = [84 10]
				// _layers.at(L + 1).Delta = [10 10]
				// _layers.at(L).X_fcl = [84 10]
			}
			else if (_layers.at(L).type == 's' || _layers.at(L).type == 'c' || _layers.at(L).type == 'i')
			// ------------------------------------------------------------------
			// ���´���Ե�5��(��������)��Ч���䡰��һ�㡱Ϊ����ȫ���Ӳ�
			{
				// ÿ�����ͨ��ͼ��ߴ�(��άʸ��,  ����ά�����������������������ά�ǳߴ�)
				int SizePic_0 = _layers.at(L).X.at(0).at(0).size();
				int SizePic_1 = _layers.at(L).X.at(0).at(0).at(0).size();

				// ���ͼ�����ظ���
				int fvnum = SizePic_0 * SizePic_1;                                  

				// ���͵�BP���������������������(�в�)�ķ��򴫲���ʽ

				// ����ǰ���ǽ������㣬�������
				int col1 = _layers.at(L + 1).W.size();
				int row1 = _layers.at(L + 1).W.at(0).size();

				int col2 = _layers.at(L + 1).Delta_fcl.size();
				int row2 = _layers.at(L + 1).Delta_fcl.at(0).size();

				_layers.at(L).Delta_Array = _layers.at(L + 1).W.transpose().product(_layers.at(L + 1).Delta_fcl);
				// ��Ϊ�ο�����L=4���ڶ����������㣬��һ��Ϊȫ���Ӳ㣩ʱ����ʽ��ά����������ʾ��
				// _layers.at(L).Delta_Array = [100 10]
				// _layers.at(L + 1).W = [120 100]
				// _layers.at(L + 1).W.transpose() = [100 120]
				// _layers.at(L + 1).Delta = [120 10]

				// ����ǰ���Ǿ����
				if (_layers.at(L).type == 'c')
				{
					// ���ھ������ڼ����������Ҫ��˵�ǰ�㼤����ĵ��������ǵ�ǰ���������
					_layers.at(L).Delta_Array.dot_product(derivation(_layers.at(L).X_Array, _activation_func_type));
				}

				for (int J = 0; J < _layers.at(L).iChannel; J++)
				{
					// �˴�Ҳ���������
					// ������ĳ�ʸ��������(�в�), ÿһ��Ϊһ������, reshape��ͨ����ʾ(ʸ����ȫ����->ͨ����ȫ����)
				}
			}
		}

		// =====================================================================
		// ���´���ԡ���һ�㡱Ϊ���²����㡱ʱ��Ч

		if (_layers.at(L + 1).type == 's')
		{

		}

		// =====================================================================
		// ���´���ԡ���һ�㡱Ϊ������㡱ʱ��Ч

		if (_layers.at(L + 1).type == 'c')
		{

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

		}

		// =====================================================================
		// ���´������ڵ�3,5��(�²�����)�ļ���

		if (_layers.at(L).type == 's')
		{

		}

		// =====================================================================
		// ���´������ڵ�6,7,8��(ȫ���Ӳ�)�ļ���

		if (_layers.at(L).type == 'f')
		{

		}

		// =====================================================================
	}
}


// CNN����,������������Ȩֵ����(���ӹ�����)
void CNN::update(void)
{
	;
}
