#include "catface.hpp"

CatFace::CatFace (const std::string cascadePath) {
  catFaceCascade.load(cascadePath);
}

std::vector<cv::Rect> CatFace::detect(cv::Mat image) {
  cv::Mat grayImg;
  std::vector<cv::Rect> faces;

  cv::cvtColor(srcImg, grayImg, CV_BGR2GRAY);
  catFaceCascade.detectMultiScale(grayImg, faces, 1.3);

  return faces;
}

void CatFace::drawRectangles(cv::Mat image, std::vector<cv::Rect>) {
  for (int index = 0; index < faces.size(); index++) {
    std::cout << faces[index] << '\n';
    cv::rectangle(
      image,
      cvPoint(faces[index].x, faces[index].y),
      cvPoint(faces[index].x + faces[index].width, faces[index].y + faces[index].height),
      cv::Scalar(0, 0, 255),
      2
    );
  }
}
