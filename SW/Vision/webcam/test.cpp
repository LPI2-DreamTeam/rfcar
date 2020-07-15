
// Use your web cam under Linux with C++ and the uvcvideo driver
// 
// On two personal computers, I discovered the integrated web cam is actually a USB device with an interface to the uvcvideo driver. But how to use such a device in a C++ application? I found out the Video 4 Linux 2 API (V4L2) is used.
// An example in C++
// 
// Originally based on code provided by mike168m, I was able to capture a motion JPEG image and write is to a file on disk. Thanks for that! I improved the code and a first C++ interface emerged:




// With these files, we can create a small example applicating saving a snapshot to disk:


///------------------------------------------- C++ Source file -------------------------------------
/// @file       main.cc
/// @brief
/// @project    Webcam
/// @version    1
/// @date       January 30, 2018
///
///-------------------------------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>

#include "V4L2WebCam.h"
#include "V4L2WebCamException.h"

static const std::string DEVICE = "/dev/video0";

using namespace std;

int main(int argc, char* argv[])
{
  int returnCode = EXIT_SUCCESS;
  V4L2WebCam webcam(V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_MEMORY_MMAP);
  
  try
  {
    webcam.open(DEVICE);
    webcam.setFormat(640, 480, V4L2_PIX_FMT_MJPEG);
    webcam.setRequestBuffer(1U);
    webcam.startStream("output.png");
    webcam.close();
  }
  catch(const V4L2WebCamException& e)
  {
    std::cerr << e.what() << std::endl;
    returnCode = EXIT_FAILURE;
  }
  
  return returnCode;
}

// When your device is attached or connected to your Linux PC, a device
// 
// /dev/video0
// 
// should appear. When running this example application, this device is opened, captures an JPEG image and writes it to disk.
// 
// The code is available for download
// 
// Last update: Friday, February 2 2018 Contact
