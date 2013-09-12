#include "global.h"

Mat foreImg;
Mat backImg;


void getBinMask( const Mat& comMask, Mat& binMask )
{
    if( comMask.empty() || comMask.type()!=CV_8UC1 )
        CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
    if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols )
        binMask.create( comMask.size(), CV_8UC1 );
    binMask = comMask & 1;  //得到mask的最低位,实际上是只保留确定的或者有可能的前景点当做mask
}


void copySubsMat(Mat src, Mat& dest, int row, int col)
{
	int nl = src.rows;
	int nc = src.cols;
	for (int i = 0; i < nl; ++i)
    {
        for (int j = 0; j < nl; ++j)
        {
            for (int k = 0; k < 3; k++)
                dest.at<Vec3b>(row+i, col+j)[k] = src.at<Vec3b>(i, j)[k];
        }
    }
}


void copySubsMat(Mat src, Mat& dest, int row, int col, Mat mask)
{
	int nl = src.rows;
	int nc = src.cols;
	for (int i = 0; i < nl; ++i)
    {
        for (int j = 0; j < nl; ++j)
        {
            for (int k = 0; k < 3; k++)
            	if(mask.at<uchar>(i, j) != 0)
                	dest.at<Vec3b>(row+i, col+j)[k] = src.at<Vec3b>(i, j)[k];
        }
    }
}