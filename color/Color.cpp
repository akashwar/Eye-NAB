/*
Color Recognition:
Code to identify the different types of colors present in the image.
Making use of HSV images.

*/



#include "cv.h"

#include "cxcore.h"

#include "highgui.h"

#include "math.h"

#include <iostream>

#include <stdio.h>

#include <string.h>

#include <conio.h>

#include <sstream>


int countpixels(IplImage * );		//function to count number of pixels in each colors present in image.
void playcolor(int percent,int g);  //function to nofity user via audio output


int maxarray[6];
char color[10][20]={"yellow","black","green","orange","red","blue","grey","white"}; //colors that can be identified
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

	hsv_min[0] = cvScalar(20,50, 80, 0);

    hsv_max[0] = cvScalar(38, 255,255, 0);
	
//--------------------------black Range
    hsv_min[1] = cvScalar(0,0,0, 0);

	 hsv_max[1] = cvScalar(180, 35,50, 0);
	
	//------------lIGHTGREEN--------
     hsv_min[2] = cvScalar(38, 80, 100, 0);

	 hsv_max[2] = cvScalar(90, 255, 255, 0);
	
		//---------- -Orange
	hsv_min[3] = cvScalar(13,50, 80, 0);

    hsv_max[3] = cvScalar(22, 255,255, 0);
	//---------Red
	
	hsv_min[4] = cvScalar(170,50,60);

    hsv_max[4] = cvScalar(180,256,256);
	
	//----------------blue
	 hsv_min[5] = cvScalar(90, 80, 100, 0);

	 hsv_max[5] = cvScalar(150, 255, 255, 0);
	//--------grey
	hsv_min[6] = cvScalar(0, 0, 80, 0);

	 hsv_max[6] = cvScalar(180,10, 215, 0);
	//----white---------
	 	//--------grey
	hsv_min[7] = cvScalar(0, 0, 215, 0);

	 hsv_max[7] = cvScalar(180,255, 255, 0);

	IplImage* hsv_mask = cvCreateImage( sz, 8, 1);
	IplImage* hsv_image = cvCreateImage( sz, 8, 3);
	IplImage* src = cvCreateImage( sz, 8, 3 );
		

	//////
	cvNamedWindow( "hsv-msk",1);
	while( c != 27)
	{
		int k=0;
		for(int i=0;i<8;i++)
		{
			maxarray[i]=0;
		}
			while(k<10)
			{

			printf("before for");
			src = cvQueryFrame( capture);
			cvDilate(src,src,0,4);
			c = cvWaitKey(100);

			cvNamedWindow( "src",1); 
			cvShowImage( "src", src);
			cvCvtColor(src, hsv_image, CV_BGR2HSV);
			cvNamedWindow( "hsv-img",1); 
			cvShowImage( "hsv-img", hsv_image);
			int maximum=0;
			int index=-1;
			
				for(int g=0;g<8;g++)
				{
					
					cvInRangeS (hsv_image, hsv_min[g], hsv_max[g], hsv_mask);
					cvShowImage( "hsv-msk", hsv_mask); hsv_mask->origin = 0; 
					maxarray[g]+=countpixels(hsv_mask);
								//(float)maxarray[g]/76800*100
					//printf("\nMax array %d %f",g,(float)maxarray[g]/76800*100);
					


				}
			k++;
			}
			for(int i=0;i<10;i++)
			{
				maxarray[i]=maxarray[i]/10;
			}
			for(int g=0;g<8;g++)
			{
				printf("\nMax array %d %f",g,(float)maxarray[g]/76800*100);
				int percent=(int)(maxarray[g]*100/76800);
				printf("\n%d",percent);
				if(percent>=20)
				playcolor(percent,g);
			}
			
			k=0;

	//////
	}

	cvReleaseCapture( &capture);

	cvDestroyAllWindows();



	
}
void playcolor(int percent,int g)
{
		int ten,unit;
		char t[20],u[20];
		if(percent==100)
			percent=99;
		ten=percent/10;
		unit=percent%10;
		sprintf(t,"%d",ten);
		sprintf(u,"%d",unit);
		{
		char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";
		strcat(address,"c:\\tv\\sounds\\bb\\");
		strcat(address,color[g]);
		strcat(address,".mp3");
		system(address);
		printf("%s",address);
		
		
		}

		{
		char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";

		strcat(address,"c:\\tv\\sounds\\");
		strcat(address,t);
		strcat(address,".mp3");
		system(address);
		
		}
		{
		char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";

		strcat(address,"c:\\tv\\sounds\\");
		strcat(address,u);
		strcat(address,".mp3");
		system(address);
		}

		{
		char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";

		strcat(address,"c:\\tv\\sounds\\bb\\");
	
		strcat(address,"percent.mp3");
		system(address);
		}


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
		
		for(int i=0;i<240;i++)
		{
			for(int j=0;j<320;j++)
			{
				
				if(datar[i*stepr+j*channelsr]==255)
					cnt++;
			}
		}
		return cnt;
	
}