#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void darkChannel(Mat &img, Mat &darkdst) {
  Mat bands[3];
  split(img, bands);
  Size a(15, 15);
  Mat dc = min(min(bands[0], bands[1]), bands[2]);
  Mat kernel = getStructuringElement(MORPH_RECT, a);
  erode(dc, darkdst, kernel);
}

void AtmLight(Mat &img, Mat &Adst, Mat &dark) {}

int main(int argc, char **argv) {
  Mat img = imread("low.png");
  bitwise_not(img, img);
  Mat darkdst, A;
  darkChannel(img, darkdst, 15);
  namedWindow("Display Image OG", WINDOW_AUTOSIZE);
  imshow("Display Image OG", img);
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", darkdst);
  waitKey(0);
  return 0;
}