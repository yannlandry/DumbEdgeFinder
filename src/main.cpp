#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "edges.hpp"

int main(int argc, char* argv[]) {
	if(argc < 2) {
		std::cerr << "Please enter the name of the image file in which to find edges." << std::endl;
		return 0;
	}

	std::string imageFile(argv[1]);
	cv::Mat image(cv::imread(imageFile));
	
	if(image.rows == 0 || image.cols == 0) {
		std::cerr << "Please enter a valid image file." << std::endl;
		return 0;
	}

	cv::Mat edges; // blank image
	findEdges(image, edges);

	// show before & after
	cv::imshow(imageFile, image);
	cv::imshow("Edges of " + imageFile, edges);
	
	// convert & save image to file
	edges.convertTo(edges, CV_8UC1, 255.0); 
	cv::imwrite("edges." + imageFile, edges);

	// wait
	cv::waitKey(0);

	return 0;
}