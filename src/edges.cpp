#include "edges.hpp"
#include <algorithm>
#include <numeric>
#include "sharpen.hpp"

#define MAXDISTANCE 441.67

struct Closer {
	const cv::Vec3b* base;
	Closer(): base(nullptr) {}
	bool operator()(const cv::Vec3b* A, const cv::Vec3b* B)
		{ return edist(*base, *A) < edist(*base, *B); }
} closer;

void findEdges(const cv::Mat& input, cv::Mat& output) {
	cv::Mat sharpened(input.size(), input.depth());
	sharpen(input, sharpened);
	output.create(sharpened.size(), CV_32F);

	const cv::Vec3b white{255,255,255};

	for(int row = 1; row < sharpened.rows - 1; ++row) {
		// rows involved in distance measurement
		const cv::Vec3b* previousRow = sharpened.ptr<cv::Vec3b>(row - 1);
		const cv::Vec3b* currentRow = sharpened.ptr<cv::Vec3b>(row);
		const cv::Vec3b* nextRow = sharpened.ptr<cv::Vec3b>(row + 1);
		float* outputRow = output.ptr<float>(row);

		// map neighboring pixels
		const cv::Vec3b* neighbors[8]{};

		// calculate the color of each pixel
		for(int col = 1; col < sharpened.cols - 1; ++col) {
			neighbors[0] = &previousRow[col - 1];
			neighbors[1] = &previousRow[col];
			neighbors[2] = &previousRow[col + 1];
			neighbors[3] = &currentRow[col - 1];
			neighbors[4] = &currentRow[col + 1];
			neighbors[5] = &nextRow[col - 1];
			neighbors[6] = &nextRow[col];
			neighbors[7] = &nextRow[col + 1];

			// sort to get the greatest differences
			closer.base = &currentRow[col];
			std::sort(neighbors, neighbors+8, closer);

			// are 3 of the 4 border pixels brighter?
			int brighter = 0;
			for(int i = 4; i < 8; ++i)
				if(edist(currentRow[i], white) - edist(*neighbors[i], white) > 0)
					++brighter;

			// if 3 pixels are brighter, color
			if(brighter >= 3)
				outputRow[col] = std::log10(12 *
					( edist(currentRow[col], *neighbors[4]) + edist(currentRow[col], *neighbors[5])
					+ edist(currentRow[col], *neighbors[6]) + edist(currentRow[col], *neighbors[7]) )
						/ 4.0 / MAXDISTANCE - 2 );
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