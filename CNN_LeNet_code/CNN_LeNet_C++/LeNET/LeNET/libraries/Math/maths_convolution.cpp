// �������
#include <maths_convolution.h>


Array3Dd convolution_n_dim(const Array3Dd &X, const Array2Dd &Ker)
{
	Array3Dd convn;

	for (int i = 0; i < X.size(); i++)
	{
		Array2Dd conv = convolution_one_dim(X.at(i), Ker);
		convn.push_back(conv);
	}

	return convn;
}


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


// ���Ҫ����Ķ���ĳߴ�С�ھ���˵ĳߴ磬�򽫾������ĳߴ���չ�����Ʊ�Ե��������͵ĳߴ硣
void change_X_size_to_fit_Ker(Array2Dd &X, const Array2Dd &Ker)
{
	int X_row = X.at(0).size();
	int X_col = X.size();

	int Ker_row = Ker.at(0).size();
	int Ker_col = Ker.size();

	if (X_row < Ker_row)
	{
		int append_row_size = Ker_row - X_row;
		for (int i = 0; i < X_col; i++)
		{
			for (int j = 0; j < append_row_size; j++)
			{
				X.at(i).push_back(X.at(i).at(X_row - 1));
			}
		}
	}

	if (X_col < Ker_col)
	{
		int append_col_size = Ker_col - X_col;
		for (int i = 0; i < append_col_size; i++)
		{
			X.push_back(X.at(X_col - 1));
		}
	}
}
