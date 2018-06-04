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

#include "window.h"

#include <opencv2/opencv.hpp>

class DemoWindowDelegate : public altego::WindowDelegate {
public:
  void AltegoWindowKeyDown(altego::Window *window, altego::KeyType type) override {
    (void)window;
    switch (type) {
    case altego::KeySizeUp:
      std::cout << "Size UP" << std::endl;
      break;
    case altego::KeySizeDown:
      std::cout << "Size Down" << std::endl;
      break;
    case altego::KeyCameraPrev:
      std::cout << "Camera Prev" << std::endl;
      break;
    case altego::KeyCameraNext:
      std::cout << "Camera Next" << std::endl;
      break;
    default:
      break;
    }
  }
};

int main() {
  DemoWindowDelegate demoDelegate;
  altego::Window window("AltEGO");
  window.SetDelegate(&demoDelegate);
  window.Run();
}
