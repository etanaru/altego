/**
 * algorithm.cpp
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

#include <dlib/opencv.h>
#include <opencv2/imgproc.hpp>

// down sample ratio
#define DSRATIO 4

bool _compareRectangleArea(dlib::rectangle lhs, dlib::rectangle rhs) { return lhs.area() < rhs.area(); }

altego::Algorithm::Algorithm() { _detector = dlib::get_frontal_face_detector(); }

bool altego::Algorithm::ResolveAndAnnotate(cv::Mat &im, altego::Result &res) {
  // down sample for face detection
  cv::Mat imSmall;
  cv::resize(im, imSmall, cv::Size(), 1.0 / DSRATIO, 1.0 / DSRATIO);
  // convert type with zero copy
  dlib::cv_image<dlib::bgr_pixel> dim(im);
  dlib::cv_image<dlib::bgr_pixel> dimSmall(imSmall);
  // detect faces
  auto faces = _detector(dimSmall);
  if (faces.empty())
    return false;
  // find largest face
  auto face = *std::max_element(faces.begin(), faces.end(), _compareRectangleArea).base();
  // upscale
  face.left() *= DSRATIO;
  face.top() *= DSRATIO;
  face.right() *= DSRATIO;
  face.bottom() *= DSRATIO;
  // detection
  auto det = _predictor(dim, face);
  // check num_parts()
  if (det.num_parts() != 68)
    return false;
  // draw detection
  for (size_t i = 0; i < det.num_parts(); i++) {
    dlib::point p = det.part(i);
    // check part valid
    if (p == dlib::OBJECT_PART_NOT_PRESENT)
      return false;
    cv::circle(im, cv::Point(static_cast<int>(p.x()), static_cast<int>(p.y())), 2, cv::Scalar(255, 0, 72), -1);
  }
  // parse full object detection
  return true;
}

void altego::Algorithm::LoadModelFile(std::string &modelFile) { dlib::deserialize(modelFile) >> _predictor; }
