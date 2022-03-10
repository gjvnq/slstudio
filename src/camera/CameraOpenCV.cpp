#include "CameraOpenCV.h"
#include <cstring>

vector<CameraInfo> CameraOpenCV::getCameraList(){
    vector<CameraInfo> ret;

    for (unsigned int i=0; i < CAMERAOPENCV_MAX_CAMS; i++) {
        CameraInfo info;
        try {
            VideoCapture cam = new VideoCapture(i);
            info.vendor = "OpenCV";
            info.model = cam.getBackendName();
            info.busID = (unsigned int)cam.get(cv::CAP_PROP_GUID);
            ret.push_back(info);
        } catch (cv::Exception& e) {
            const char* err_msg = e.what();
            cerr << "CameraOpenCV::getCameraList(): exception caught: " << err_msg << "\n";
        }
    }
    
    return ret;
}

CameraOpenCV::CameraOpenCV(unsigned int camNum, CameraTriggerMode triggerMode) : Camera(triggerMode) {

    try {
        cam = new VideoCapture(camNum);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV::CameraOpenCV(" << camNum << ", " << triggerMode << "): exception caught: " << err_msg << "\n";
        cerr << "opencv: Could not open camera!";
        return;
    }

    // Disable most automatic stuff
    safeSetProp(cam, cv::CAP_PROP_AUTO_EXPOSURE, 0);
    safeSetProp(cam, cv::CAP_PROP_AUTOFOCUS, 0);
    safeSetProp(cam, cv::CAP_PROP_AUTO_WB, 0);
    // Try to get it in monoscale
    safeSetProp(cam, cv::CAP_PROP_CONVERT_RGB, 0);
    safeSetProp(cam, cv::CAP_PROP_MONOCHROME, 1);
    // Aparently we need to set it to ISO 800
    safeSetProp(cam, cv::CAP_PROP_ISO_SPEED, 800);
    // CAP_PROP_TRIGGER
    // CAP_PROP_TRIGGER_DELAY

    // Set reasonable default settings
    CameraSettings settings;
    settings.shutter = 16.66;
    settings.gain = 0.0;
    this->setCameraSettings(settings);

    // Print camera features and settings
    printAllProps(cam);
    fflush(stdout);

    return;
}

