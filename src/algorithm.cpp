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
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

// down sample ratio
#define DSRATIO 4

class Detection {
public:
  Detection(dlib::full_object_detection *rawDet) : _rawDet(rawDet) {}

  cv::Point2d operator[](size_t index) { return cv::Point2d(_rawDet->part(index).x(), _rawDet->part(index).y()); }

private:
  dlib::full_object_detection *_rawDet;
};

bool _cameraPointsConsideredSame(std::vector<cv::Point2d> &last, std::vector<cv::Point2d> &current) {
  if (last.empty() || (last.size() != current.size()))
    return false;
  double base = cv::norm(current[0] - current[1]);
  if (base == 0)
    return false;
  double total = 0;
  for (size_t i = 0; i < last.size(); i++) {
    total += cv::norm(current[i] - last[i]);
  }
  return (total / base) < 0.1;
}

bool _compareRectangleArea(dlib::rectangle &lhs, dlib::rectangle &rhs) { return lhs.area() < rhs.area(); }

altego::Algorithm::Algorithm() {
  // initialize detector
  _detector = dlib::get_frontal_face_detector();

  // initialize reference points
  // The first must be (0,0,0) while using POSIT
  _referencePoints.emplace_back(0.0f, 0.0f, 0.0f);          // 30
  _referencePoints.emplace_back(0.0f, -330.0f, -65.0f);     // 8
  _referencePoints.emplace_back(-225.0f, 170.0f, -135.0f);  // 36
  _referencePoints.emplace_back(225.0f, 170.0f, -135.0f);   // 45
  _referencePoints.emplace_back(-150.0f, -150.0f, -125.0f); // 48
  _referencePoints.emplace_back(150.0f, -150.0f, -125.0f);  // 54

  // initialize distCoeffs
  _distCoeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);
}

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
  auto rawDet = _predictor(dim, face);
  // check num_parts()
  if (rawDet.num_parts() != 68)
    return false;
  // draw detection
  for (size_t i = 0; i < rawDet.num_parts(); i++) {
    dlib::point p = rawDet.part(i);
    // check part valid
    if (p == dlib::OBJECT_PART_NOT_PRESENT)
      return false;
    cv::circle(im, cv::Point(static_cast<int>(p.x()), static_cast<int>(p.y())), 2, cv::Scalar(255, 0, 72), -1);
  }

  // wrap dlib::full_object_detection
  Detection det(&rawDet);

  // cameraPoints
  std::vector<cv::Point2d> cp;
  // nose tip
  cp.push_back(det[30]);
  // chin
  cp.push_back(det[8]);
  // left eye left corner
  cp.push_back(det[36]);
  // right eye right corner
  cp.push_back(det[45]);
  // left Mouth corner
  cp.push_back(det[48]);
  // right mouth corner
  cp.push_back(det[54]);

  // stabilize
  if (_cameraPointsConsideredSame(_lastCameraPoints, cp))
    return false;

  // update lastCameraPoints
  _lastCameraPoints = cp;

  // camera matrix
  cv::Mat_<double> cameraMatrix(3, 3);
  cameraMatrix << im.cols, 0, im.cols / 2.f, 0, im.cols, im.rows / 2.f, 0, 0, 1;

  // rotation vector, translation vector
  cv::Mat rv, tv;

  // solve
  cv::solvePnP(_referencePoints, cp, cameraMatrix, _distCoeffs, rv, tv);

  // set rv, tv to result
  res.r1 = rv.at<double>(0, 1);
  res.r2 = rv.at<double>(0, 2);

  return true;
}

void altego::Algorithm::LoadModelFile(std::string &modelFile) { dlib::deserialize(modelFile) >> _predictor; }
