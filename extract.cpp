#include "extract.h"

Extract sepAPP;


Extract::Extract()
{
    initial = false;
    backStateOn = false;
    foreStateOn = true;
    possibleStateOn = false;
    rectSet = false;
    rectDef = false;

    rect.x = 0;
    rect.y = 0;
    rect.width = 0;
    rect.height = 0;
}


Extract::Extract(char *windowName, Mat img)
{
    Extract();
    init(windowName, img);
}


void Extract::init(char *windowName, Mat img)
{
    image = img.clone();
    strcpy(winName, windowName); 
    if(mask.empty())
    {
        mask.create(image.rows, image.cols, CV_8UC1);
        mask.setTo(Scalar::all(GC_BGD));
    }
}


void Extract::reset()
{
    initial = false;
    backStateOn = false;
    foreStateOn = true;
    possibleStateOn = false;
    rectSet = false;
    rectDef =  false;

    mask.setTo(Scalar::all(GC_BGD));
    forePixels.clear();
    backPixels.clear();
}


void Extract::showImage()
{
    if( image.empty())
        return;
    Mat res;
    Mat binMask;
    if( !initial )
        image.copyTo( res );
    else
    {
        getBinMask( mask, binMask );
        image.copyTo(res, binMask );  
        Mat mask2 = \
        addWeighted()
        bitwise_and(binMask, alphaImage, showImage);

        // for (int i = 0; i < image.rows; ++i)
        // {
        //     for (int j = 0; j < image.cols; ++j)
        //     {
        //         if(mask.at<uchar>(i, j) == GC_PR_FGD) //possible foreground
        //         {
                        //addWeighted(image, )
        //             for (int k = 0; k < 3; k++)
        //                 res.at<Vec3b>(i, j)[k] = image.at<Vec3b>(i, j)[k]*0.5 + PINK[k]*0.5;
        //         }
        //         if(mask.at<uchar>(i, j) == GC_PR_BGD) //possibel background
        //         {
        //             for (int k = 0; k < 3; k++)
        //                 res.at<Vec3b>(i, j)[k] = image.at<Vec3b>(i, j)[k]*0.5 + LIGHTBLUE[k]*0.5;
        //         }
        //     }
        // } 
    }
    

    int fsize = forePixels.size();
    for (int i = 0; i < fsize; ++i)
        circle( res, forePixels[i], radius, RED, thickness );
    int bsize = backPixels.size();
    for (int i = 0; i < bsize; ++i)
        circle( res, backPixels[i], radius, BLUE, thickness );
    if(rectSet || rectDef)
    {
        //object window
        Point p1(rect.x, rect.y);
        Point p2(rect.width + p1.x, rect.height + p1.y);
        rectangle(res, p1, p2, GREEN, 2);
    }
    imshow( winName, res);
}

void Extract::saveImage()
{
    if( image.empty())
        return;

    Mat res;
    Mat binMask;
    if( !initial )
        image.copyTo( res );
    else
    {
        getBinMask( mask, binMask );
        image.copyTo( res, binMask );  
    }
    imwrite("./out.jpg", res);
    imwrite("./mask.jpg", mask);
    // namedWindow("1");
    // imshow("1", res);
    // waitKey(0);
}


void Extract::setMask()
{
    printf("excute mask ...\n");
    //foreground pixels
    int fsize = forePixels.size();
    for (int i = 0; i < fsize; ++i)
        circle( mask, forePixels[i], radius, GC_FGD, thickness);
    //background pixels
    int bsize = backPixels.size();
    for (int i = 0; i < bsize; ++i)
        circle( mask, backPixels[i], radius, GC_BGD, thickness);
}

void Extract::setRectMask(Rect r)
{
    (mask(rect)).setTo( Scalar(GC_PR_FGD) );
}


void Extract::apply()
{
    if(initial)
    {
        setMask(); 
        grabCut( image, mask, rect, bgdModel, fgdModel, 1 );  
        forePixels.clear();
        backPixels.clear(); 
    }
    else
    {
        initial = true;
        if(forePixels.empty() && backPixels.empty())
            grabCut( image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT );
        else
        {
            setMask(); 
            grabCut( image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK );
            forePixels.clear();
            backPixels.clear(); 
        }    
    }   
}