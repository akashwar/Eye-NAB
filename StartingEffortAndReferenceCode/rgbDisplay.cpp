#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <conio.h>
//#include<stdafx.h>
IplImage* NormalizeImage(IplImage* theimg)
{
	 IplImage* redchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(theimg), 8, 1);
	 IplImage* redavg = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* greenavg = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* blueavg= cvCreateImage(cvGetSize(theimg), 8, 1);
 
    IplImage* imgavg = cvCreateImage(cvGetSize(theimg), 8, 3);
	cvSplit(theimg, bluechannel, greenchannel, redchannel, NULL);
	   		 cvNormalize(redchannel, redavg, 0, 255, CV_MINMAX);
			 cvNormalize(greenchannel, greenavg, 0, 255, CV_MINMAX);
			 cvNormalize(bluechannel, blueavg, 0, 255, CV_MINMAX);
            
	
	    cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
		cvNamedWindow("imgavg",1);
		cvShowImage("imgavg",imgavg);
		cvWaitKey(1000);
		
    return imgavg;



}
int main()
{

	IplImage* frame;
	CvCapture *cap=NULL;
	cap=cvCaptureFromCAM(CV_CAP_ANY);
	for(int i=0;i<8;i++){
		frame = cvQueryFrame( cap);
	}
	while(1)
	{
		frame = cvQueryFrame( cap);

		cvNamedWindow("frame",1);
		cvShowImage("frame",frame);
		cvWaitKey(200);
		NormalizeImage(frame);	
	}
	return 0;

}