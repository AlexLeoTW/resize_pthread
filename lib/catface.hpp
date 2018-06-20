#ifndef CAT_FACE
#define CAT_FACE

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class CatFace {
private:
  cv::CascadeClassifier catFaceCascade;

public:
  CatFace (std::string cascadePath);

  std::vector<cv::Rect> detect(cv::Mat image);

  virtual ~CatFace ();
};

void drawRectangles(cv::Mat image, std::vector<cv::Rect>, double factor=1);

#endif
