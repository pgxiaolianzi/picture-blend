#include "compose.h"
#include "global.h"
#include "extract.h"

int x = 0;
int y = 0;
double alpha = 1;
double delta = 0.1;
bool saveFlag = false;
//double shrink = 1.2;


void showCompImage(Mat& compImg)
{
    Mat binMask;
    getBinMask(sepAPP.mask, binMask);
    int newR = foreImg.rows * alpha;
    int newC = foreImg.cols * alpha;

    Mat selfFore = foreImg.clone();
    resize(binMask, binMask, Size(newC, newR));
    resize(selfFore, selfFore, Size(newC, newR));
    Mat parentFore;
    selfFore.copyTo(parentFore, binMask);
    backImg.copyTo(compImg);
    copySubsMat(selfFore, compImg, x, y, binMask);
}


void composeKeyPressed(char c, bool& exitFlag)
{
    switch(c)
    {
        case 'q':
            printf("quit compose program...\n");
            exitFlag = true;
            break;
        case 'a':
            printf("move left ...\n");
            if(y >= 10)
                y -= 10;
            break;
        case 'd':
            printf("move right ...\n");
            if(y < backImg.cols - 10)
                y += 10;
            break;
        case 'w':
            printf("move top ...\n");
            if(x >= 10)
                x -= 10;
            break;
        case 's':
            printf("move down ...\n");
            if(x < backImg.rows - 10)
                x += 10;
            break;
        case 'e':
            printf("zoom ...\n");
            alpha += delta;
            if(!zoomImage(foreImg, alpha))
               alpha -= delta; 
            break;
        case 'r':
            printf("shrink ...\n");
            alpha -= delta;
            if(!zoomImage(foreImg, alpha))
                alpha += delta;
            break;
        case 'c':
            printf("save ...\n");
            saveFlag = true;
            break;

        default:
            break;
    }
}

bool zoomImage(Mat& img, double alpha)
{
    int newR = img.rows * alpha;
    int newC = img.cols * alpha;
    if(x + newR > backImg.rows || y + newC > backImg.cols || newR <= 1 || newC <= 1)
        return false;
    return true;
}

void saveCompose(Mat compImg)
{
    imwrite("compose.jpg", compImg);
}

void* composeImage(void* args)
{
    Mat compImg;
    namedWindow("compose"); 

    bool exitFlag = false;
    while(!exitFlag)  
    {
        showCompImage(compImg);
        imshow("compose", compImg );
        if(saveFlag)
        {
            saveFlag = false;
            saveCompose(compImg);
        }  
        char c = (char)waitKey(0); 
        composeKeyPressed(c, exitFlag);         
    }   
       
    destroyWindow("compose");
    return 0;
}