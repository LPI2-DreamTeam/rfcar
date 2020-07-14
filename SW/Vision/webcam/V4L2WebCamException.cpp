///------------------------------------------- C++ Source file -------------------------------------
/// @file       V4L2WebCamException.cc
/// @brief      Implementation of an exception thrown by a web cam based on V4L2
/// @project    Webcam
/// @version    1
/// @date       January 30, 2018
///-------------------------------------------------------------------------------------------------
///
/// Module description
/// ------------------
/// @file
/// @verbatim
/// Global usage description:
/// -------------------------
/// Used for a device implementing the Video 4 Linux 2 API
///
/// @endverbatim
///-------------------------------------------------------------------------------------------------

#include "V4L2WebCamException.h"

V4L2WebCamException::V4L2WebCamException(const std::string& errorMsg) : std::runtime_error(errorMsg)
{
}

V4L2WebCamException::V4L2WebCamException(const char* errorMsg) : std::runtime_error(errorMsg)
{
}

V4L2WebCamException::~V4L2WebCamException()
{
}
