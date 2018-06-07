/**
 * window.h
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

#ifndef __ALTEGO_WINDOW_H__
#define __ALTEGO_WINDOW_H__

#include <mutex>
#include <opencv2/core.hpp>

namespace altego {

typedef enum {
  KeyUnknown,
  KeySizeUp,
  KeySizeDown,
  KeyCameraPrev,
  KeyCameraNext,
} KeyType;

class Window;

class WindowDelegate {
public:
  virtual void AltegoWindowKeyDown(Window *window, KeyType type) = 0;
};

/**
 * Window
 *
 * main window of altego
 */
class Window {
public:
  explicit Window(const std::string &title);

  ~Window();

  void SetDelegate(WindowDelegate *delegate);

  void ClearImage();

  void SetImage(cv::Mat &im);

  void SetDevice(int device);

  void SetSize(int width, int height);

  void SetFPS(int fps);

  void ShowErrorAndExit(const std::string &error);

  void Run();

private:
  // title string
  const std::string _title;
  // help string
  const std::string _help;
  // calculated help string size
  cv::Size _helpSize;
  // initial image
  cv::Mat _initialImage;
  // current image
  cv::Mat _im;
  // lock for current image
  std::mutex _imMutex;
  // information
  int _device = 0, _width = 0, _height = 0, _fps = 0;
  // mark for re-render
  bool _touched = false;
  // delegate
  WindowDelegate *_delegate = nullptr;

  void copyImage(cv::Mat &im);

  void renderTitle(cv::Mat &im);

  void renderStatus(cv::Mat &im);
};
} // namespace altego

#endif // __ALTEGO_WINDOW_H__
