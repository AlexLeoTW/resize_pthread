#ifndef RESIZE_HPP_
#define RESIZE_HPP_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef cv::Point_<int> Point2i;
typedef struct _shrinkPartArgs {
  Point2i from;
  Point2i to;
  double rowFactor;
  double colFactor;
  cv::Mat* image;
  cv::Mat* resultImage;
} ShrinkArgs;

cv::Mat shrink(cv::Mat &image, int rows, int cols, bool keepAspect=false);
cv::Mat shrink(cv::Mat &image, int rows, int cols, int n_thread, bool keepAspect=false);
cv::Vec3b average(cv::Mat &image, Point2i fromPixel, Point2i toPixel);
void *shrinkPart(void* _args);

#endif /* RESIZE_HPP_ */
