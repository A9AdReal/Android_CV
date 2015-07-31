#include <iostream>
#include "tracker.hpp"

void Tracker::trackPoints(const cv::Mat &prevImg, const std::vector<cv::Point2f> &prevPts,
		 	 	 	 	  const cv::Mat &currImg, std::vector<cv::Point2f> &currPts, std::vector<uchar> &trackStatus) {
	std::vector<float> err;
	cv::Size winSize = cv::Size(this->windowSize, this->windowSize);
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, this->maxIterations, this->epsilon);
	cv::calcOpticalFlowPyrLK(prevImg, currImg, prevPts, currPts, trackStatus, err, winSize, this->maxLevel, criteria, this->flags, this->minEigThresh);
	this->currPts_ = &currPts;
	this->trackStatus_ = &trackStatus;

}
