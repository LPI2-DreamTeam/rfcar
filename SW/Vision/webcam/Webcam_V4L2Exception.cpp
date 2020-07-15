/**
 * @file Webcam_V4L2Exception.h
 * @brief Implements refined exceptions to provide better exception handling
 */
#include "Webcam_V4L2Exception.h"

Webcam_V4L2Exception::Webcam_V4L2Exception(const std::string& errorMsg) : std::runtime_error(errorMsg)
{
}

Webcam_V4L2Exception::Webcam_V4L2Exception(const char* errorMsg) : std::runtime_error(errorMsg)
{
}

Webcam_V4L2Exception::~Webcam_V4L2Exception()
{
}
