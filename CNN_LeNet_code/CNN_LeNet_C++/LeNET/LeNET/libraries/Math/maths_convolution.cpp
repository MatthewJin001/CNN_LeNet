// �������
#include <maths_convolution.h>

vector<array_2D_double> convolution_n_dim(const vector<array_2D_double> &X, const array_2D_double &Ker)
{
	vector<array_2D_double> convn;

	for (int i = 0; i < X.size(); i++)
	{
		array_2D_double conv = convolution_one_dim(X.at(i), Ker);
		convn.push_back(conv);
	}

	return convn;
}


array_2D_double convolution_one_dim(array_2D_double X, array_2D_double Ker)
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
	array_2D_double conv = zero_array_2D_double(conv_col, conv_row);

	// �Ծ���˽���xy����ķ�ת
	flip_xy_array_2D_double(Ker);

	// ���������;������˵���һ��X������
	array_2D_double X_patch = get_zero_array_2D_double_same_size_as(Ker);

	// ��ʼ���о��
	for (int i = 0; i < conv_col; i++)
	{
		for (int j = 0; j < conv_row; j++)
		{
			X_patch = get_specific_size_array_2D_double_from_specific_position(X, Ker_col, Ker_row, i, j);
			// conv[i][j] = sum(X_patch .* Ker);
			conv.at(i).at(j) = sum_of_array_2D_double(get_A_dot_product_B_array_2D_double(X_patch, Ker));
		}
	}

	return conv;
}


// ���Ҫ����Ķ���ĳߴ�С�ھ���˵ĳߴ磬�򽫾������ĳߴ���չ�����Ʊ�Ե��������͵ĳߴ硣
void change_X_size_to_fit_Ker(array_2D_double &X, const array_2D_double &Ker)
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
