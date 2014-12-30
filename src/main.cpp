#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "edges.hpp"
#include "sharpen.hpp"

int main() {
	cv::Mat image(cv::imread("swiss.jpg"));
	cv::Mat edges;

	//std::cout << image.channels() << std::endl;
	findEdges(image, edges);

	cv::imshow("nyc.jpg", image);
	cv::imshow("Edges of nyc.jpg", edges);
	cv::waitKey(0);

	return 0;
}