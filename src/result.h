/**
 * result.h
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

#ifndef __ALTEGO_RESULT_H__
#define __ALTEGO_RESULT_H__

#include <iostream>

#include <dlib/threads.h>
#include <opencv2/core.hpp>

#include "store.h"

namespace altego {
class Result {
public:
  // camera size
  double w = 0;
  double h = 0;
  // translation vector
  double t0 = 0;
  double t1 = 0;
  double t2 = 0;
  // rotation vector
  double r0 = 0;
  double r1 = 0;
  double r2 = 0;
  // eye open size
  double le = 0;
  double re = 0;
  // eyebrow height
  double lb = 0;
  double rb = 0;
  // mouth width/height
  double mw = 0;
  double mh = 0;

  // nose length as base
  double base = 0;

  // serialize result to stream
  void Serialize(std::ostream &out);
};

// ResultStore with wait and broadcast
using ResultStore = Store<Result>;

} // namespace altego

#endif