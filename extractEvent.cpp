#include "extractEvent.h"
#include "compose.h"
#include "extract.h"
#include "borderMatting.h"



void setRect(Rect& r, int x, int y)
{
    r.width = x - r.x;
    r.height = y - r.y;
    if(r.width < 0)
        r.x = x;
    if(r.height < 0)
        r.y = y;
    r.width = abs(r.width);
    r.height = abs(r.height);
}


void* extractImage(void* args)
{
    char winName[N] = "image"; 
    sepAPP.init(winName, foreImg);
    namedWindow( winName, CV_WINDOW_AUTOSIZE );
    setMouseCallback( winName, on_mouse, 0 );

    bool exitFlag = false;
    while(!exitFlag)
    {
        sepAPP.showImage();
        char c = (char)cvWaitKey(0);
        keyPressed(c, exitFlag);
    }
    destroyWindow( winName );
    return 0;
}


void keyPressed(char c, bool& exitFlag)
{
	switch(c)
    {
        case 'q':
            printf("quit application...\n");
            exitFlag = true;
            break;

        case 'z':
            printf("undo ...\n");
            sepAPP.reset();
            break;

        case 'y':
            printf("applying the change ...\n");
            sepAPP.apply();
            sepAPP.showImage();
            printf("having done the the change ...\n");
            break;

        case 'f':
            printf("switching select mode to foreground  ...\n");
            sepAPP.foreStateOn = true;
            sepAPP.backStateOn = false;
            break;

        case 'b':
            if(!sepAPP.rectSet)
            {
                printf("first you should specify the object window\n");
                printf("so switching select mode to background failed ...\n");
            }
            else
            {
                printf("switching select mode to background ...\n");
                sepAPP.backStateOn = true;
                sepAPP.foreStateOn = false;
            }
            break;

        case 'l':
            if(sepAPP.foreStateOn)
                printf("in foreground mode ...\n");
            else
                printf("in background mode ...\n");
            break;

        case 's':
            printf("saving the image ...\n");
            sepAPP.saveImage();
            break;

        case 'c':
            printf("composing the image ...\n");
            composeImage(0);
            break;

        case 'm':
            printf("matting the image border ...\n");
            maskMatting();
            sepAPP.showImage();
            break;

        default:
            break;
    }
}



void on_mouse( int event, int x, int y, int flags, void* param )
{
    sepAPP.rectTemp.x = sepAPP.x1;
    sepAPP.rectTemp.y = sepAPP.y1;
    switch( event )
    {
        // surely pixels
        case CV_EVENT_LBUTTONDOWN: 
            printf("left button down ...\n");
            if(sepAPP.rectDef)
                sepAPP.rectSet = true;
            else
                sepAPP.rectDef = true;
            sepAPP.x1 = x;
            sepAPP.y1 = y;
            break;

        case CV_EVENT_LBUTTONUP:
            printf("left button up ...\n");
            if(!sepAPP.rectSet)
            {
                setRect(sepAPP.rectTemp, x, y);
                sepAPP.rect = sepAPP.rectTemp;
                sepAPP.setRectMask(sepAPP.rect);
                sepAPP.rectSet = true;
            }  
            else
            {
                if(sepAPP.backStateOn)
                    sepAPP.backPixels.push_back(Point(x, y));
                if(sepAPP.foreStateOn)
                    sepAPP.forePixels.push_back(Point(x, y));

            }
            sepAPP.showImage();
            break;

        case CV_EVENT_MOUSEMOVE:
            if(flags == 33 || flags == 34) 
            {
                if(!sepAPP.rectSet)
                {
                    setRect(sepAPP.rectTemp, x, y);
                    sepAPP.rect = sepAPP.rectTemp; 
                }  
                else
                {
                    if(sepAPP.backStateOn)
                        sepAPP.backPixels.push_back(Point(x, y));
                    if(sepAPP.foreStateOn)
                        sepAPP.forePixels.push_back(Point(x, y));
                }
                sepAPP.showImage();
            }  
            break;
    }
}