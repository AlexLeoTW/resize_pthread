#include "catface.hpp"

CatFace::CatFace (const std::string cascadePath) {
  catFaceCascade.load(cascadePath);
}

std::vector<cv::Rect> CatFace::detect(cv::Mat image) {
  cv::Mat grayImg;
  std::vector<cv::Rect> faces;

  cv::cvtColor(image, grayImg, CV_BGR2GRAY);
  catFaceCascade.detectMultiScale(grayImg, faces, 1.3);

  return faces;
}

void drawRectangles(cv::Mat image, std::vector<cv::Rect> faces, double factor/*=1*/) {
  int start_x, end_x, start_y, end_y;
  for (int index = 0; index < faces.size(); index++) {

    if (factor != 1) {
      start_x = faces[index].x * factor;
      start_y = faces[index].y * factor;
      end_x   = (faces[index].x + faces[index].width) * factor;
      end_y   = (faces[index].y + faces[index].height) * factor;
    } else {
      start_x = faces[index].x;
      start_y = faces[index].y;
      end_x   = faces[index].x + faces[index].width;
      end_y   = faces[index].y + faces[index].height;
    }

    std::cout << faces[index] << '\n';
    cv::rectangle(
      image,
      cvPoint(start_x, start_y),
      cvPoint(end_x, end_y),
      cv::Scalar(0, 0, 255),
      2
    );
  }
}

CatFace::~CatFace () {}
