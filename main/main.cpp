#include "../global.h"
#include "../extractEvent.h"
#include "../compose.h"

#include <pthread.h>


void help()
{
    printf("\n");
    printf("This program demonstrates GrabCut segmentation -- select an object in a region\n"
            "and then grabcut will attempt to segment it out.\n");

    printf("Call:\n");
    printf("./PicBlend foregroundImage backgroundImage\n");
    printf("\n");

    printf("Select a rectangular area around the object you want to segment\n");
    printf("Hot keys: \n");
    printf("\n");
    printf("\tq - quit the program\n");
    printf("\tz - restore the original image\n");
    printf("\ty - next iteration\n");
    printf("\tf - switch the select mode to foreground\n");
    printf("\tb - switch the select mode to background\n");
    printf("\tl - find out the current select mode\n");
    printf("\n");

    printf("\tfirst, left mouse button - set object rectangle\n");
    printf("\n");
    printf("\tleft mouse button - set background pixels\n");
    printf("\tright mouse button - set foreground pixels\n");
    printf("\n");
}



int main( int argc, char** argv )
{
	help();

    if(argc < 3)
        return 0;
    foreImg = imread(argv[1]);
    if(foreImg.empty() )
    {
        printf("couldn't read image filename %s\n", argv[1]);
        exit(1);
    }
    backImg = imread(argv[2]);
    if(backImg.empty() )
    {
        printf("couldn't read image filename %s\n", argv[2]);
        exit(1);
    }
    extractImage(0);
    return 0;
}