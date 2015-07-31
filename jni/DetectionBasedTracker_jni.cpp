#include "tracking_engine.hpp"
#include "a9_com_a9adsrealandroid_VideoRegionSelectActivity.h"
#include <android/log.h>

#define LOG_TAG "AdsRealCV"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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

	int newHeight = height + height / 2;

	// convert point
	jfloat *iarr = env->GetFloatArrayElements(jpoint, 0);
	cv::Point2f cvPoint;
	cvPoint.x = iarr[1]*width;
	cvPoint.y = (1-iarr[0])*height;

	// convert array
	jbyte* jbae = env->GetByteArrayElements(jframe, 0);
	Mat frame(newHeight, width, CV_8UC1, (unsigned char *) jbae);
	Mat frame_g;
	cvtColor(frame, frame_g, COLOR_YUV2GRAY_420);

	LOGE("The image size rows: %d, cols: %d", frame_g.rows, frame_g.cols);
	LOGE("height: %d, width %d", height, width);
	LOGE("opencv x:%f, y:%f",cvPoint.x, cvPoint.y);

	// ivoke engine
	cv::Point2f corrected;
	engine.addTrackingPoint(cvPoint, frame_g, corrected);

	// return the array
	jfloat result[2];
	result[0]=1-corrected.y/height;
	result[1]=corrected.x/width;
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

	// return the array
	jfloatArray newArray = env->NewFloatArray(cvPoints.size()*2);
	jfloat *result = new jfloat[cvPoints.size()*2];
	for (int i = 0; i < cvPoints.size(); i++) {
		result[2 * i] = 1-cvPoints[i].y/height;
		result[2 * i + 1] = cvPoints[i].x/width;
	}
	env->SetFloatArrayRegion(newArray, 0, cvPoints.size()*2, result);

	// free the memory
	delete result;
	env->ReleaseByteArrayElements(jcframe, jbae, 0);

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
