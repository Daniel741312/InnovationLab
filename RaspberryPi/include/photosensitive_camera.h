#ifndef __PHOTOSENSITIVE_CAMERA_H__
#define __PHOTOSENSITIVE_CAMERA_H__

namespace photosensitive_camera {

class PhotoSentiveCamera {
   public:
    PhotoSentiveCamera(int DO, bool trigerByA = false);
    ~PhotoSentiveCamera();
    void WaitForDO(void (*func)(void*), void* arg);
    int TakeAPic(const char* delay);
    int do_;
    bool triger_mode_=false;
};

}  // namespace photosensitive_camera

#endif
