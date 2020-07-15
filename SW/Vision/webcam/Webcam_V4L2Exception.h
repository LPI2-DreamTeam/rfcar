/**
 * @file Webcam_V4L2Exception.h
 * @brief refines exceptions to provide better exception handling
 */
#ifndef WEBCAM_V4L2EXCEPTION_H
#define WEBCAM_V4L2EXCEPTION_H

#include <stdexcept>

class Webcam_V4L2Exception : public std::runtime_error
{
public:
  explicit Webcam_V4L2Exception(const std::string& errorMsg);
  explicit Webcam_V4L2Exception(const char* errorMsg);
  ~Webcam_V4L2Exception();
};

#endif /* WEBCAM_V4L2EXCEPTION_H */
