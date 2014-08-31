#include "RaspiCamCV.h"


#include <stdio.h>
#include <memory.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <memory.h>
#include <math.h>
#include <termio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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

#if 1
	struct sched_param sp;
	memset(&sp, 0, sizeof(sp));
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sp);
	//mlockall(MCL_CURRENT | MCL_FUTURE); //doesn't make much of a difference
	printf("result = %d\n",	setpriority(PRIO_PROCESS, 0, sched_get_priority_max(SCHED_FIFO)));
#endif

	fprintf(stderr,"test\n");fflush(stderr);
	usleep(500000);
	fprintf(stderr,"test\n");fflush(stderr);

	namedWindow("RGB",0);
	namedWindow("HSV",0);

	RaspiCamCvCapture * capture = raspiCamCvCreateCameraCapture(0);

	char key = (char)waitKey(1); //delay N millis, usually long enough to display and capture input

	double t = (double)getTickCount();
	while (1) 
	{
		i++;
		t = ((double)getTickCount() - t)/getTickFrequency();
		printf("%f\n",t); fflush(stdout);
		t = (double)getTickCount();

		rgb_frame = raspiCamCvQueryFrame_New(capture, mode);
		cvtColor(rgb_frame,hsv_frame,COLOR_RGB2HSV);
		fprintf(stderr,".");fflush(stderr);
		if(i>=0)
		{
			i=0;
			imshow("RGB",rgb_frame);
			imshow("HSV",hsv_frame);
		}
		char key = (char)waitKey(1); //delay N millis, usually long enough to display and capture input
	}
	raspiCamCvReleaseCapture(&capture);
	waitKey(0);
	return 0;
}
