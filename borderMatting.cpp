#include "borderMatting.h"
#include "global.h"
#include "bayesian.h"
#include "extract.h"

#define max(x, y) (x>y? x:y)
#define min(x, y) (x>y? y:x)


Mat trimap;



void setPixels(int var, int x, int y)
{
	switch(var)
	{
		case 0:
			trimap.at<Vec3b>(x, y)[0] = 255;
			trimap.at<Vec3b>(x, y)[1] = 255;
			trimap.at<Vec3b>(x, y)[2] = 255;
			break;
		case 1:
			trimap.at<Vec3b>(x, y)[0] = 0;
			trimap.at<Vec3b>(x, y)[1] = 0;
			trimap.at<Vec3b>(x, y)[2] = 0;
			break;
		case 2:
			trimap.at<Vec3b>(x, y)[0] = 255;
			trimap.at<Vec3b>(x, y)[1] = 255;
			trimap.at<Vec3b>(x, y)[2] = 255;
			break;
		case 3:
			trimap.at<Vec3b>(x, y)[0] = 0;
			trimap.at<Vec3b>(x, y)[1] = 0;
			trimap.at<Vec3b>(x, y)[2] = 0;
			break;
		default:
			printf("error, no such value ! \n");
			break;
	}
}

bool isSure(Mat mask, int i, int j)
{
	if(mask.at<uchar>(i, j) == 0)
		return true;
	if(mask.at<uchar>(i, j) == 1)
		return true;

	Rect roi;
	roi.x = max(i-radius, 0);
	roi.y = max(j-radius, 0);
	roi.width = min(2*radius, mask.cols - roi.x);
	roi.height = min(2*radius, mask.rows - roi.y);
	 
	Mat binMask;
	getBinMask(mask, binMask );
	Mat subs = binMask(roi);
	int e = subs.at<uchar>(0, 0);
	for (int i = 0; i < subs.rows; ++i)
	{
		for (int j = 0; j < subs.cols; ++j)
		{
			if(subs.at<uchar>(i, j) != e)
				return false;
		}	
	}
	return true;
}


void getTrimap(Mat mask)
{
	Mat binMask;
	getBinMask(mask, binMask );
	binMask = binMask * 255;
	
	
	Mat binMasks[3];
	binMask.copyTo(binMasks[0]);
	binMask.copyTo(binMasks[1]);
	binMask.copyTo(binMasks[2]);
	merge(binMasks, 3, trimap);

	Mat myEdges;
	//Sobel(trimap, trimap, CV_8U, 1, 0);
	Laplacian(binMask, myEdges, CV_8U);
	// imshow("edges imgae", myEdges);
	int nl = myEdges.rows;
	int nc = myEdges.cols;
	// cout << edges << endl;
	for (int i = 0; i < nl; ++i)
	{
		for (int j = 0; j < nc; ++j)
		{
			if(myEdges.at<uchar>(i, j) != 0 )
			{
				// printf("%d\n", myEdges.at<uchar>(i, j));
				circle(trimap, Point(j,i), 4, CV_RGB(128,128,128), CV_FILLED);
			}
		}	
	}
}


void initialSettings(Mat img)
{
    trimap = Mat(img.size(), CV_8UC3, cv::Scalar(128,128,128)); 
}


Mat borderMatting()
{
	initialSettings(foreImg);
	getTrimap(sepAPP.mask);
	bitwise_not(trimap, trimap);
    
    BayesianMatting bm(foreImg, trimap);
    Mat alpha = bm.solve();
    return alpha;
}


void maskMatting()
{
	Mat alpha = borderMatting();
	bitwise_not(alpha, alpha);

	// Mat alphaComp[3];
	// alpha.copyTo(alphaComp[0]);
	// alpha.copyTo(alphaComp[1]);
	// alpha.copyTo(alphaComp[2]);
	// Mat alphaImage;
	// merge(alphaComp, 3, alphaImage);

	// Mat showImage;
	// bitwise_and(foreImg, alphaImage, showImage);
	// imshow("alpha", showImage);

	alpha.copyTo(sepAPP.mask);
}



