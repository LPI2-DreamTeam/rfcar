/**
 * @file webcam-main.cpp
 * @brief Driver to test Webcam V42L wrapper
 */
#include <iostream>
#include <cstdlib>

#include "Webcam_V4L2.h"
#include "Webcam_V4L2Exception.h"


using namespace std;
static const string DEVICE = "/dev/video0"; /**< device location */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
  /* Create webcam for video capture */
  Webcam_V4L2 wbc(V4L2_BUF_TYPE_VIDEO_CAPTURE, V4L2_MEMORY_MMAP);
  
  try{
/* Open the device */
    wbc.open(DEVICE);
/* Set format to 320x240 and Motion JPEG */
    wbc.setFormat(320, 240, V4L2_PIX_FMT_MJPEG);
/* Request 1 buffer */
    wbc.setRequestBuffer(1U);
/* Start stream and write to file */
    wbc.startStream("output.jpg");
/* Close device */
    wbc.close();
  }catch(const Webcam_V4L2Exception& e){
    std::cerr << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  
  return retCode;
}
