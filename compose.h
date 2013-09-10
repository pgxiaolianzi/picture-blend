#ifndef _LIAN_COMPOSE
#define _LIAN_COMPOSE

#include "global.h"


extern int x, y;
extern pthread_mutex_t mutex;

void showCompImage(Mat& compImg);
bool zoomImage(Mat& img, double alpha);
void composeKeyPressed(char c, bool& exitFlag, Mat& foreImg, Mat& mask, Mat& backImg);
void* composeImage(void* args);

#endif