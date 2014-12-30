#include "edges.hpp"
#include <algorithm>
#include <numeric>
#include "sharpen.hpp"

void findEdges(const cv::Mat& input, cv::Mat& output) {
	cv::Mat sharpened(input.size(), input.depth());
	sharpen(input, sharpened);

	output.create(sharpened.size(), CV_32F);

	for(int row = 1; row < sharpened.rows - 1; ++row) {
		// rows involved in distance measurement
		const cv::Vec3b* previousRow = sharpened.ptr<cv::Vec3b>(row - 1);
		const cv::Vec3b* currentRow = sharpened.ptr<cv::Vec3b>(row);
		const cv::Vec3b* nextRow = sharpened.ptr<cv::Vec3b>(row + 1);
		float* outputRow = output.ptr<float>(row);

		// euclidean distances to the colors of neighboring pixels
		float distances[8]{};

		// calculate the color of each pixel
		for(int col = 1; col < sharpened.cols - 1; ++col) {
			distances[0] = edist(currentRow[col], previousRow[col - 1]);
			distances[1] = edist(currentRow[col], previousRow[col]);
			distances[2] = edist(currentRow[col], previousRow[col + 1]);
			distances[3] = edist(currentRow[col], currentRow[col - 1]);
			distances[4] = edist(currentRow[col], currentRow[col + 1]);
			distances[5] = edist(currentRow[col], nextRow[col - 1]);
			distances[6] = edist(currentRow[col], nextRow[col]);
			distances[7] = edist(currentRow[col], nextRow[col + 1]);

			// sort to get the greatest differences
			std::sort(distances, distances+8);

			outputRow[col] = std::log10(12 * std::accumulate(distances+4, distances+8, 0) / 4.0 / 441.67 - 2); // 441.67 -> max distance between 2 colors
		}
	}
}

float edist(const cv::Vec3b& A, const cv::Vec3b& B) {
	return sqrt(
		  std::pow(A[0] - B[0], 2)
		+ std::pow(A[1] - B[1], 2)
		+ std::pow(A[2] - B[2], 2)
	);
}