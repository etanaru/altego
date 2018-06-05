/**
 * capture.h
 *
 * MIT License
 *
 * Copyright (c) 2018 LandZERO
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ALTEGO_CAPTURE_H__
#define __ALTEGO_CAPTURE_H__

#include <opencv2/core.hpp>

namespace altego {
class Capture;

class CaptureDelegate {
public:
  virtual void AltegoCaptureDeviceOpened(Capture *capture, int device) = 0;

  virtual void AltegoCaptureFrameRead(Capture *capture, cv::Mat &im) = 0;

  virtual void AltegoCaptureFPSUpdated(Capture *capture, double fps) = 0;
};

class Capture {
public:
  Capture();

  void SetDelegate(CaptureDelegate *delegate);

  void SetDevice(int device);

  void SetSize(double width, double height);

  void Run();

  void Stop();

private:
  int _device;
  double _width, _height;
  bool _stopMark;
  CaptureDelegate *_delegate;
};
} // namespace altego

#endif //__ALTEGO_CAPTURE_H__
