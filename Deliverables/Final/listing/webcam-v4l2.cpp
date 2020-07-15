/**
 * @file Webcam_V4L2.h
 * @brief Webcam wrapper implementation using V4L2 API
 */
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstring>
#include <fstream>

#include "Webcam_V4L2.h"
#include "Webcam_V4L2Exception.h"

static const int BUFFERSIZE = 80;

static const char* DeviceNotOpenStr = "The device is not open";
static const char* UnableToSetImageFormatStr = "Unable to set image format";
static const char* NoBufferFromDeviceStr = "Could not request buffer from device";
static const char* NoBufferInformationStr = "Device did not provide buffer information";
static const char* FailToOpenWebCamStr = "Failed to open the web cam";
static const char* FailToQueryCapabilitiesStr = "Failed to query web cam capabilities";
static const char* CouldNotStartVideoStreamStr = "Could not start video stream";
static const char* CouldNotStopVideoStreamStr = "Could not stop video stream";
static const char* CouldNotQueueBufferStr = "Could not queue buffer";
static const char* CouldNotDequeueBufferStr = "Could not dequeue buffer";

Webcam_V4L2::Webcam_V4L2(v4l2_buf_type type, v4l2_memory memory)
  : m_bufferType(type),
    m_memory(memory),
    m_fileDescriptor(-1),
    m_bufferPtr(nullptr)
{
  memset(&m_capability, 0x0, sizeof(v4l2_capability));
}

Webcam_V4L2::~Webcam_V4L2()
{
  close();
}

void Webcam_V4L2::open(const std::string& device)
{
  if (isOpen())
  {
    return;
  }
  
  throwIfError((-1 == access(device.c_str(), F_OK)), ENODEV, device);
  
  open(device.c_str());
  throwIfError((-1 == fileDescriptor()), errno, FailToOpenWebCamStr);
  
  throwIfError((-1 == ioctl(fileDescriptor(), VIDIOC_QUERYCAP, &m_capability)),
               errno, FailToQueryCapabilitiesStr);
}

void Webcam_V4L2::close() noexcept
{
  if (!isOpen())
  {
    return;
  }
  
  ::close(fileDescriptor());
  std::lock_guard lock(m_mutex);
  m_fileDescriptor = -1;
}

bool Webcam_V4L2::isOpen() const noexcept
{
  return (-1 != fileDescriptor());
}

void Webcam_V4L2::setFormat(uint32_t width, uint32_t height, uint32_t pixelformat, v4l2_field field)
{
  throwIfError((-1 == fileDescriptor()), EBADF, DeviceNotOpenStr);
  
  v4l2_format imageFormat = {0};
  imageFormat.type = m_bufferType;
  imageFormat.fmt.pix.width = width;
  imageFormat.fmt.pix.height = height;
  imageFormat.fmt.pix.pixelformat = pixelformat;
  imageFormat.fmt.pix.field = field;
 
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_S_FMT, &imageFormat),
               errno, UnableToSetImageFormatStr);
}

void Webcam_V4L2::setRequestBuffer(uint32_t numBuffers)
{
  throwIfError((-1 == fileDescriptor()), EBADF, DeviceNotOpenStr);
  
  v4l2_requestbuffers requestBuffer = {0};
  requestBuffer.count = numBuffers;
  requestBuffer.type = m_bufferType;
  requestBuffer.memory = m_memory;
  
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_REQBUFS, &requestBuffer),
               errno, NoBufferFromDeviceStr);
  
  allocateBuffer();
}

void Webcam_V4L2::allocateBuffer()
{
  throwIfError((-1 == fileDescriptor()), EBADF, DeviceNotOpenStr);
  
  v4l2_buffer queryBuffer = {0};
  queryBuffer.type = m_bufferType;
  queryBuffer.memory = m_memory;
  queryBuffer.index = 0U;

  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_QUERYBUF, &queryBuffer),
               errno, NoBufferInformationStr);  
      
  // use a pointer to point to the newly created buffer
  // mmap() will map the memory address of the device to
  // an address in memory
  m_bufferPtr = static_cast<char *> (mmap(NULL,
                                  queryBuffer.length,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  fileDescriptor(),
                                  queryBuffer.m.offset));
  
  memset(m_bufferPtr, 0x0, queryBuffer.length);  
}

void Webcam_V4L2::startStream(const std::string& file)
{
  throwIfError((-1 == fileDescriptor()), EBADF, DeviceNotOpenStr);
  
  v4l2_buffer bufferinfo;
  memset(&bufferinfo, 0x0, sizeof(v4l2_buffer));
  
  bufferinfo.type = m_bufferType;
  bufferinfo.memory = m_memory;
  bufferinfo.index = 0U;
  
  uint32_t requestedType = bufferinfo.type;
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_STREAMON, &requestedType),
               errno, CouldNotStartVideoStreamStr); 
  
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_QBUF, &bufferinfo),
               errno, CouldNotQueueBufferStr);
  
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_DQBUF, &bufferinfo),
               errno, CouldNotDequeueBufferStr);
  
  std::ofstream outFile;
  outFile.open(file.c_str(), std::ios::binary| std::ios::app);
  
  const int bufferSize = bufferinfo.bytesused;
  outFile.write(m_bufferPtr, bufferSize);

  v4l2_buf_type tmpType = m_bufferType;
  throwIfError(-1 == ioctl(fileDescriptor(), VIDIOC_STREAMOFF, &tmpType),
               errno, CouldNotStopVideoStreamStr);  

  outFile.close();
}

void Webcam_V4L2::throwIfError(bool errorOccured, int errNum, const std::string& errorMsg)
{
  if (!errorOccured)
  {
    return;
  }
  
  char buffer[BUFFERSIZE];
  memset(buffer, 0x0, sizeof(buffer));
  throw Webcam_V4L2Exception(errorMsg + ": " + strerror_r(errNum, buffer,
                                                          sizeof(buffer)));
}

int Webcam_V4L2::fileDescriptor() const noexcept
{
  std::lock_guard lock(m_mutex);
  return m_fileDescriptor;
}

void Webcam_V4L2::open(const char* device) noexcept
{
  std::lock_guard lock(m_mutex);
  m_fileDescriptor = ::open(device, O_RDWR);
}
