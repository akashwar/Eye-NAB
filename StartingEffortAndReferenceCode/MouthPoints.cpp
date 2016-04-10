#include <stdio.h>
#include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
IplImage *mouthImage;

void plotMouthPoints() {
	
	mouthImage = cvLoadImage("c:\\tv\\facialexpression\\mouth.jpg",1);
	IplImage *graymouth = cvCreateImage(cvGetSize(mouthImage),8,1);
	
	cvCvtColor(mouthImage, graymouth, CV_RGB2GRAY);

	/*int stepr, channelsr;
	uchar *datar;

	stepr=graymouth->widthStep;
	channelsr=graymouth->nChannels;
	datar = (uchar *)graymouth->imageData;
	
	
	for(int i = 0; i< graymouth->height ; i++) {
		for(int j = 0 ; j<graymouth->width; j++) {
			printf(" %d",datar[i*stepr+j*channelsr]);
			if(datar[i*stepr+j*channelsr]<90)
				datar[i*stepr+j*channelsr] = 0;
			else
				datar[i*stepr+j*channelsr] = 255;
		}
	}
	*/
	// Perform canny edge finding on tempframe, and push the result back into itself!
	cvNamedWindow("MouthImagegray",1);
	cvShowImage("MouthImagegray",graymouth);
	cvCanny(graymouth, graymouth, 50,90 , 3);
	cvNamedWindow("MouthImage",1);
	cvShowImage("MouthImage",graymouth);


}
