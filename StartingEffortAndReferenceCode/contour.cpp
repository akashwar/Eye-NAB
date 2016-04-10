#include<stdio.h>
#include<highgui.h>
#include<cv.h>
#include<math.h>
#include<stdlib.h>

IplImage* NormalizeImage(IplImage* theimg) {
IplImage* redchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* greenchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* bluechannel = cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* redavg = cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* greenavg = cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* blueavg= cvCreateImage(cvGetSize(theimg), 8, 1);
IplImage* imgavg = cvCreateImage(cvGetSize(theimg), 8, 3);
cvSplit(theimg, bluechannel, greenchannel, redchannel, NULL);
for (int x = 0; x <theimg->width; x++) {
for(int y = 0; y <theimg->height; y++) {
int redValue = cvGetReal2D(redchannel, y, x);
int greenValue = cvGetReal2D(greenchannel, y, x);
int blueValue = cvGetReal2D(bluechannel, y, x);
double sum = redValue + greenValue + blueValue;
cvSetReal2D(redavg, y, x, redValue / sum * 255);
cvSetReal2D(greenavg, y, x, greenValue / sum * 255);
cvSetReal2D(blueavg, y, x, blueValue / sum * 255);
}
}
cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
cvReleaseImage(&redchannel);
cvReleaseImage(&greenchannel);
cvReleaseImage(&bluechannel);
cvReleaseImage(&redavg);
cvReleaseImage(&greenavg);
cvReleaseImage(&blueavg);
return imgavg;
}
int main(int argc, char **argv)

{	
	IplImage * img=cvLoadImage("c:\\tv\\images\\r22.jpg");
	IplImage *img1=NormalizeImage(img);
	cvNamedWindow("show",1);
	cvShowImage("show",img1);
	cvWaitKey();
}
