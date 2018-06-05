/**
 * capture.cpp
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

#include "capture.h"

#include <opencv2/videoio.hpp>
#include <thread>

altego::Capture::Capture() : _device(0), _width(800), _height(600), _stopMark(false), _delegate(nullptr) {}

void altego::Capture::SetDelegate(altego::CaptureDelegate *delegate) { _delegate = delegate; }

void altego::Capture::SetDevice(int device) { _device = device; }

void altego::Capture::SetSize(double width, double height) {
  _width = width;
  _height = height;
}

void altego::Capture::Run() {
  _stopMark = false;

  // device retry loop
  while (!_stopMark) {
    // device copied
    int device = _device;
    // device size, will be updated in inner loop
    double width = 0, height = 0;

    // variables declared
    cv::VideoCapture cap;
    cv::Mat im;
    // counter
    int count = 0;
    double t = 0;

    // wait 5s and retry if failed to open device
    if (!cap.open(device)) {
      std::this_thread::sleep_for(std::chrono::seconds(5));
      continue;
    }

    // notify device opened
    if (_delegate != nullptr)
      _delegate->AltegoCaptureDeviceOpened(this, device);

    // set camera FPS
    cap.set(cv::CAP_PROP_FPS, 30);

    // camera read loop
    while (!_stopMark) {
      // break inner loop immediately if device changed
      if (device != _device) {
        break;
      }
      // update width if changed
      if (width != _width) {
        width = _width;
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
      }
      // update height if changed
      if (height != _height) {
        height = _height;
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
      }

      // update fps
      if (count == 0)
        t = cv::getTickCount();

      // read frame
      if (!cap.read(im)) {
        // wait 5s and break inner loop if camera offline
        std::this_thread::sleep_for(std::chrono::seconds(5));
        break;
      }

      // notify frame read
      if (_delegate != nullptr)
        _delegate->AltegoCaptureFrameRead(this, im);

      // calculate FPS
      count++;
      if (count == 100) {
        t = (cv::getTickCount() - t) / cv::getTickFrequency();
        count = 0;
        if (_delegate != nullptr) {
          _delegate->AltegoCaptureFPSUpdated(this, 100.f / t);
        }
      }
    }
  }
}

void altego::Capture::Stop() { _stopMark = true; }
