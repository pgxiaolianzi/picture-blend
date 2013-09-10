#ifndef _LIAN_EXTRACT
#define _LIAN_EXTRACT

#include "global.h"


struct Extract
{
    bool initial;
    bool backStateOn;
    bool foreStateOn;
    bool possibleStateOn;
    bool rectSet;
    bool rectDef;

    char winName[N];

    Mat image;
    Mat mask;
    Mat bgdModel, fgdModel;
    vector<Point> backPixels;
    vector<Point> forePixels;
    Rect rect;

    int x1, y1;
    Rect rectTemp; 

    Extract();
    Extract(char *windowName, Mat img);
    void init(char *windowName, Mat img);
    void showImage();
    void copyPixel(bool surely);
    void specifyObject();
    void apply();
    void reset();
    void setMask();
    void setRectMask(Rect r);
    void saveImage();

    // friend void on_mouse( int event, int x, int y, int flags, void* param );
};

extern Extract sepAPP;


#endif