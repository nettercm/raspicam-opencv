#include "RaspiCamCV.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

using namespace cv;
using namespace std;

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void delay(long double time) 
{
	usleep(1000000*time);
}

int main(int argc, char *argv[])
{
	Mat rgb_frame;
	Mat hsv_frame;
	int i=0;
	long double waitTime = 1;
	int mode = 0; //2;

	fprintf(stderr,"test\n");fflush(stderr);
	usleep(500000);
	fprintf(stderr,"test\n");fflush(stderr);

	namedWindow("RGB",0);
	namedWindow("HSV",0);

	RaspiCamCvCapture * capture = raspiCamCvCreateCameraCapture(0);

	char key = (char)waitKey(1); //delay N millis, usually long enough to display and capture input

	while (1) 
	{
		i++;
		rgb_frame = raspiCamCvQueryFrame_New(capture, mode);
		cvtColor(rgb_frame,hsv_frame,COLOR_RGB2HSV);
		fprintf(stderr,"\n.");fflush(stderr);
		if(i>=10)
		{
			i=0;
			//imshow("RGB",rgb_frame);
			//imshow("HSV",hsv_frame);
		}
		char key = (char)waitKey(1); //delay N millis, usually long enough to display and capture input
	}
	raspiCamCvReleaseCapture(&capture);
	waitKey(0);
	return 0;
}
