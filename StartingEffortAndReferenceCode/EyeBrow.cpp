#include <stdio.h>
#include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#define LEFTIMAGEOFFSET 5
#define LEFTBROW 0
#define RIGHTBROW 1
#define LEFTSIDEBROWLEFT 0
#define LEFTSIDEBROWRIGHT 1

#define RIGHTSIDEBROWLEFT 0
#define RIGHTSIDEBROWRIGHT 1
CvPoint pt[6];

#define MAXBLACK 1 
IplImage *img;

float angleBetween(CvPoint v1, CvPoint v2)
{
	double m1 = v1.x * v1.x + v1.y * v1.y;
	double m2 = v2.x * v2.x + v2.y * v2.y;
    float len1 = sqrt(m1);
    float len2 = sqrt(m2);

    float dot = v1.x * v2.x + v1.y * v2.y;

    float a = dot / (len1 * len2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return 3.14;
    else
        return acos(a); // 0..PI
}
void topPointBrow(IplImage *blackwhiteImage,int whichBrow) {
	int stepr, channelsr;
	uchar *datar;
	int lowerbound;
	int upperbound;
	stepr=blackwhiteImage->widthStep;
	channelsr=blackwhiteImage->nChannels;
	datar = (uchar *)blackwhiteImage->imageData;
	int pointindex;
	if(whichBrow == 0) {
		lowerbound = LEFTIMAGEOFFSET;
		upperbound = blackwhiteImage->width/2;
		pointindex = 0;
	}else {
		lowerbound = blackwhiteImage->width/2;
		upperbound = blackwhiteImage->width - LEFTIMAGEOFFSET;
		pointindex = 1;
	}
	int count;
	int flag = 0;
	int flag1 = 0;
	for(int i = 0; i< blackwhiteImage->height/2;i++ ){
		count = 0;
		for(int j = lowerbound; j< upperbound;j++) {
			if(flag == 1) {

				if(datar[i*stepr+j*channelsr] == 0){
					CvScalar s = cvGet2D(img,i,j);
					pt[pointindex]= cvPoint(j,i);
					printf(" %d %d \n",pt[pointindex].x,pt[pointindex].y);
					s.val[0]=0.0;
					s.val[1]=255.0;
					s.val[2]=0.0;
				
					cvSet2D(img, i, j, s );
					printf("got");
				count++;
				}
				
				if(count == MAXBLACK) {
					flag1 =1;
					break;
				}
			}else {

				if(datar[i*stepr+j*channelsr] == 0) {
					count++;

				}
				if(count == MAXBLACK) {
					printf("Got ");
					flag = 1;
					i--;
					break;
				}
			}

		}
		if(flag1 == 1){
			break;
		}

	}
	cvNamedWindow("GrayTopPoint",1);
	cvShowImage("GrayTopPoint",img);
}

void sidePointBrowLeft(IplImage *blackwhiteImage,int whichBrow) {
	int stepr, channelsr;
	uchar *datar;
	int lowerbound;
	int upperbound;
	stepr=blackwhiteImage->widthStep;
	channelsr=blackwhiteImage->nChannels;
	datar = (uchar *)blackwhiteImage->imageData;
	int pointindex;
	if(whichBrow == 0) {
		lowerbound = LEFTIMAGEOFFSET;
		upperbound = blackwhiteImage->width/2;
		pointindex = 2;
	}else {
		lowerbound = blackwhiteImage->width/2;
		upperbound = blackwhiteImage->width - LEFTIMAGEOFFSET;
		pointindex = 3;
	}
	int count;
	printf(" step %d channel %d",stepr,channelsr);
	int flag = 0;
	int flag1 = 0;
	for(int j = lowerbound; j< upperbound;j++ ){
		count = 0;
		for(int i = 0; i< blackwhiteImage->height/2;i++ ) {
			if(flag == 1) {

				if(datar[i*stepr+j*channelsr] == 0){
					pt[pointindex]= cvPoint(j,i);
					printf(" %d %d \n",pt[pointindex].x,pt[pointindex].y);
					CvScalar s = cvGet2D(img,i,j);
					s.val[0]=0.0;
					s.val[1]=255.0;
					s.val[2]=0.0;
				
					cvSet2D(img, i, j, s );
					
					printf("got");
				count++;
				}
				
				if(count == MAXBLACK) {
					flag1 =1;
					break;
				}
			}else {

				if(datar[i*stepr+j*channelsr] == 0) {
					count++;

				}
				if(count == MAXBLACK) {
					printf("Got ");
					flag = 1;
					j--;
					break;
				}
			}

		}
		if(flag1 == 1){
			break;
		}

	}
	cvNamedWindow("GrayTopPoint",1);
	cvShowImage("GrayTopPoint",img);
}

void sidePointBrowRight(IplImage *blackwhiteImage,int whichBrow) {
	int stepr, channelsr;
	uchar *datar;
	int lowerbound;
	int upperbound;
	stepr=blackwhiteImage->widthStep;
	channelsr=blackwhiteImage->nChannels;
	datar = (uchar *)blackwhiteImage->imageData;
	int pointindex;
	if(whichBrow == 0) {
		pointindex = 4;
		lowerbound = blackwhiteImage->width/2;
		upperbound = LEFTIMAGEOFFSET;
	}else {
		pointindex = 5;
		lowerbound = blackwhiteImage->width - LEFTIMAGEOFFSET;
		upperbound = blackwhiteImage->width/2;
	}
	int count;
	printf(" step %d channel %d",stepr,channelsr);
	int flag = 0;
	int flag1 = 0;
	for(int j = lowerbound; j> upperbound;j-- ){
		count = 0;
		for(int i = 0; i< blackwhiteImage->height/2;i++ ) {
			if(flag == 1) {

				if(datar[i*stepr+j*channelsr] == 0){
					pt[pointindex]= cvPoint(j,i);
					printf(" %d %d \n",pt[pointindex].x,pt[pointindex].y);
					CvScalar s = cvGet2D(img,i,j);
					s.val[0]=0.0;
					s.val[1]=255.0;
					s.val[2]=0.0;
				
					cvSet2D(img, i, j, s );
					
					printf("got");
				count++;
				}
				
				if(count == MAXBLACK) {
					flag1 =1;
					break;
				}
			}else {

				if(datar[i*stepr+j*channelsr] == 0) {
					count++;

				}
				if(count == MAXBLACK) {
					printf("Got ");
					flag = 1;
					j++;
					break;
				}
			}

		}
		if(flag1 == 1){
			break;
		}

	}
	cvNamedWindow("GrayTopPoint",1);
	cvShowImage("GrayTopPoint",img);
}


void blackWhiteEyeBrow() {
	int stepr, channelsr;
	uchar *datar;
	img = cvLoadImage("c:\\tv\\facialExpression\\eyeBrow.jpg",1);
	IplImage *blackwhiteImage = cvCreateImage(cvGetSize(img),8,1);
	cvCvtColor(img, blackwhiteImage, CV_RGB2GRAY);
	
	stepr=blackwhiteImage->widthStep;
	channelsr=blackwhiteImage->nChannels;
	datar = (uchar *)blackwhiteImage->imageData;
	
	
	for(int i = 0; i< blackwhiteImage->height ; i++) {
		for(int j = 0 ; j<blackwhiteImage->width; j++) {
			printf(" %d",datar[i*stepr+j*channelsr]);
			if(datar[i*stepr+j*channelsr]<100)
				datar[i*stepr+j*channelsr] = 0;
			else
				datar[i*stepr+j*channelsr] = 255;
		}
	}
	cvNamedWindow("BlackWhiteEyeBrow",1);
	cvShowImage("BlackWhiteEyeBrow",blackwhiteImage);	
	topPointBrow(blackwhiteImage,LEFTBROW);
	topPointBrow(blackwhiteImage,RIGHTBROW);
	sidePointBrowLeft(blackwhiteImage,LEFTSIDEBROWLEFT);
	sidePointBrowLeft(blackwhiteImage,LEFTSIDEBROWRIGHT);
	sidePointBrowRight(blackwhiteImage,RIGHTSIDEBROWLEFT);
	sidePointBrowRight(blackwhiteImage,RIGHTSIDEBROWRIGHT);
	cvLine(img,pt[0],pt[2],CV_RGB( 255, 0, 0 ),1,8,0);
	printf(" angle between points %f",angleBetween(pt[0],pt[1])*180/3.14);
	cvLine(img,pt[0],pt[4],CV_RGB( 255, 0, 0 ),1,8,0);
	cvLine(img,pt[2],pt[4],CV_RGB( 255, 0, 0 ),1,8,0);
	cvLine(img,pt[1],pt[3],CV_RGB( 255, 0, 0 ),1,8,0);
	cvLine(img,pt[1],pt[5],CV_RGB( 255, 0, 0 ),1,8,0);
	cvLine(img,pt[3],pt[5],CV_RGB( 255, 0, 0 ),1,8,0);
	cvNamedWindow("FinalEyeBrow",1);
	cvShowImage("FinalEyeBrow",img);
}

