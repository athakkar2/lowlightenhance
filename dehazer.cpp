#include <math.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

vector<int> argsort(const vector<float> &array) {
  vector<int> indices(array.size());
  iota(indices.begin(), indices.end(), 0);
  sort(indices.begin(), indices.end(), [&array](int left, int right) -> bool {
    // sort indices according to corresponding array element
    return array[left] < array[right];
  });

  return indices;
}

void darkChannel(Mat &img, Mat &darkdst) {
  Mat bands[3];
  split(img, bands);
  Size a(15, 15);
  Mat dc = min(min(bands[0], bands[1]), bands[2]);
  Mat kernel = getStructuringElement(MORPH_RECT, a);
  erode(dc, darkdst, kernel);
}

void AtmLight(Mat &img, float A[], Mat &dark) {
  int imgsz = img.rows * img.cols;
  int numpx = int(max(floor(imgsz / 1000), 1.0));
  Mat darkvec, imvec;
  darkvec = dark.reshape(0, imgsz);
  imvec = img.reshape(3, imgsz);
  darkvec.convertTo(darkvec, CV_32FC1);
  vector<float> darkvec1(darkvec.rows * darkvec.cols * darkvec.channels());
  if (darkvec.isContinuous()) {
    darkvec1.assign(darkvec.data,
                    darkvec.data + darkvec.total() * darkvec.channels());
  }
  vector<int> indices;
  indices = argsort(darkvec1);
  auto iterator = indices.begin();
  indices.erase(iterator, iterator + imgsz - numpx);

  float atmsum[3] = { };
  for(int i = 1; i < numpx; i++){
    for(int j = 0; j < 3; j++){
      atmsum[j] = atmsum[j] + imvec.at<Vec3b>(indices[i])[j];
    }
  }

  for (int z = 0; z < 3; z++){A[z] = atmsum[z] / numpx;}
}

int main(int argc, char **argv) {
  Mat img = imread("Low.png");
  bitwise_not(img, img);
  Mat darkdst;
  float A[3] = {};
  darkChannel(img, darkdst);
  AtmLight(img, A, darkdst);
  namedWindow("Display Image OG", WINDOW_AUTOSIZE);
  imshow("Display Image OG", img);
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", darkdst);
  waitKey(0);
  return 0;
}