
#include "cv.h"

#include "cxcore.h"

#include "highgui.h"

#include "math.h"

#include <iostream>

#include <stdio.h>

#include <string.h>

#include <conio.h>

#include <sstream>




using namespace std;

/*

--------------------------------------------*/

int main()

{

	int c = 0;

    CvCapture* capture = cvCaptureFromCAM(0);

		if(!cvQueryFrame(capture)){ cout<<"Video capture failed, please check the camera."<<endl;}else{cout<<"Video camera capture status: OK"<<endl;};

    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
		CvSize sz = cvGetSize(cvQueryFrame( capture));

	IplImage* src = cvCreateImage( sz, 8, 3 );

	IplImage* hsv_image = cvCreateImage( sz, 8, 3);

	IplImage* hsv_mask = cvCreateImage( sz, 8, 1);
// black
	 /*CvScalar  hsv_min = cvScalar(0 ,0, 0, 0);

    CvScalar hsv_max = cvScalar(180, 60,60, 0);
*/
	//for yellow
	 CvScalar  hsv_min = cvScalar(23,50, 80, 0);

    CvScalar hsv_max = cvScalar(35, 255,255, 0);


	//////

	while( c != 27)

	{

		src = cvQueryFrame( capture);
		 cvSmooth(src, src, CV_GAUSSIAN,3,3);
	    cvDilate(src, src, 0, 1);

			cvNamedWindow( "src",1); cvShowImage( "src", src);

		cvCvtColor(src, hsv_image, CV_BGR2HSV);

			cvNamedWindow( "hsv-img",1); cvShowImage( "hsv-img", hsv_image);

		cvInRangeS (hsv_image, hsv_min, hsv_max, hsv_mask);

			cvNamedWindow( "hsv-msk",1); cvShowImage( "hsv-msk", hsv_mask); hsv_mask->origin = 0; 

			



		c = cvWaitKey( 10);

	}

	//////

	cvReleaseCapture( &capture);

	cvDestroyAllWindows();



}
