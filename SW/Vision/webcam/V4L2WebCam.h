///------------------------------------------- C++ Header file -------------------------------------
/// @file       V4L2WebCam.h
/// @brief      Definition of a web cam based on the Video for Linux 2 API (V4L2)
/// @project    Webcam
/// @version    1
/// @date       January 29, 2018
///-------------------------------------------------------------------------------------------------
///
/// Module description
/// ------------------
/// @file
/// @verbatim
/// Global usage description:
/// -------------------------
/// See V4L2WebCam.cc
///
/// @endverbatim
///-------------------------------------------------------------------------------------------------
#ifndef V4L2WEBCAM_H
#define V4L2WEBCAM_H

#include <string>
#include <mutex>
#include <linux/videodev2.h>

class V4L2WebCam
{
public:
  V4L2WebCam(v4l2_buf_type type, v4l2_memory memory);
  V4L2WebCam(const V4L2WebCam& src) = delete;
  virtual ~V4L2WebCam();
  
  void open(const std::string& device);
  void close() noexcept;
  bool isOpen() const noexcept;
  
  void setFormat(uint32_t width, uint32_t height, uint32_t pixelformat,
                 v4l2_field field = V4L2_FIELD_NONE);
  
  void setRequestBuffer(uint32_t numBuffers);
  void startStream(const std::string& file);
  
private:
  
  void allocateBuffer();
  static void throwIfError(bool errorOccured, int errNum, const std::string& errorMsg);
  
  int fileDescriptor() const noexcept;
  void open(const char* device) noexcept;
  
  const v4l2_buf_type m_bufferType;
  const v4l2_memory m_memory;
  int m_fileDescriptor;
  char* m_bufferPtr;
  v4l2_capability m_capability;
  
  mutable std::mutex m_mutex;
};

#endif /* V4L2WEBCAM_H */
