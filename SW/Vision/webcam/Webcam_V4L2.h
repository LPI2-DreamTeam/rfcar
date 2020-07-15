/**
 * @file Webcam_V4L2.h
 * @brief Webcam wrapper interface using V4L2 API
 */

#ifndef WEBCAM_V4L2_H
#define WEBCAM_V4L2_H

#include <string>
#include <mutex>
#include <linux/videodev2.h>

class Webcam_V4L2
{
public:
  
/// @brief Constructor
/// @param type: type of capture to perform
/// @param memory: type of buffer memory used
  Webcam_V4L2(v4l2_buf_type type, v4l2_memory memory);
  Webcam_V4L2(const Webcam_V4L2& src) = delete;
  virtual ~Webcam_V4L2();
  
/// @brief Opens the web cam using a named device node
/// @param device: e.g, /dev/video0
  void open(const std::string& device);

/// @brief Close the webcam
  void close() noexcept;

/// @brief Queries if the webcam has been opened
/// @return True if opened, false otherwise
  bool isOpen() const noexcept;
  
/// @brief Sets the image format and dimensions
/// @param width        The width of the image in pixels
/// @param height       The height of the image in pixels
/// @param pixelformat  The encoding of the image (e.g., JPEG)
/// @param field        Unknown
  void setFormat(uint32_t width, uint32_t height, uint32_t pixelformat,
                 v4l2_field field = V4L2_FIELD_NONE);
  
/// @brief Sets the nr of buffers to use
/// @param numBuffers: nr of buffers to use
  void setRequestBuffer(uint32_t numBuffers);

/// @brief Starts the stream and writes the output to a file
/// @param file The filename to write to
  void startStream(const std::string& file);
  
private:
  
/// @brief allocates buffer on @setRequestBuffer behalf
  void allocateBuffer();

/// @brief Throws the message associated to an error
  static void throwIfError(bool errorOccured, int errNum,
                           const std::string& errorMsg);

/// @brief Maintains integrity of the file descriptor (FID) and returns it
/// @return FID if valid, -1 otherwise
  int fileDescriptor() const noexcept;

/// @brief Low level call to open device
/// @param device: device to be opened
  void open(const char* device) noexcept;

    /* V4L2 variables */
  const v4l2_buf_type m_bufferType; /**< Buffer type */
  const v4l2_memory m_memory; /**< Buffer memory */
  v4l2_capability m_capability; /**< Buffer capabilites */
 
  int m_fileDescriptor; /**< file descriptior associated with the device */ 
  char* m_bufferPtr; /**< Pointer to a buffer */
  
  mutable std::mutex m_mutex; /**< mutex variable to prevent unattended access*/
};

#endif /* WEBCAM_V4L2_H */
