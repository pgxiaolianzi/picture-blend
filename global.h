#ifndef _LIAN_GLOBAL
#define _LIAN_GLOBAL


#include "opencv/highgui.h"
#include "opencv/cv.h"

#include <iostream>
#include <stdio.h>


using namespace std;
using namespace cv;

#define N 50

const Scalar GREEN = Scalar(0,255,0);
const Scalar RED = Scalar(0,0,255);
const Scalar BLUE = Scalar(255,0,0);

const Scalar PINK = Scalar(230,130,255);
const Scalar LIGHTBLUE = Scalar(255,255,160);

const int radius = 2;
const int thickness = -1;

extern Mat foreImg;
extern Mat backImg;



void getBinMask( const Mat& comMask, Mat& binMask );
void copySubsMat(Mat src, Mat& dest, int row, int col);
void copySubsMat(Mat src, Mat& dest, int row, int col, Mat mask);


#endif