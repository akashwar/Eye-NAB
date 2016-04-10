//VERSION: HAND DETECTION 1.0

//AUTHOR: ANDOL LI

//PROJECT: HAND DETECTION PROTOTYPE

//LAST UPDATED: 03/2009



#include "cv.h"

#include "cxcore.h"

#include "highgui.h"

#include "math.h"

#include <iostream>

#include <stdio.h>

#include <string.h>

#include <conio.h>

#include <sstream>

int maxarray[6];
int countpixels(IplImage * );


using namespace std;

/*

--------------------------------------------*/
void play(int no)
{

	if(no==0)
	{
		printf("Currency");
		char address[500]="c:\\tv\\vlc\\vlc.exe ";
		char temp[50]="c:\\tv\\speech\\currency.mp3";
		strcat(address,temp);
		system(address);
		system("taskkill /IM vlc.exe");
	

	}
	else
		if(no==1)
		{
			printf("Color");
			char address[500]="c:\\tv\\vlc\\vlc.exe ";
			char temp[50]="c:\\tv\\speech\\color.mp3";
			strcat(address,temp);
			system(address);
			system("taskkill /IM vlc.exe");
	
		}
		else
			if(no==2)
			{
				char address[500]="c:\\tv\\vlc\\vlc.exe ";
		char temp[50]="c:\\tv\\speech\\face.mp3";
		strcat(address,temp);
		system(address);
		system("taskkill /IM vlc.exe");
	
				printf("Face");


			}else
				if(no==3)
				{
					char address[500]="c:\\tv\\vlc\\vlc.exe ";
		char temp[50]="c:\\tv\\speech\\object.mp3";
		strcat(address,temp);
		system(address);
		system("taskkill /IM vlc.exe");
	
					printf("Object");


				}


}
int main()
{

	int c = 0;

    CvCapture* capture = cvCaptureFromCAM(0);
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT,240);

		if(!cvQueryFrame(capture)){ cout<<"Video capture failed, please check the camera."<<endl;}else{cout<<"Video camera capture status: OK"<<endl;};

    CvSize sz = cvGetSize(cvQueryFrame( capture));


	

	
///-------------------------Yellow range
	CvScalar  hsv_min[10];
	CvScalar  hsv_max[10];

	hsv_min[0] = cvScalar(23,50, 80, 0);

    hsv_max[0] = cvScalar(35, 255,255, 0);
	
//--------------------------black Range
    hsv_min[1] = cvScalar(0,0,0, 0);

	 hsv_max[1] = cvScalar(180, 35,50, 0);
	
	//---------- -lIGHTGREEN
     hsv_min[2] = cvScalar(53, 80, 100, 0);

	 hsv_max[2] = cvScalar(80, 255, 255, 0);
	
		//---------- -Orange
	hsv_min[3] = cvScalar(14,50, 80, 0);

    hsv_max[3] = cvScalar(22, 255,255, 0);
	//------------BLACK
	
IplImage* hsv_mask = cvCreateImage( sz, 8, 1);
			IplImage* hsv_image = cvCreateImage( sz, 8, 3);
				IplImage* src = cvCreateImage( sz, 8, 3 );
		

	//////
cvNamedWindow( "hsv-msk",1);
	while( c != 27)

	{
		printf("before for");
				src = cvQueryFrame( capture);
				cvNamedWindow( "src",1); cvShowImage( "src", src);

			cvCvtColor(src, hsv_image, CV_BGR2HSV);

			cvNamedWindow( "hsv-img",1); cvShowImage( "hsv-img", hsv_image);
		int maximum=0;
		int index=-1;
		for(int g=0;g<4;g++)
		{
			
		
			
		cvInRangeS (hsv_image, hsv_min[g], hsv_max[g], hsv_mask);

			 cvShowImage( "hsv-msk", hsv_mask); hsv_mask->origin = 0; 
			maxarray[g]=countpixels(hsv_mask);
			printf("\nMax array %d %d",g,maxarray[g]);
			if(maxarray[g]>maximum)
			{	maximum=maxarray[g];
				index=g;
			}

			c = cvWaitKey(1000);


		}
		if(maximum>500 &&index!=-1)
			play(index);
	printf("after for");

	//////
	}
	cvReleaseCapture( &capture);

	cvDestroyAllWindows();



	
}
int countpixels(IplImage * whiteblack)
{
	int cnt=0;
	int stepr, channelsr;
	uchar *datar;
	printf("inside countpixel");
	stepr=whiteblack->widthStep;
		channelsr=whiteblack->nChannels;
		datar = (uchar *)whiteblack->imageData;
		
		for(int i=60;i<180;i++)
		{
			for(int j=0;j<320;j++)
			{
				
				if(datar[i*stepr+j*channelsr]==255)
					cnt++;
			}
		}
		return cnt;
	
}