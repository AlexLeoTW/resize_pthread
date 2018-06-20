#include "resize.hpp"

cv::Mat shrink(cv::Mat &image, int rows, int cols, bool keepAspect/*=false*/) {
  // Blow up when not shrinking image
  if (image.rows < rows || image.cols < cols) { exit(1); }
  std::cout << "single thread" << '\n';
  cv::Mat resultImage(rows, cols, CV_8UC3);        // imread seems to return CV_8UC3 by default anyway...
  double rowFactor = image.rows / rows;
  double colFactor = image.cols / cols;
  if (keepAspect) {
    double commonFactor = rowFactor > colFactor ? rowFactor : colFactor;
    rowFactor = commonFactor;
    colFactor = commonFactor;
  }

  ShrinkArgs shrinkArgs = {
    .from        = Point2i(0, 0),
    .to          = Point2i(cols, rows),
    .rowFactor   = rowFactor,
    .colFactor   = colFactor,
    .image       = &image,
    .resultImage = &resultImage
  };

  shrinkPart((void*) &shrinkArgs);

  return resultImage;
}

cv::Mat shrink(cv::Mat &image, int rows, int cols, int n_thread, bool keepAspect/*=false*/) {
  // Blow up when not shrinking image
  if (image.rows < rows || image.cols < cols || image.rows < n_thread) { exit(1); }

  pthread_t* workers = (pthread_t*)calloc(n_thread, sizeof(pthread_t));
  ShrinkArgs* args =  (ShrinkArgs*)calloc(n_thread, sizeof(ShrinkArgs));
  cv::Mat resultImage(rows, cols, CV_8UC3);        // imread seems to return CV_8UC3 by default anyway...
  double rowFactor = image.rows / rows;
  double colFactor = image.cols / cols;
  int increment = rows % n_thread == 0
                ? (int)(rows / n_thread)
                : (int)(rows / n_thread + 1);

  if (keepAspect) {
    double commonFactor = rowFactor > colFactor ? rowFactor : colFactor;
    rowFactor = commonFactor;
    colFactor = commonFactor;
  }

  for (size_t nth = 0, boundary = 0; boundary < rows; nth++, boundary += increment) {
    args[nth].from        = Point2i(0, boundary);
    args[nth].to          = Point2i(cols, boundary+increment > rows ? rows : boundary+increment);
    args[nth].rowFactor   = rowFactor;
    args[nth].colFactor   = colFactor;
    args[nth].image       = &image;
    args[nth].resultImage = &resultImage;
    pthread_create(&workers[nth], NULL, shrinkPart, &args[nth]);
  }

  for (size_t nth = 0; nth < n_thread; nth++) {
    pthread_join(workers[nth], NULL);
  }
  free(args);

  return resultImage;
}

void *shrinkPart(void* _args) {
  ShrinkArgs* args = (ShrinkArgs*)_args;

  for (size_t row = args->from.y; row < args->to.y; row++) {
    for (size_t col = args->from.x; col < args->to.x; col++) {

      if (ceil(args->colFactor * (col + 1)) > args->image->cols ||
          ceil(args->rowFactor * (row + 1)) > args->image->rows) {
        args->resultImage->at<cv::Vec3b>(row, col) = (cv::Vec3b){0, 0, 0};
      } else {
        args->resultImage->at<cv::Vec3b>(row, col) = average(
          *args->image,
          Point2i( ceil(args->colFactor * col), ceil(args->rowFactor * row) ),
          Point2i( ceil(args->colFactor * (col + 1)), ceil(args->rowFactor * (row + 1)) )
        );
      }
    }
  }
}

cv::Vec3b average(cv::Mat &image, Point2i fromPixel, Point2i toPixel) {

  cv::Vec3i result(0, 0, 0);
  Point2i pixelDiff = fromPixel - toPixel;
  int pixelCount = pixelDiff.x * pixelDiff.y;

  for (size_t row = fromPixel.y; row < toPixel.y; row++) {
    for (size_t col = fromPixel.x; col < toPixel.x; col++) {
      result += image.at<cv::Vec3b>(row, col);
    }
  }
  result = (cv::Vec3b)(result / pixelCount);

  return result;
}
