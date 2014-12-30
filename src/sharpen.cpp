#include "sharpen.hpp"
#include <opencv2/imgproc/imgproc.hpp>

void sharpen(const cv::Mat& input, cv::Mat& output) {
	
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
		kernel.at<float>(0, 1) = -1;
		kernel.at<float>(1, 2) = -1;
		kernel.at<float>(2, 1) = -1;
		kernel.at<float>(1, 0) = -1;
		kernel.at<float>(1, 1) = 5;

	cv::filter2D(input, output, input.depth(), kernel);
	
}