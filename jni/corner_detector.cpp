#include "corner_detector.hpp"
#include <iostream>

using namespace cv;

void CornerDetector::detectCorner(const Mat& img, const Point2f& searchCenter,
								  const int searchSize, Point2f& searchResult) {

	Mat patch = img.colRange(searchCenter.x - searchSize, searchCenter.x + searchSize)
				.rowRange(searchCenter.y - searchSize, searchCenter.y + searchSize);
	Mat dst = Mat::zeros(patch.size(), CV_32FC1);

	cornerHarris(patch, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	double min, max;
	Point min_loc, max_loc;
	minMaxLoc(dst, &min, &max, &min_loc, &max_loc);

	searchResult.x = searchCenter.x - searchSize + max_loc.x;
	searchResult.y = searchCenter.y - searchSize + max_loc.y;

}
