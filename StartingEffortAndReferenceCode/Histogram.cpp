#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <cvaux.h>
#include <cxcore.h>
#include <conio.h>
//#include <iostream.h>
IplImage* DrawHistogram(CvHistogram *hist, float scaleX=1, float scaleY=1);

void main()
{
	IplImage* img = cvLoadImage("C:\\tv\\images\\mixed\\sampletemplate\\100final2.jpg");
	int numBins = 256;
    float range[] = {0, 255};
    float *ranges[] = { range };
 
    CvHistogram *hist = cvCreateHist(1, &numBins, CV_HIST_ARRAY, ranges, 1);
    cvClearHist(hist);
 cvNamedWindow("Red1",1);
	  cvNamedWindow("Rgb",1);
	 IplImage* imgRed = cvCreateImage(cvGetSize(img), 8, 1);
    IplImage* imgGreen = cvCreateImage(cvGetSize(img), 8, 1);
    IplImage* imgBlue = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img,img,CV_BGR2HSV);
	//cvCvtColor(img,img,CV_HSV2BGR);
    cvCvtPixToPlane(img, imgBlue, imgGreen, imgRed, NULL);
	 
	   cvNamedWindow("Rgb1",1);
	  cvShowImage("Rgb1",img);
    cvNamedWindow("Green1",1);
    cvNamedWindow("Blue1",1);
 cvShowImage("Red1",imgRed);
 cvShowImage("Blue1",imgBlue);
 cvShowImage("Green1",imgGreen);
 cvZero(imgBlue);
 //cvZero(imgGreen);
 //cvZero(imgRed);
 cvMerge(imgBlue, imgGreen, imgRed, NULL,img);
 cvNamedWindow("hsv",1);
 cvShowImage("hsv",img);

 cvCvtColor(img,img,CV_HSV2BGR);
cvShowImage("Rgb",img);   

 cvCalcHist(&imgRed, hist, 0, 0);
	  IplImage* imgHistRed = DrawHistogram(hist);
    cvClearHist(hist);


		  cvCalcHist(&imgGreen, hist, 0, 0);
    IplImage* imgHistGreen = DrawHistogram(hist);
    cvClearHist(hist);
 
    cvCalcHist(&imgBlue, hist, 0, 0);
    IplImage* imgHistBlue = DrawHistogram(hist);
    cvClearHist(hist);


	  cvNamedWindow("Red");
    cvNamedWindow("Green");
    cvNamedWindow("Blue");
 
    cvShowImage("Red", imgHistRed);
    cvShowImage("Green", imgHistGreen);
    cvShowImage("Blue", imgHistBlue);
 
    cvWaitKey(0);
}
IplImage* DrawHistogram(CvHistogram *hist, float scaleX, float scaleY)
{
	 float histMax = 0;
    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0);
	IplImage* imgHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8 ,1);
    cvZero(imgHist);
	  for(int i=0;i<255;i++)
    {
        float histValue = cvQueryHistValue_1D(hist, i);
        float nextValue = cvQueryHistValue_1D(hist, i+1);
 
        CvPoint pt1 = cvPoint(i*scaleX, 64*scaleY);
        CvPoint pt2 = cvPoint(i*scaleX+scaleX, 64*scaleY);
        CvPoint pt3 = cvPoint(i*scaleX+scaleX, (64-nextValue*64/histMax)*scaleY);
        CvPoint pt4 = cvPoint(i*scaleX, (64-histValue*64/histMax)*scaleY);
 
        int numPts = 5;
        CvPoint pts[] = {pt1, pt2, pt3, pt4, pt1};
 
        cvFillConvexPoly(imgHist, pts, numPts, cvScalar(255));
    }
	    return imgHist;
}