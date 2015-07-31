LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include /Users/Tony/tools/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES  := corner_detector.cpp
LOCAL_SRC_FILES  += DetectionBasedTracker_jni.cpp
LOCAL_SRC_FILES  += tracker.cpp
LOCAL_SRC_FILES  += tracking_engine.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := detection_based_tracker
#LOCAL_STATIC_LIBRARIES := opencv_java3

include $(BUILD_SHARED_LIBRARY)
