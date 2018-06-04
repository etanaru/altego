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

#define KEY_ESC 27

#define HELP_TEXT "ESC: Quit | Arrow Up/Down: Change Size | Arrow Left/Right: Switch Camera"

altego::Window::Window(const std::string &title) : _initialImage(600, 800, CV_8UC3, cv::Scalar(255, 0, 72)) {
  _title = title;
  _helpSize = cv::getTextSize(HELP_TEXT, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, nullptr);
  // setup window
  cv::namedWindow(_title, cv::WINDOW_AUTOSIZE);
  // render initial image
  static const std::string empty = "E M P T Y";
  cv::Size size = cv::getTextSize(empty, cv::FONT_HERSHEY_SIMPLEX, 2, 2, nullptr);
  cv::putText(_initialImage, empty, cv::Point((_initialImage.cols - size.width) / 2, (_initialImage.rows + size.height) / 2), cv::FONT_HERSHEY_SIMPLEX, 2,
              cv::Scalar(255, 255, 255), 2);
  // show initial image
  ShowInitialImage();
}

void altego::Window::ShowInitialImage() { Show(_initialImage); }

void altego::Window::Show(cv::Mat &im) {
  PutTitle(im);
  PutStatus(im);
  cv::imshow(_title, im);
}

void altego::Window::PutTitle(cv::Mat &im) {
  cv::rectangle(im, cv::Point(0, 0), cv::Point(im.cols, _helpSize.height + 20), cv::Scalar(255, 99, 72), -1);
  cv::putText(im, _title, cv::Point(10, 10 + _helpSize.height), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
  cv::putText(im, HELP_TEXT, cv::Point(im.cols - _helpSize.width - 10, 10 + _helpSize.height), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
}

void altego::Window::PutStatus(cv::Mat &im) {
  _width = im.cols;
  _height = im.rows;
  cv::rectangle(im, cv::Point(0, im.rows - _helpSize.height - 20), cv::Point(im.cols, im.rows), cv::Scalar(255, 99, 72), -1);
  std::string s = "CAM: " + std::to_string(_cam) + " | SIZE: " + std::to_string(_width) + "x" + std::to_string(_height) + " | FPS: " + std::to_string(_fps);
  cv::putText(im, s, cv::Point(10, im.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
}

void altego::Window::Run() {
  for (;;) {
    auto key = static_cast<char>(cv::waitKey(10));
    if (key == KEY_ESC) {
      break;
    }
  }
}
