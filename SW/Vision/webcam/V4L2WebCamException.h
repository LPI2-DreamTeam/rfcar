///------------------------------------------- C++ Header file -------------------------------------
/// @file       V4L2WebCamException.h
/// @brief      Definition of an exception thrown by a web cam based on V4L2
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
/// See V4L2WebCamException.cc
///
/// @endverbatim
///-------------------------------------------------------------------------------------------------

#ifndef V4L2WEBCAMEXCEPTION_H
#define V4L2WEBCAMEXCEPTION_H

#include <stdexcept>

class V4L2WebCamException : public std::runtime_error
{
public:
  explicit V4L2WebCamException(const std::string& errorMsg);
  explicit V4L2WebCamException(const char* errorMsg);
  ~V4L2WebCamException();
};

#endif /* V4L2WEBCAMEXCEPTION_H */
