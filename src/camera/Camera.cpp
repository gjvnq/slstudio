#include "Camera.h"

#ifdef WITH_CAMERAIIDC
    #include "CameraIIDC.h"
#endif
#ifdef WITH_CAMERAXIMEA
    #include "CameraXIMEA.h"
#endif
#ifdef WITH_CAMERAIDSIMAGING
    #include "CameraIDSImaging.h"
#endif
#ifdef WITH_CAMERAPOINTGREY
    #include "CameraPointGrey.h"
#endif
#ifdef WITH_CAMERAOPENCV
    #include "CameraOpenCV.h"
#endif

// Global camera enumerator
std::vector< std::vector<CameraInfo> > Camera::GetInterfaceCameraList(){
    std::vector< std::vector<CameraInfo> > ret;

#ifdef WITH_CAMERAIIDC
    std::vector<CameraInfo> iidccameras = CameraIIDC::getCameraList();
    ret.push_back(iidccameras);
#endif
#ifdef WITH_CAMERAXIMEA
    std::vector<CameraInfo> ximeacameras = CameraXIMEA::getCameraList();
    ret.push_back(ximeacameras);
#endif
#ifdef WITH_CAMERAIDSIMAGING
    std::vector<CameraInfo> idscameras = CameraIDSImaging::getCameraList();
    ret.push_back(idscameras);
#endif
#ifdef WITH_CAMERAPOINTGREY
    std::vector<CameraInfo> ptgreycameras = CameraPointGrey::getCameraList();
    ret.push_back(ptgreycameras);
#endif
#ifdef WITH_CAMERAOPENCV
    std::vector<CameraInfo> opencvcameras = CameraOpenCV::getCameraList();
    ret.push_back(opencvcameras);
#endif

    return ret;
}

// Camera factory
Camera* Camera::NewCamera(unsigned int interfaceNum, unsigned int camNum, CameraTriggerMode triggerMode){

    interfaceNum += 1;

#ifdef WITH_CAMERAIIDC
    interfaceNum -= 1;
    if(interfaceNum == 0)
        return new CameraIIDC(camNum, triggerMode);
#endif
#ifdef WITH_CAMERAXIMEA
    interfaceNum -= 1;
    if(interfaceNum == 0)
        return new CameraXIMEA(camNum, triggerMode);
#endif
#ifdef WITH_CAMERAIDSIMAGING
    interfaceNum -= 1;
    if(interfaceNum == 0)
        return new CameraIDSImaging(camNum, triggerMode);
#endif
#ifdef WITH_CAMERAPOINTGREY
    interfaceNum -= 1;
    if(interfaceNum == 0)
        return new CameraPointGrey(camNum, triggerMode);
#endif
#ifdef WITH_CAMERAOPENCV
    interfaceNum -= 1;
    if(interfaceNum == 0)
        return new CameraOpenCV(camNum, triggerMode);
#endif

    return (Camera*)NULL;
}
