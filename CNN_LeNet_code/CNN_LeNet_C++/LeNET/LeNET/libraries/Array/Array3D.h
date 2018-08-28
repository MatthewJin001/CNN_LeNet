#pragma once
#include <Array2D.h>

template <typename T>
class Array3D
{
public:
	// ����
	Array3D() { }

	Array3D(int page, int col, int row, T value);

	// *************************** ���� ***************************************** //

	void from_vector_image_64FC1(const vector<Mat> &vector_img);

	// *************************** ��ֵ ***************************************** //

	Array2D<T>& at(int page);// д

	const Array2D<T>& at(int page) const;// ��

	void push_back(const Array2D<T> &val);


	// *************************** ���� ***************************************** //

	void set_zero();

	void set_value(T val);

	void set_zero_same_size_as(const Array3D<T> &array2D);

	// ��һ��Ϊ0~1
	void normalize();

	// ************************** ��ѧ���� **************************************** //


	// *************************** ��� ***************************************** //

	int size() const;

private:

	vector<Array2D<T>> _array3D;

};

typedef Array3D<double>                Array3Dd;
