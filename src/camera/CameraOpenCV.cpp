#include "CameraOpenCV.h"
#include <cstring>
#include <sstream>

void printAllProps(cv::VideoCapture *cam);

unsigned int getSettingDouble(cv::VideoCapture *cam, int propId){
    try {
        return cam->get(propId);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV/getSettingDouble(cam, " << propId << "): exception caught: " << err_msg << "\n";
        return 0;
    }
}

unsigned int getSettingInt(cv::VideoCapture *cam, int propId){
    double raw_val = getSettingDouble(cam, propId);
    cout << "CameraOpenCV/getSettingInt(cam, " << propId << ") = " << raw_val;
    return round(raw_val);
}


bool setSettingDouble(cv::VideoCapture *cam, int propId, double val){
    try {
        return cam->set(propId, val);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV/setSettingDouble(cam, " << propId << ", " << val << "): exception caught: " << err_msg << "\n";
        return 0;
    }
}

vector<CameraInfo> CameraOpenCV::getCameraList(){
    vector<CameraInfo> ret;

    for (unsigned int i=0; i < CAMERAOPENCV_MAX_CAMS; i++) {
        CameraInfo info;
        try {
            cv::VideoCapture *cam = new cv::VideoCapture(i);
            if (!cam->isOpened()) {
                continue;
            }
            
            info.vendor = "OpenCV";
            std::ostringstream model;
            model << "Cam " << i;
            info.model = model.str();
            info.busID = (unsigned int)cam->get(CV_CAP_PROP_GUID);
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
        cam = new cv::VideoCapture(camNum);
    } catch (cv::Exception& e) {
        const char* err_msg = e.what();
        cerr << "CameraOpenCV::CameraOpenCV(" << camNum << ", " << triggerMode << "): exception caught: " << err_msg << "\n";
        cerr << "opencv: Could not open camera!";
        return;
    }

    // Disable most automatic stuff
    setSettingDouble(cam, CV_CAP_PROP_AUTO_EXPOSURE, 0);
    // setSettingDouble(cam, CV_CAP_PROP_AUTOFOCUS, 0);
    // setSettingDouble(cam, CV_CAP_PROP_AUTO_WB, 0);
    // Try to get it in monoscale
    setSettingDouble(cam, CV_CAP_PROP_CONVERT_RGB, 0);
    setSettingDouble(cam, CV_CAP_PROP_MONOCROME, 1);
    // Aparently we need to set it to ISO 800
    setSettingDouble(cam, CV_CAP_PROP_ISO_SPEED, 800);
    // CV_CAP_PROP_TRIGGER
    // CV_CAP_PROP_TRIGGER_DELAY

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

void printAllProps(cv::VideoCapture *cam) {
    string prop_names[] = {
        "CV_CAP_PROP_POS_MSEC",
        "CV_CAP_PROP_POS_FRAMES",
        "CV_CAP_PROP_POS_AVI_RATIO",
        "CV_CAP_PROP_FRAME_WIDTH",
        "CV_CAP_PROP_FRAME_HEIGHT",
        "CV_CAP_PROP_FPS",
        "CV_CAP_PROP_FOURCC",
        "CV_CAP_PROP_FRAME_COUNT",
        "CV_CAP_PROP_FORMAT",
        "CV_CAP_PROP_MODE",
        "CV_CAP_PROP_BRIGHTNESS",
        "CV_CAP_PROP_CONTRAST",
        "CV_CAP_PROP_SATURATION",
        "CV_CAP_PROP_HUE",
        "CV_CAP_PROP_GAIN",
        "CV_CAP_PROP_EXPOSURE",
        "CV_CAP_PROP_CONVERT_RGB",
        "CV_CAP_PROP_WHITE_BALANCE_U",
        "CV_CAP_PROP_RECTIFICATION",
        "CV_CAP_PROP_MONOCROME",
        "CV_CAP_PROP_SHARPNESS",
        "CV_CAP_PROP_AUTO_EXPOSURE",
        "CV_CAP_PROP_GAMMA",
        "CV_CAP_PROP_TEMPERATURE",
        "CV_CAP_PROP_TRIGGER",
        "CV_CAP_PROP_TRIGGER_DELAY",
        "CV_CAP_PROP_WHITE_BALANCE_V",
        "CV_CAP_PROP_ZOOM",
        "CV_CAP_PROP_FOCUS",
        "CV_CAP_PROP_GUID",
        "CV_CAP_PROP_ISO_SPEED",
        "CV_CAP_PROP_BACKLIGHT",
        "CV_CAP_PROP_PAN",
        "CV_CAP_PROP_TILT",
        "CV_CAP_PROP_ROLL",
        "CV_CAP_PROP_IRIS",
        "CV_CAP_PROP_SETTINGS",
        "CV_CAP_PROP_BUFFERSIZE",
        "CV_CAP_PROP_TEMPERATURE",
        "CV_CAP_PROP_PVAPI_PIXELFORMAT",
        "CV_CAP_PROP_XI_TIMEOUT"};
    int prop_ids[] = {
        CV_CAP_PROP_POS_MSEC,
        CV_CAP_PROP_POS_FRAMES,
        CV_CAP_PROP_POS_AVI_RATIO,
        CV_CAP_PROP_FRAME_WIDTH,
        CV_CAP_PROP_FRAME_HEIGHT,
        CV_CAP_PROP_FPS,
        CV_CAP_PROP_FOURCC,
        CV_CAP_PROP_FRAME_COUNT,
        CV_CAP_PROP_FORMAT,
        CV_CAP_PROP_MODE,
        CV_CAP_PROP_BRIGHTNESS,
        CV_CAP_PROP_CONTRAST,
        CV_CAP_PROP_SATURATION,
        CV_CAP_PROP_HUE,
        CV_CAP_PROP_GAIN,
        CV_CAP_PROP_EXPOSURE,
        CV_CAP_PROP_CONVERT_RGB,
        CV_CAP_PROP_WHITE_BALANCE_U,
        CV_CAP_PROP_RECTIFICATION,
        CV_CAP_PROP_MONOCROME,
        CV_CAP_PROP_SHARPNESS,
        CV_CAP_PROP_AUTO_EXPOSURE,
        CV_CAP_PROP_GAMMA,
        CV_CAP_PROP_TEMPERATURE,
        CV_CAP_PROP_TRIGGER,
        CV_CAP_PROP_TRIGGER_DELAY,
        CV_CAP_PROP_WHITE_BALANCE_V,
        CV_CAP_PROP_ZOOM,
        CV_CAP_PROP_FOCUS,
        CV_CAP_PROP_GUID,
        CV_CAP_PROP_ISO_SPEED,
        CV_CAP_PROP_BACKLIGHT,
        CV_CAP_PROP_PAN,
        CV_CAP_PROP_TILT,
        CV_CAP_PROP_ROLL,
        CV_CAP_PROP_IRIS,
        CV_CAP_PROP_SETTINGS,
        CV_CAP_PROP_BUFFERSIZE,
        CV_CAP_PROP_TEMPERATURE,
        CV_CAP_PROP_PVAPI_PIXELFORMAT,
        CV_CAP_PROP_XI_TIMEOUT,
        -1};
    for (int i=0; prop_ids[i] >= 0; i++) {
        double val = getSettingDouble(cam, prop_ids[i]);
        cout << prop_names << " = " << val << "\n";
    }
}

CameraSettings CameraOpenCV::getCameraSettings(){

    // Get settings:
    CameraSettings settings;
    settings.gain = getSettingInt(cam, CV_CAP_PROP_GAIN);
    settings.shutter = getSettingDouble(cam, CV_CAP_PROP_EXPOSURE)/1000;

    return settings;
}

void CameraOpenCV::setCameraSettings(CameraSettings settings){
    setSettingDouble(cam, CV_CAP_PROP_GAIN, settings.gain);
    setSettingDouble(cam, CV_CAP_PROP_EXPOSURE, settings.shutter/1000.0);
}

void CameraOpenCV::startCapture(){
}

void CameraOpenCV::stopCapture(){
}

CameraFrame CameraOpenCV::getFrame(){
    CameraFrame frame;

    if (raw_img == NULL) {
        raw_img = new cv::Mat();
    }

    if (!cam->read(*raw_img)) {
        cerr << "opencv: failed to read frame";
    }

    // Copy frame address and properties
    frame.memory = raw_img->data;
    frame.width = raw_img->cols;
    frame.height = raw_img->rows;
    frame.sizeBytes = raw_img->elemSize()*frame.width*frame.height;
    total_frame_bytes = frame.sizeBytes;

    return frame;
}

size_t CameraOpenCV::getFrameSizeBytes(){
    return total_frame_bytes;
}

size_t CameraOpenCV::getFrameWidth(){
    return getSettingInt(cam, CV_CAP_PROP_FRAME_WIDTH);
}


size_t CameraOpenCV::getFrameHeight(){
    return getSettingInt(cam, CV_CAP_PROP_FRAME_HEIGHT);
}


CameraOpenCV::~CameraOpenCV(){
    if (raw_img != NULL) {
        raw_img->release();
    }
    cam->release();
}



