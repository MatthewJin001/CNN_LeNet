#pragma once

#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <string>
#include <vector>

// ���ܳ��֣�using namespace cv;
using namespace std;

cv::Mat matrix_double_to_Mat_64FC1(double *array, int row, int col);

cv::Mat vector_vector_double_to_Mat_64FC1(const vector<vector<double>> &array);

void show_matrix_double_as_image_64FC1(double *array, int row, int col, int time_msec);

void show_vector_vector_double_as_image_64FC1(const vector<vector<double>> &array, int time_msec);

void multi_images_64FC1_show_one_window(const std::string& MultiShow_WinName, const vector<cv::Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size, int time_msec);

void multi_images_8UC3_show_one_window(const std::string& MultiShow_WinName, const vector<cv::Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size, int time_msec);

void read_batch_images(string file_addr, string image_suffix, int begin_num, int end_num, vector<cv::Mat> &data_set);

void images_convert_to_64FC1(vector<cv::Mat> &data_set);

void show_curve_image(vector<double>data_x, vector<double>data_y, float scale, int msec);