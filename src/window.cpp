/**
 * window.cpp
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

#include "window.h"

#include <opencv2/opencv.hpp>

#define HELP_STRING "ESC: Quit | W/S: Change Size | A/D: Switch Camera"

#define KEY_ESC 27

altego::Window::Window(const std::string &title) : _title(title), _help(HELP_STRING), _initialImage(600, 800, CV_8UC3, cv::Scalar(255, 0, 72)) {
  // calculate help size
  _helpSize = cv::getTextSize(_help, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, nullptr);
  // setup window
  cv::namedWindow(_title, cv::WINDOW_AUTOSIZE);
  // render initial image
  static const std::string empty = "E M P T Y";
  cv::Size size = cv::getTextSize(empty, cv::FONT_HERSHEY_SIMPLEX, 2, 2, nullptr);
  cv::putText(_initialImage, empty, cv::Point((_initialImage.cols - size.width) / 2, (_initialImage.rows + size.height) / 2), cv::FONT_HERSHEY_SIMPLEX, 2,
              cv::Scalar(255, 255, 255), 2);
  // show initial image
  ClearImage();
}

altego::Window::~Window() { cv::destroyWindow(_title); }

void altego::Window::SetDelegate(altego::WindowDelegate *delegate) { _delegate = delegate; }

void altego::Window::ClearImage() { SetImage(_initialImage); }

void altego::Window::SetImage(cv::Mat &im) {
  std::lock_guard<std::mutex> lock(_imMutex);
  im.copyTo(_im);
  _width = im.cols;
  _height = im.rows;
  _touched = true;
}

void altego::Window::renderTitle(cv::Mat &im) {
  cv::rectangle(im, cv::Point(0, 0), cv::Point(im.cols, _helpSize.height + 20), cv::Scalar(255, 99, 72), -1);
  cv::putText(im, _title, cv::Point(10, 10 + _helpSize.height), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
  cv::putText(im, _help, cv::Point(im.cols - _helpSize.width - 10, 10 + _helpSize.height), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
}

void altego::Window::renderStatus(cv::Mat &im) {
  _width = im.cols;
  _height = im.rows;
  cv::rectangle(im, cv::Point(0, im.rows - _helpSize.height - 20), cv::Point(im.cols, im.rows), cv::Scalar(255, 99, 72), -1);
  std::string s = "CAM: " + std::to_string(_device) + " | SIZE: " + std::to_string(_width) + "x" + std::to_string(_height) + " | FPS: " + std::to_string(_fps);
  cv::putText(im, s, cv::Point(10, im.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
}

void altego::Window::Run() {
  cv::Mat im;
  for (;;) {
    // re-render if needed
    if (_touched) {
      copyImage(im);
      renderTitle(im);
      renderStatus(im);
      cv::imshow(_title, im);
      _touched = false;
    }
    // wait key
    auto key = static_cast<char>(cv::waitKey(10));
    if (key == 0)
      continue;
    // handle key
    if (key == KEY_ESC) {
      break;
    } else {
      if (_delegate != nullptr) {
        switch (key) {
        case 'w':
        case 'W':
          _delegate->AltegoWindowKeyDown(this, KeySizeUp);
          break;
        case 's':
        case 'S':
          _delegate->AltegoWindowKeyDown(this, KeySizeDown);
          break;
        case 'a':
        case 'A':
          _delegate->AltegoWindowKeyDown(this, KeyCameraPrev);
          break;
        case 'd':
        case 'D':
          _delegate->AltegoWindowKeyDown(this, KeyCameraNext);
          break;
        default:
          _delegate->AltegoWindowKeyDown(this, KeyUnknown);
        }
      }
    }
  }
}

void altego::Window::copyImage(cv::Mat &im) {
  std::lock_guard<std::mutex> lock(_imMutex);
  _im.copyTo(im);
}

void altego::Window::SetDevice(int device) {
  _device = device;
  _touched = true;
}

void altego::Window::SetSize(int width, int height) {
  _width = width;
  _height = height;
  _touched = true;
}

void altego::Window::SetFPS(int fps) {
  _fps = fps;
  _touched = true;
}
