//============================================================================
// Name        : image-processing.cpp
// Author      : ITM13
// Version     :
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  Mat im = imread(argc == 2 ? argv[1] : "lena.png", 1);
  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }

  imshow("image", im);
  waitKey(0);

  return 0;
}
