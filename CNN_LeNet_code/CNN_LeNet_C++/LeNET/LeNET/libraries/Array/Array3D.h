#pragma once
#include <Array2D.h>

template <typename T>
class Array3D
{
public:
	// ����
	Array3D() { }

	Array3D(int page, int col, int row, T value);

	// from vector image 64FC1
	Array3D(const vector<Mat> &vector_img);

	// *************************** ���� ***************************************** //

	void from_vector_image_64FC1(const vector<Mat> &vector_img);

	// *************************** ��ֵ ***************************************** //

	void create(int page, int col, int row, T value);

	Array2D<T>& at(int page);// д

	const Array2D<T>& at(int page) const;// ��

	void push_back(const Array2D<T> &val);


	// *************************** ���� ***************************************** //

	void set_zero();

	void set_value(T val);

	void set_zero_same_size_as(const Array3D<T> &array2D);

	// ��һ��Ϊ0~1
	void normalize();

	Array3D<T> sampling(const int &sample_interval) const;

	// ************************** ��ѧ���� **************************************** //

	Array3D<T> operator + (const T &val) const;

	Array3D<T> operator * (const T &val) const;

	void add(const Array3D<T> &array3D);

	void dot_product(const Array3D<T> &array3D);

	// *************************** ��� ***************************************** //

	int size() const;

	void print() const;

	void show_specified_images_64FC1(const std::string& MultiShow_WinName, CvSize SubPlot, CvSize ImgMax_Size, int time_msec) const;

private:

	vector<Mat> to_vector_Mat_64FC1() const;

	vector<Array2D<T>> _array3D;

};

typedef Array3D<double>                Array3Dd;
