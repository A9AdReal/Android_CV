#ifndef _TRACKING_ENGINE_HPP
#define _TRACKING_ENGINE_HPP

#include <vector>
#include <opencv2/core.hpp>
#include "tracker.hpp"
#include "corner_detector.hpp"

using namespace cv;

class TrackingEngine {

public:
	TrackingEngine();
	~TrackingEngine(){};

	bool addTrackingPoint(const Point2f& point, const Mat& frame, Point2f& corrected);
	bool trackAllPoints(const Mat& cFrame, std::vector<Point2f>& outPoints);
	bool trackAllPointsBrutal(const Mat& cFrame, std::vector<Point2f>& outPoints);
	void resetPoints();

private:
	CornerDetector detector;
	Tracker tracker;
	KalmanFilter KF;
	int count;

	std::vector<Point2f> points;
	cv::Mat pFrame;
	bool isFirstFrame;
};

#endif
