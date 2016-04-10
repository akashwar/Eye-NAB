#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>		// For _kbhit() on Windows
#include <direct.h>		// For mkdir(path) on Windows
#include <vector>
#include <string>
#define snprintf sprintf_s	// Visual Studio on Windows comes with sprintf_s() instead of snprintf()

int main()
{
	IplImage* src;
	CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
	char cstr[256];
	int keyPressed=0;
	while(1)
	{
		src = cvQueryFrame( capture );
	}


}