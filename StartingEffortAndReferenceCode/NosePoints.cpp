#include <stdio.h>
#include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#define SHIFTCONSTANT 5
IplImage *noseImage;
void findnoseextremePoints (IplImage *edgeGrayNose,IplImage *noseColorNose,int leftnosepoint,int rightnosepoint ) ;

void plotNosePoints() {
	int total[500];
	noseImage = cvLoadImage("c:\\tv\\facialexpression\\nose.jpg",1);
	IplImage *graynose = cvCreateImage(cvGetSize(noseImage),8,1);
	
	cvCvtColor(noseImage, graynose, CV_RGB2GRAY);

	/*int stepr, channelsr;
	uchar *datar;

	stepr=graynose->widthStep;
	channelsr=graynose->nChannels;
	datar = (uchar *)graynose->imageData;
	
	
	for(int i = 0; i< graynose->height ; i++) {
		for(int j = 0 ; j<graynose->width; j++) {
			printf(" %d",datar[i*stepr+j*channelsr]);
			if(datar[i*stepr+j*channelsr]<90)
				datar[i*stepr+j*channelsr] = 0;
			else
				datar[i*stepr+j*channelsr] = 255;
		}
	}
	*/
	// Perform canny edge finding on tempframe, and push the result back into itself!
	cvNamedWindow("noseImagegray",1);
	cvShowImage("noseImagegray",graynose);
	cvErode(graynose,graynose,0,1);
	cvDilate(graynose,graynose,0,1);
	IplImage *nostril = cvCreateImage(cvGetSize(graynose),8,1);
	cvCopyImage(graynose,nostril);
	cvCanny(graynose, graynose, 50,150 , 3);
	cvNamedWindow("noseImage",1);
	cvShowImage("noseImage",graynose);
	
	int stepr, channelsr;
	uchar *datar;

	stepr=nostril->widthStep;
	channelsr=nostril->nChannels;
	datar = (uchar *)nostril->imageData;
	
	for(int i = 0; i< nostril->height ; i++) {
		for(int j = 0 ; j<nostril->width; j++) {
			printf(" %d",datar[i*stepr+j*channelsr]);
			if(datar[i*stepr+j*channelsr] < 30 ){
				datar[i*stepr+j*channelsr] = 255;
			}else {
				datar[i*stepr+j*channelsr] = 0;
			}
		}
	}
	cvNamedWindow("hs ",1);
	cvShowImage("hs",nostril);
	int leftnosepoint=999;
	int row;
	for(int i =0; i< nostril->height ; i++) {
		for(int j = 0 ; j<nostril->width; j++) {
			if(datar[i*stepr+j*channelsr] == 255) {
				if(j<leftnosepoint) {
					leftnosepoint = j;
					row = i;
					break;
				}
			}
		}
	}
	int rightnosepoint=0;
	for(int i =0; i< nostril->height ; i++) {
		for(int j = 0 ; j<nostril->width; j++) {
			if(datar[i*stepr+j*channelsr] == 255) {
				if(j>rightnosepoint) {
					rightnosepoint = j;
					
				}
			}
		}
	}
	int midforTipNose = (leftnosepoint + rightnosepoint) /2;
	printf(" left %d  right%d",leftnosepoint,rightnosepoint);
	CvScalar s = cvGet2D(noseImage,row, leftnosepoint-SHIFTCONSTANT);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(noseImage, row, leftnosepoint-SHIFTCONSTANT, s );

	s = cvGet2D(noseImage,row, rightnosepoint+SHIFTCONSTANT);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(noseImage, row, rightnosepoint+SHIFTCONSTANT, s );

	s = cvGet2D(noseImage,row, midforTipNose);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(noseImage, row, midforTipNose, s );
	cvNamedWindow("tip ",1);
	cvShowImage("tip",noseImage);
	findnoseextremePoints(graynose,noseImage,leftnosepoint-SHIFTCONSTANT,rightnosepoint+SHIFTCONSTANT );
}

void findnoseextremePoints (IplImage *edgeGrayNose,IplImage *noseColorNose,int leftnosepoint,int rightnosepoint ) {
	
	int flag = 0;
	int topleftnose_x;
	int row;
	int stepr, channelsr;
	uchar *datar;

	stepr=edgeGrayNose->widthStep;
	channelsr=edgeGrayNose->nChannels;
	datar = (uchar *)edgeGrayNose->imageData;
	for(int i =0; i< edgeGrayNose->height ; i++) {
		for(int j = 0 ; j<edgeGrayNose->width; j++) {
			if(datar[i*stepr+j*channelsr] == 255) {
				topleftnose_x = j;
				row = i;
				flag = 1;
				break;
			}
		}
		if(flag == 1) 
			break;
	}	
	int distance = topleftnose_x - leftnosepoint;
	int toprightnose_x = rightnosepoint - distance;
	CvScalar s = cvGet2D(noseImage,row, topleftnose_x);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(noseImage, row, topleftnose_x, s );

	s = cvGet2D(noseImage,row, toprightnose_x);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(noseImage, row, toprightnose_x, s );
	cvNamedWindow("tip ",1);
	cvShowImage("tip",noseImage);
	
}