void printAllProps(VideoCapture *cam) {
    string[] prop_names = {
        "cv::CAP_PROP_POS_MSEC",
        "cv::CAP_PROP_POS_FRAMES",
        "cv::CAP_PROP_POS_AVI_RATIO",
        "cv::CAP_PROP_FRAME_WIDTH",
        "cv::CAP_PROP_FRAME_HEIGHT",
        "cv::CAP_PROP_FPS",
        "cv::CAP_PROP_FOURCC",
        "cv::CAP_PROP_FRAME_COUNT",
        "cv::CAP_PROP_FORMAT",
        "cv::CAP_PROP_MODE",
        "cv::CAP_PROP_BRIGHTNESS",
        "cv::CAP_PROP_CONTRAST",
        "cv::CAP_PROP_SATURATION",
        "cv::CAP_PROP_HUE",
        "cv::CAP_PROP_GAIN",
        "cv::CAP_PROP_EXPOSURE",
        "cv::CAP_PROP_CONVERT_RGB",
        "cv::CAP_PROP_WHITE_BALANCE_BLUE_U",
        "cv::CAP_PROP_RECTIFICATION",
        "cv::CAP_PROP_MONOCHROME",
        "cv::CAP_PROP_SHARPNESS",
        "cv::CAP_PROP_AUTO_EXPOSURE",
        "cv::CAP_PROP_GAMMA",
        "cv::CAP_PROP_TEMPERATURE",
        "cv::CAP_PROP_TRIGGER",
        "cv::CAP_PROP_TRIGGER_DELAY",
        "cv::CAP_PROP_WHITE_BALANCE_RED_V",
        "cv::CAP_PROP_ZOOM",
        "cv::CAP_PROP_FOCUS",
        "cv::CAP_PROP_GUID",
        "cv::CAP_PROP_ISO_SPEED",
        "cv::CAP_PROP_BACKLIGHT",
        "cv::CAP_PROP_PAN",
        "cv::CAP_PROP_TILT",
        "cv::CAP_PROP_ROLL",
        "cv::CAP_PROP_IRIS",
        "cv::CAP_PROP_SETTINGS",
        "cv::CAP_PROP_BUFFERSIZE",
        "cv::CAP_PROP_AUTOFOCUS",
        "cv::CAP_PROP_SAR_NUM",
        "cv::CAP_PROP_SAR_DEN",
        "cv::CAP_PROP_BACKEND",
        "cv::CAP_PROP_CHANNEL",
        "cv::CAP_PROP_AUTO_WB",
        "cv::CAP_PROP_WB_TEMPERATURE",
        "cv::CAP_PROP_CODEC_PIXEL_FORMAT",
        "cv::CAP_PROP_BITRATE",
        "cv::CAP_PROP_ORIENTATION_META",
        "cv::CAP_PROP_ORIENTATION_AUTO",
        "cv::CAP_PROP_HW_ACCELERATION",
        "cv::CAP_PROP_HW_DEVICE",
        "cv::CAP_PROP_HW_ACCELERATION_USE_OPENCL",
        "cv::CAP_PROP_OPEN_TIMEOUT_MSEC",
        "cv::CAP_PROP_READ_TIMEOUT_MSEC",
        "cv::CAP_PROP_STREAM_OPEN_TIME_USEC",
        "cv::CAP_PROP_VIDEO_TOTAL_CHANNELS",
        "cv::CAP_PROP_VIDEO_STREAM",
        "cv::CAP_PROP_AUDIO_STREAM",
        "cv::CAP_PROP_AUDIO_POS",
        "cv::CAP_PROP_AUDIO_SHIFT_NSEC",
        "cv::CAP_PROP_AUDIO_DATA_DEPTH",
        "cv::CAP_PROP_AUDIO_SAMPLES_PER_SECOND",
        "cv::CAP_PROP_AUDIO_BASE_INDEX",
        "cv::CAP_PROP_AUDIO_TOTAL_CHANNELS",
        "cv::CAP_PROP_AUDIO_TOTAL_STREAMS",
        "cv::CAP_PROP_AUDIO_SYNCHRONIZE",
        "cv::CAP_PROP_LRF_HAS_KEY_FRAME",
        "cv::CAP_PROP_CODEC_EXTRADATA_INDEX"};
    int[] prop_ids = {
        cv::CAP_PROP_POS_MSEC,
        cv::CAP_PROP_POS_FRAMES,
        cv::CAP_PROP_POS_AVI_RATIO,
        cv::CAP_PROP_FRAME_WIDTH,
        cv::CAP_PROP_FRAME_HEIGHT,
        cv::CAP_PROP_FPS,
        cv::CAP_PROP_FOURCC,
        cv::CAP_PROP_FRAME_COUNT,
        cv::CAP_PROP_FORMAT,
        cv::CAP_PROP_MODE,
        cv::CAP_PROP_BRIGHTNESS,
        cv::CAP_PROP_CONTRAST,
        cv::CAP_PROP_SATURATION,
        cv::CAP_PROP_HUE,
        cv::CAP_PROP_GAIN,
        cv::CAP_PROP_EXPOSURE,
        cv::CAP_PROP_CONVERT_RGB,
        cv::CAP_PROP_WHITE_BALANCE_BLUE_U,
        cv::CAP_PROP_RECTIFICATION,
        cv::CAP_PROP_MONOCHROME,
        cv::CAP_PROP_SHARPNESS,
        cv::CAP_PROP_AUTO_EXPOSURE,
        cv::CAP_PROP_GAMMA,
        cv::CAP_PROP_TEMPERATURE,
        cv::CAP_PROP_TRIGGER,
        cv::CAP_PROP_TRIGGER_DELAY,
        cv::CAP_PROP_WHITE_BALANCE_RED_V,
        cv::CAP_PROP_ZOOM,
        cv::CAP_PROP_FOCUS,
        cv::CAP_PROP_GUID,
        cv::CAP_PROP_ISO_SPEED,
        cv::CAP_PROP_BACKLIGHT,
        cv::CAP_PROP_PAN,
        cv::CAP_PROP_TILT,
        cv::CAP_PROP_ROLL,
        cv::CAP_PROP_IRIS,
        cv::CAP_PROP_SETTINGS,
        cv::CAP_PROP_BUFFERSIZE,
        cv::CAP_PROP_AUTOFOCUS,
        cv::CAP_PROP_SAR_NUM,
        cv::CAP_PROP_SAR_DEN,
        cv::CAP_PROP_BACKEND,
        cv::CAP_PROP_CHANNEL,
        cv::CAP_PROP_AUTO_WB,
        cv::CAP_PROP_WB_TEMPERATURE,
        cv::CAP_PROP_CODEC_PIXEL_FORMAT,
        cv::CAP_PROP_BITRATE,
        cv::CAP_PROP_ORIENTATION_META,
        cv::CAP_PROP_ORIENTATION_AUTO,
        cv::CAP_PROP_HW_ACCELERATION,
        cv::CAP_PROP_HW_DEVICE,
        cv::CAP_PROP_HW_ACCELERATION_USE_OPENCL,
        cv::CAP_PROP_OPEN_TIMEOUT_MSEC,
        cv::CAP_PROP_READ_TIMEOUT_MSEC,
        cv::CAP_PROP_STREAM_OPEN_TIME_USEC,
        cv::CAP_PROP_VIDEO_TOTAL_CHANNELS,
        cv::CAP_PROP_VIDEO_STREAM,
        cv::CAP_PROP_AUDIO_STREAM,
        cv::CAP_PROP_AUDIO_POS,
        cv::CAP_PROP_AUDIO_SHIFT_NSEC,
        cv::CAP_PROP_AUDIO_DATA_DEPTH,
        cv::CAP_PROP_AUDIO_SAMPLES_PER_SECOND,
        cv::CAP_PROP_AUDIO_BASE_INDEX,
        cv::CAP_PROP_AUDIO_TOTAL_CHANNELS,
        cv::CAP_PROP_AUDIO_TOTAL_STREAMS,
        cv::CAP_PROP_AUDIO_SYNCHRONIZE,
        cv::CAP_PROP_LRF_HAS_KEY_FRAME,
        cv::CAP_PROP_CODEC_EXTRADATA_INDEX,
        -1};
    for (int i=0; prop_ids[i] >= 0; i++) {
        double val = getSettingDouble(cam, prop_ids[i]);
        cout << prop_names << " = " << val << "\n";
    }
}

