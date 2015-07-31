#include "tracking_engine.hpp"
#include <iostream>

using namespace cv;

TrackingEngine::TrackingEngine() {

	//init the kalman filter
	KF = KalmanFilter(8, 8, 8);  //numState, numMeasure, numControl
	setIdentity(KF.measurementMatrix); //C
	setIdentity(KF.transitionMatrix);  //A
	setIdentity(KF.controlMatrix); //B
	KF.processNoiseCov = Mat::eye(8, 8, CV_32F) * 0.01;
	KF.measurementNoiseCov = Mat::eye(8, 8, CV_32F) * 0.001;

	//others
	count = 0;
	isFirstFrame = true;
	points.clear();
}

bool TrackingEngine::addTrackingPoint(const Point2f& point, const Mat& frame, Point2f& corrected) {
	if (points.size() >= 4)
		return false;

	// add to point repo
	detector.detectCorner(frame, point, 10, corrected);
	points.push_back(corrected);

	// init kalman state
	int i = points.size() - 1;
	KF.statePost.at<float>(2 * i, 0) = corrected.x;
	KF.statePost.at<float>(2 * i + 1, 0) = corrected.y;

	return true;
}

bool TrackingEngine::trackAllPoints(const Mat& cFrame, std::vector<Point2f>& outPoints)
{
	// check first frame
	if (isFirstFrame) {
		isFirstFrame = false;
		pFrame = cFrame.clone();
		outPoints = points;
		return true;
	}

	// check point size
	if (points.size() == 0)
		return true;

	std::vector<Point2f> newPoints = points;
	std::vector<uchar> status;
	outPoints = points;

	Mat delta = Mat::zeros(cv::Size(1,8),CV_32F);
	Mat measure = Mat::zeros(cv::Size(1,8),CV_32F);

	if (count < 20) {
		// run tracker
		tracker.trackPoints(pFrame, points, cFrame, newPoints, status);
		for (int i = 0; i < (int) (points.size()); i++) {
			delta.at<float>(i * 2, 0) = newPoints[i].x - points[i].x;
			delta.at<float>(i * 2 + 1, 0) = newPoints[i].y - points[i].y;
		}

		// kalman predict
		KF.predict(delta);

	} else {
		// run detector
		for (int i = 0; i < (int) (points.size()); i++) {
			detector.detectCorner(cFrame, points[i], 10, newPoints[i]);
			measure.at<float>(i * 2, 0) = newPoints[i].x;
			measure.at<float>(i * 2 + 1, 0) = newPoints[i].y;
		}

		// kalman corrector
		KF.correct(measure);
		count = 0;
	}

	// update the inpts
	for (int i = 0; i < (int) (points.size()); i++) {
		points[i].x = KF.statePost.at<float>(i * 2, 0);
		points[i].y = KF.statePost.at<float>(i * 2 + 1, 0);
		outPoints[i].x = KF.statePost.at<float>(i * 2, 0);
		outPoints[i].y = KF.statePost.at<float>(i * 2 + 1, 0);
	}

	// copy the frame
	pFrame = cFrame.clone();

	return true;
}
