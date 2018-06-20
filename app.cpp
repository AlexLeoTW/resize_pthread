#include <iostream>
#include <regex>
#include <ftw.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "lib/resize.hpp"
#include "lib/catface.hpp"

int callback(const char *fpath, const struct stat *sb, int typeflag);
void printHelp();

const char* targetPath;
int targetCols = 0;
int targetRows = 0;
int n_thread = 0;
CatFace catface("haarcascade_frontalcatface.xml");

int main(int argc, char const *argv[]) {

  if (argc < 4) {
    printHelp();
    return 0;
  }

  targetPath = argv[1];
  sscanf(argv[2], "%d", &targetCols);
  sscanf(argv[3], "%d", &targetRows);
  if (argc == 5) { sscanf(argv[4], "%d", &n_thread); } else { n_thread = 1; }

  ftw(targetPath, callback, 16);

  return 0;
}

// TODO: handle this more like a command
int callback(const char *fpath, const struct stat *sb, int typeflag) {
  if (typeflag == FTW_F && std::regex_match(std::string(fpath), std::regex(".+\\.(bmp|jpg|png)$"))) {

    std::cout << fpath << std::endl;

    char newPath[100];
    strcpy(newPath, fpath);

    strtok(newPath, "/");
    char* filename = strtok(NULL, "/");

    char outPath[100];
    sprintf(outPath, "output/%s", filename);

    cv::Mat image = cv::imread(fpath);
    cv::Mat srcImage = image.clone();
    if (n_thread > 1) {
      image = shrink(image, targetRows, targetCols, n_thread, true);
    } else {
      image = shrink(image, targetRows, targetCols, true);
    }

    std::vector<cv::Rect> faces = catface.detect(image);

    if (faces.size() != 0) {
      drawRectangles(srcImage, faces,
        srcImage.cols / targetCols > srcImage.rows / targetRows
        ? srcImage.cols / targetCols
        : srcImage.rows / targetRows
      );
      cv::imwrite(outPath, srcImage);
    }

    std::cout << " --> " << outPath << std::endl;
  }
  return 0;
}

void printHelp() {
  std::cout << "format:  ./app path cols rows [n_thread] \n";
  std::cout << "example: ./app pics 1920 1080 8  \n";
  std::cout << "example: ./app pics 1920 1080 \n";
}
