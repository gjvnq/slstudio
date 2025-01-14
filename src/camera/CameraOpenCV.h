#ifndef CAMERAOPENCV_H
#define CAMERAOPENCV_H

#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>
#define CAMERAOPENCV_MAX_CAMS 10

using namespace std;

class CameraOpenCV : public Camera {
    public:
        // Static methods
        static vector<CameraInfo> getCameraList();
        // Interface function
        CameraOpenCV(unsigned int camNum, CameraTriggerMode triggerMode);
        CameraSettings getCameraSettings();
        void setCameraSettings(CameraSettings);
        void startCapture();
        void stopCapture();
        CameraFrame getFrame();
        size_t getFrameSizeBytes();
        size_t getFrameWidth();
        size_t getFrameHeight();
        ~CameraOpenCV();
    private:
        cv::VideoCapture *cam;
        cv::Mat *raw_img;
        size_t total_frame_bytes;
        // dc1394_t *context;
        // dc1394camera_t *cam;
        // dc1394video_mode_t video_mode;
        // dc1394video_frame_t *currentFrame;
        // void flushBuffer();
};

#endif
