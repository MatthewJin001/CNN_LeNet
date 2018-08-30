// �������
#include <maths_convolution.h>

/*
Array3Dd convolution_n_dim(const Array3Dd &X, const Array2Dd &Ker)
{
	Array3Dd convn;

	for (int i = 0; i < X.size(); i++)
	{
		Array2Dd conv = convolution_one_dim_fast(X.at(i), Ker);
		convn.push_back(conv);
	}

	return convn;
}
*/

// ������������������������vector���ٶ�Ҫ��10%��
Array3Dd convolution_n_dim(const Array3Dd &X, const Array2Dd &Ker)
{
	int X_page = X.size();
	int X_row = X.at(0).at(0).size();
	int X_col = X.at(0).size();

	int Ker_row = Ker.at(0).size();
	int Ker_col = Ker.size();

	int i, j, k;

	// ���Ҫ����Ķ���ĳߴ�С�ھ���˵ĳߴ磬�򽫾������ĳߴ���չ�����Ʊ�Ե��������͵ĳߴ硣
	if (X_row < Ker_row || X_col < Ker_col)
	{
		cout << "X size is smaller than Ker size!" << endl << "convolution_n_dim() failed!" << endl;
		Array3Dd temp;
		return temp;
	}

	// �����������������conv����ʼ��Ϊ0
	int conv_row = X.at(0).at(0).size() - Ker.at(0).size() + 1;
	int conv_col = X.at(0).size() - Ker.size() + 1;
	Array3Dd convn(X_page, conv_col, conv_row, 0);

	double *arr_X = new double[X_page * X_row * X_col]();
	double *arr_Ker = new double[Ker_row * Ker_col]();

	for (i = 0; i < X_page; i++)
	{
		for (j = 0; j < X_row; j++)
		{
			for (k = 0; k < X_col; k++)
			{
				// ��arr_X��ֵ
				arr_X[i * (X_row * X_col) + j * X_col + k] = X.at(i).at(k).at(j);

				// ��arr_Ker��ֵ
				if((i == 0) && (j < Ker_row) && (k < Ker_col))
				{
					// x,y��ͬʱ��ת
					arr_Ker[j * Ker_col + k] = Ker.at(Ker_col - 1 - k).at(Ker_row - 1 - j);
				}
			}
		}
	}

	int row, col;
	for (i = 0; i < X_page; i++)
	{
		for (j = 0; j < conv_row; j++)
		{
			for (k = 0; k < conv_col; k++)
			{
				// �����������(j,k)���ֵ
				double sum_ijk = 0;
				for (row = j; row < j + Ker_row; row++)
				{
					for (col = k; col < k + Ker_col; col++)
					{
						sum_ijk += arr_X[i * (X_row * X_col) + row * X_col + col] * arr_Ker[(row - j) * Ker_col + (col - k)];
					}
				}
				convn.at(i).at(k).at(j) = sum_ijk;
			}
		}
	}

	delete[] arr_X;
	delete[] arr_Ker;

	return convn;
}


/*
Array2Dd convolution_one_dim(Array2Dd X, Array2Dd Ker)
{
	int X_row = X.at(0).size();
	int X_col = X.size();

	int Ker_row = Ker.at(0).size();
	int Ker_col = Ker.size();

	// ���Ҫ����Ķ���ĳߴ�С�ھ���˵ĳߴ磬�򽫾������ĳߴ���չ�����Ʊ�Ե��������͵ĳߴ硣
	if (X_row < Ker_row || X_col < Ker_col)
	{
		change_X_size_to_fit_Ker(X, Ker);
	}

	// ���»�ȡX�ĳߴ�
	X_row = X.at(0).size();
	X_col = X.size();

	// �����������������conv����ʼ��Ϊ0
	int conv_row = X.at(0).size() - Ker.at(0).size() + 1;
	int conv_col = X.size() - Ker.size() + 1;
	Array2Dd conv(conv_col, conv_row, 0);

	// �Ծ���˽���xy����ķ�ת
	Ker.flip_xy();

	// ���������;������˵���һ��X������
	Array2Dd X_patch;

	// ��ʼ���о��
	for (int i = 0; i < conv_col; i++)
	{
		for (int j = 0; j < conv_row; j++)
		{
			X_patch.get_specific_patch(X, Ker_col, Ker_row, i, j);
			// conv[i][j] = sum(X_patch .* Ker);
			conv.at(i).at(j) = (X_patch * Ker).sum();
		}
	}

	return conv;
}
*/


// ������������������������vector���ٶ�Ҫ��30����
Array2Dd convolution_one_dim(Array2Dd X, Array2Dd Ker)
{
	int X_row = X.at(0).size();
	int X_col = X.size();

	int Ker_row = Ker.at(0).size();
	int Ker_col = Ker.size();

	// ���Ҫ����Ķ���ĳߴ�С�ھ���˵ĳߴ磬�򽫾������ĳߴ���չ�����Ʊ�Ե��������͵ĳߴ硣
	if (X_row < Ker_row || X_col < Ker_col)
	{
		cout << "X size is smaller than Ker size!" << endl << "convolution_n_dim() failed!" << endl;
		Array2Dd temp;
		return temp;
	}

	// �����������������conv����ʼ��Ϊ0
	int conv_row = X.at(0).size() - Ker.at(0).size() + 1;
	int conv_col = X.size() - Ker.size() + 1;
	Array2Dd conv(conv_col, conv_row, 0);

	double *arr_X = new double[X_row * X_col] ();
	double *arr_Ker = new double[Ker_row * Ker_col] ();

	int i, j;

	for (i = 0; i < X_row; i++)
	{
		for (j = 0; j < X_col; j++)
		{
			// ��arr_X��ֵ
			arr_X[i * X_col + j] = X.at(j).at(i);

			// ��arr_Ker��ֵ
			if ((i < Ker_row) && (j < Ker_col))
			{
				// x,y��ͬʱ��ת
				arr_Ker[i * Ker_col + j] = Ker.at(Ker_col -1 - j).at(Ker_row - 1 - i);
			}
		}
	}

	int row, col;
	for (i = 0; i < conv_row; i++)
	{
		for (j = 0; j < conv_col; j++)
		{
			// �����������(i,j)���ֵ
			double sum_ij = 0;
			for (row = i; row < i + Ker_row; row++)
			{
				for (col = j; col < j + Ker_col; col++)
				{
					sum_ij += arr_X[row * X_col + col] * arr_Ker[(row - i) * Ker_col + (col - j)];
				}
			}
			conv.at(j).at(i) = sum_ij;
		}
	}

	delete[] arr_X;
	delete[] arr_Ker;

	return conv;
}