bool safeSetProp(VideoCapture *cam, int propId, double value) {
    try {
        double ans = cam.set(propId, value);
        cout << "CameraOpenCV/setSetting(cam, " << propId << ", " << value << ") = " << ans;
        return ans;
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV/safeSetProp(cam, " << propId << "): exception caught: " << err_msg << "\n";
        return false;
    }
}

unsigned int getSettingDouble(VideoCapture *cam, int propId){
    try {
        return cam.get(propId);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV/getSettingDouble(cam, " << propId << "): exception caught: " << err_msg << "\n";
        return 0;
    }
}

unsigned int getSettingInt(VideoCapture *cam, int propId){
    double raw_val = getSettingDouble(cam, propId);
    cout << "CameraOpenCV/getSettingInt(cam, " << propId << ") = " << raw_val;
    return round(raw_val);
}


bool setSettingDouble(VideoCapture *cam, int propId, double val){
    try {
        return cam.set(propId, val);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV/setSettingDouble(cam, " << propId << ", " << val << "): exception caught: " << err_msg << "\n";
        return 0;
    }
}

CameraSettings CameraOpenCV::getCameraSettings(){

    // Get settings:
    CameraSettings settings;
    settings.gain = getSettingInt(cam, cv::CAP_PROP_GAIN);
    settings.shutter = getSettingDouble(cam, cv::CAP_PROP_EXPOSURE)/1000;

    return settings;
}

