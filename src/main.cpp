/**
 * main.cpp
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
#include "window.h"

#include <thread>

#include <opencv2/opencv.hpp>

static const double CAPTURE_WIDTHS[] = {1280, 800, 640};
static const double CAPTURE_HEIGHTS[] = {720, 600, 360};

class Application : public altego::WindowDelegate, public altego::CaptureDelegate {
public:
  Application() : _capture(), _window("AltEGO"), _device(0), _sizeIdx(0) {
    _window.SetDelegate(this);
    _capture.SetDelegate(this);
    _capture.SetDevice(_device);
    _capture.SetSize(CAPTURE_WIDTHS[0], CAPTURE_HEIGHTS[0]);
  }

  void Run() {
    std::thread captureThread(&altego::Capture::Run, &_capture);
    _window.Run();
    _capture.Stop();
    captureThread.join();
  }

  void AltegoWindowKeyDown(altego::Window *window, altego::KeyType type) override {
    (void)window;
    switch (type) {
    case altego::KeySizeUp:
      _sizeIdx++;
      _capture.SetSize(CAPTURE_WIDTHS[_sizeIdx % 3], CAPTURE_HEIGHTS[_sizeIdx % 3]);
      break;
    case altego::KeySizeDown:
      _sizeIdx--;
      _capture.SetSize(CAPTURE_WIDTHS[_sizeIdx % 3], CAPTURE_HEIGHTS[_sizeIdx % 3]);
      break;
    case altego::KeyCameraPrev:
      if (_device > 0) {
        _device--;
        _capture.SetDevice(_device);
      }
      break;
    case altego::KeyCameraNext:
      _device++;
      _capture.SetDevice(_device);
      break;
    default:
      break;
    }
  }

  void AltegoCaptureDeviceOpened(altego::Capture *capture, int device) override {
    (void)capture;
    _window.SetDevice(device);
    _window.ClearImage();
  }

  void AltegoCaptureFrameRead(altego::Capture *capture, cv::Mat &im) override {
    (void)capture;
    _window.SetImage(im);
  }

  void AltegoCaptureFPSUpdated(altego::Capture *capture, double fps) override {
    (void)capture;
    _window.SetFPS(static_cast<int>(fps));
  }

private:
  altego::Capture _capture;
  altego::Window _window;
  int _device;
  int _sizeIdx;
};

int main() {
  Application application;
  application.Run();
}
