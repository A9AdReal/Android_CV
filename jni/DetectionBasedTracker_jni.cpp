#include "tracking_engine.hpp"
#include "a9_com_a9adsrealandroid_VideoRegionSelectActivity.h"

TrackingEngine engine;

/*
 * Class:     a9_com_a9adsrealandroid_VideoRegionSelectActivity
 * Method:    findCornerOnScreen
 * Signature: (Landroid/graphics/PointF;[B)Landroid/graphics/PointF;
 */
// return: PointF
// input : PointF, byte[]
JNIEXPORT jfloatArray JNICALL Java_a9_com_a9adsrealandroid_VideoRegionSelectActivity_findCornerOnScreen(
		JNIEnv *env, jobject obj, jfloatArray jpoint, jbyteArray jframe, jint width,
		jint height) {

	// convert point
	jfloat *iarr = env->GetFloatArrayElements(jpoint, 0);
	cv::Point2f cvPoint;
	cvPoint.x = iarr[0]*width;
	cvPoint.y = iarr[1]*height;

	// convert array
	jbyte* jbae = env->GetByteArrayElements(jframe, 0);
	Mat frame(height + height / 2, width, CV_8UC1, (unsigned char *) jbae);
	Mat frame_g;
	cvtColor(frame, frame_g, COLOR_YUV2GRAY_420);

	// ivoke engine
	cv::Point2f corrected;
	engine.addTrackingPoint(cvPoint, frame_g, corrected);

	// return the array
	jfloat result[2];
	result[0]=corrected.x/width;
	result[1]=corrected.y/height;
	jfloatArray newArray = env->NewFloatArray(2);
	env->SetFloatArrayRegion(newArray, 0, 2, result);

	// release memory
	env->ReleaseFloatArrayElements(jpoint, iarr, 0);
	env->ReleaseByteArrayElements(jframe, jbae, 0);

	return newArray;
}

/*
 * Class:     a9_com_a9adsrealandroid_VideoRegionSelectActivity
 * Method:    trackingPoint
 * Signature: (Ljava/util/ArrayList;[B[B)Ljava/util/ArrayList;
 */
// return: ArrayList<PointF>
// input : ArrayList<PointF>, preFrame, curFrame
JNIEXPORT jfloatArray JNICALL Java_a9_com_a9adsrealandroid_VideoRegionSelectActivity_trackingPoint(
		JNIEnv * env, jobject, jfloatArray jpoints, jbyteArray jpframe,
		jbyteArray jcframe, jint width, jint height) {

	// ignore jpframe, jpoints
	// convert jcframe
	jbyte* jbae = env->GetByteArrayElements(jcframe, 0);
	Mat frame(height + height / 2, width, CV_8UC1, (unsigned char *) jbae);
	Mat frame_g;
	cvtColor(frame, frame_g, COLOR_YUV2GRAY_420);

	// invoke engine
	std::vector<Point2f> cvPoints;
	engine.trackAllPoints(frame_g, cvPoints);

	// return
	jfloatArray newArray = env->NewFloatArray(cvPoints.size());
	jfloat *narr = env->GetFloatArrayElements(newArray, 0);
	for (int i = 0; i < cvPoints.size(); i++) {
		narr[2 * i] = cvPoints[i].x/width;
		narr[2 * i + 1] = cvPoints[i].y/height;
	}
	return newArray;
}

/*
 * Class:     a9_com_a9adsrealandroid_VideoRegionSelectActivity
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_a9_com_a9adsrealandroid_VideoRegionSelectActivity_init
(JNIEnv *, jobject) {
	// do nothing for now
}
