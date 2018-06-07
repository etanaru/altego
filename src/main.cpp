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

#include "algorithm.h"
#include "capture.h"
#include "result.h"
#include "server.h"
#include "window.h"

#include <fstream>
#include <pwd.h>
#include <thread>
#include <unistd.h>

using namespace altego;

static const double CAPTURE_WIDTHS[] = {1280, 800, 640};
static const double CAPTURE_HEIGHTS[] = {720, 600, 360};

class Application : public WindowDelegate, public CaptureDelegate {
public:
  Application() : _capture(), _window("AltEGO"), _device(0), _sizeIdx(0) {
    _window.SetDelegate(this);
    _capture.SetDelegate(this);
    _capture.SetDevice(_device);
    _capture.SetSize(CAPTURE_WIDTHS[0], CAPTURE_HEIGHTS[0]);
    _server.set_listening_ip("127.0.0.1");
    _server.set_listening_port(6699);
    _server.SetResultStore(&_resultStore);
  }

  void Run() {
    // determine model file
    const char *home = nullptr;
    if ((home = getenv("HOME")) == nullptr) {
      home = getpwuid(getuid())->pw_dir;
    }
    if (home == nullptr) {
      _window.ShowErrorAndExit("Failed to determine $HOME directory");
    }
    std::string modelFile = std::string(home) + "/.altego/shape_predictor_68_face_landmarks.dat";
    // load model file
    try {
      _algorithm.LoadModelFile(modelFile);
    } catch (std::exception &err) {
      _window.ShowErrorAndExit("Failed to load model: " + std::string(err.what()));
    }
    // start capture thread
    std::thread captureThread(&Capture::Run, &_capture);
    // start server async
    _server.start_async();
    // run the main loop
    _window.Run();
    // stop capture
    _capture.Stop();
    captureThread.join();
    // stop server
    //_server.clear();
    exit(EXIT_SUCCESS);
  }

  void AltegoWindowKeyDown(Window *window, KeyType type) override {
    (void)window;
    switch (type) {
    case KeySizeUp:
      _sizeIdx++;
      _capture.SetSize(CAPTURE_WIDTHS[_sizeIdx % 3], CAPTURE_HEIGHTS[_sizeIdx % 3]);
      break;
    case KeySizeDown:
      _sizeIdx--;
      _capture.SetSize(CAPTURE_WIDTHS[_sizeIdx % 3], CAPTURE_HEIGHTS[_sizeIdx % 3]);
      break;
    case KeyCameraPrev:
      if (_device > 0) {
        _device--;
        _capture.SetDevice(_device);
      }
      break;
    case KeyCameraNext:
      _device++;
      _capture.SetDevice(_device);
      break;
    default:
      break;
    }
  }

  void AltegoCaptureDeviceOpened(Capture *capture, int device) override {
    (void)capture;
    _window.SetDevice(device);
    _window.ClearImage();
  }

  void AltegoCaptureFrameRead(Capture *capture, cv::Mat &im) override {
    (void)capture;
    // resolve and annotate camera frame
    if (_algorithm.ResolveAndAnnotate(im, _result)) {
      _resultStore.Set(_result);
    }
    _window.SetImage(im);
  }

  void AltegoCaptureFPSUpdated(Capture *capture, double fps) override {
    (void)capture;
    _window.SetFPS(static_cast<int>(fps));
  }

private:
  ResultStore _resultStore;
  Result _result;
  Capture _capture;
  Window _window;
  Algorithm _algorithm;
  Server _server;
  int _device;
  int _sizeIdx;
};

int main() {
  Application application;
  application.Run();
}