void setSetting(dc1394camera_t *cam, dc1394feature_t setting, float value){
    dc1394error_t err;
    float min, max;
    dc1394_feature_get_absolute_boundaries(cam, setting, &min, &max);
    if(value < min || value > max){
        std::cerr << "CameraOpenCV: cannot set value. Out of permissable range." << std::endl;
    } else {
        err = dc1394_feature_set_absolute_control(cam, setting, DC1394_ON);
        if(err != DC1394_SUCCESS)
            cerr << "Could not enable absolute control!" << endl;
        err = dc1394_feature_set_absolute_value(cam, setting, value);
        if(err != DC1394_SUCCESS)
            cerr << "Could not set absolute value!" << endl;
    }
}

void CameraOpenCV::setCameraSettings(CameraSettings settings){
    setSettingDouble(cam, cv::CAP_PROP_GAIN, settings.gain);
    setSettingDouble(cam, cv::CAP_PROP_EXPOSURE, settings.shutter/1000.0);
}

void CameraOpenCV::startCapture(){
}

void CameraOpenCV::stopCapture(){
}

CameraFrame CameraOpenCV::getFrame(){
    CameraFrame frame;

    if (raw_img == NULL) {
        raw_img = cv::Mat();
    }

    if (!cam.read(raw_img)) {
        cerr << "opencv: failed to read frame";
    }

    // Copy frame address and properties
    frame.memory = currentFrame->image;
    frame.width = raw_img.cols();
    frame.height = raw_img.rows();
    frame.sizeBytes = raw_img.elemSize()*frame.width*frame.height;

    return frame;
}

void CameraOpenCV::flushBuffer(){
    // // This function is courtesy of ofxVideoGrabber/Libdc1394Grabber
    // bool bufferEmpty = false;

    // while (!bufferEmpty){
    //     if(dc1394_capture_dequeue(cam, DC1394_CAPTURE_POLICY_POLL, &currentFrame) == DC1394_SUCCESS){
    //         if(currentFrame != NULL){
    //             dc1394_capture_enqueue(cam, currentFrame);
    //         } else {
    //             bufferEmpty = true;
    //         }
    //     } else {
    //         bufferEmpty = true;
    //     }
    // }
}


size_t CameraOpenCV::getFrameSizeBytes(){
    if (!capturing) {
        cerr << "ERROR: Cannot get frame size before capturing. Call startCapture() before getFrameSizeBytes()." << endl;
        return 0;
    }

//    dc1394video_frame_t *frame = NULL;
//    dc1394error_t err;
    
//	// Get frame from ring buffer:
//    err = dc1394_capture_dequeue(cam, DC1394_CAPTURE_POLICY_WAIT, &frame);
//    err = dc1394_capture_enqueue(cam, frame);
    
//    return frame->image_bytes;
    uint64_t total_bytes;
    dc1394_format7_get_total_bytes(cam, video_mode, &total_bytes);

    return total_bytes;
}

size_t CameraOpenCV::getFrameWidth(){
    return getSettingInt(cam, cv::CAP_PROP_FRAME_WIDTH);
}


size_t CameraOpenCV::getFrameHeight(){
    return getSettingInt(cam, cv::CAP_PROP_FRAME_HEIGHT);
}


CameraOpenCV::~CameraOpenCV(){
    if (raw_img != NULL) {
        raw_img.release();
    }
    cam.release();
}



