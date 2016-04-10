#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>		// For _kbhit() on Windows
#include <direct.h>		// For mkdir(path) on Windows
#include <vector>
#include <string>
#define snprintf sprintf_s	// Visual Studio on Windows comes with sprintf_s() instead of snprintf()
IplImage * subtractBackground(IplImage * );
IplImage* GetThresholdedImage(IplImage* imgHSV);
IplImage* back;
int lastX = -1;
int lastY = -1;


int main()
{
	IplImage* src;
	IplImage *rgbdiffimg;
	CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	cvWaitKey(1000);
	for(int i=0;i<10;i++)
	{
		src = cvQueryFrame( capture );
		cvNamedWindow("backgroundimage",1);
		cvShowImage("backgroundimage",src);
		cvWaitKey(1000);
		printf("Captured");
	}
	cvWaitKey(3000);
	printf("ready background now ");
	
	src = cvQueryFrame( capture );

	cvNamedWindow("backgroundimage",1);
	cvShowImage("backgroundimage",src);
	back=cvCreateImage(cvGetSize(src),8,3);
	cvCopyImage(src,back,0);
	printf("Ready for frontend");
	cvWaitKey(4000);
	while(1)
	{
		src = cvQueryFrame( capture );
		rgbdiffimg=subtractBackground(src);	
		IplImage* imgThresh=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U, 1);
		
		IplImage* imgHSV = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3); 
          cvCvtColor(rgbdiffimg, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
           
		imgThresh=GetThresholdedImage(imgHSV);

		cvWaitKey(1000);	
	}


}
IplImage *subtractBackground(IplImage *rgbsrc)
{

	IplImage *rgbdiffimg=cvCreateImage(cvGetSize(rgbsrc),8,3);
	int stepr,step, channelsr,channels;
	uchar *datar,*data;
	
	stepr=rgbsrc->widthStep;
	channelsr=rgbsrc->nChannels;
	datar = (uchar *)rgbsrc->imageData;
		
	step=back->widthStep;
	channels=back->nChannels;
	data = (uchar *)back->imageData;

	for(int i=0;i<rgbsrc->height;i++)
		{
			for(int j=0;j<rgbsrc->width;j++)
			{
				
				//printf("%d %d %d",datar[i*stepr+j*channelsr],datar[i*stepr+j*channelsr+1],datar[i*stepr+j*channelsr+2]);
				if((abs(datar[i*stepr+j*channelsr]-data[i*stepr+j*channelsr])<20) && (abs(datar[i*stepr+j*channelsr+1]-data[i*stepr+j*channelsr+1])<20) && (abs(datar[i*stepr+j*channelsr+2]-data[i*stepr+j*channelsr+2])<20) )
				{
					datar[i*stepr+j*channelsr]=255;
					datar[i*stepr+j*channelsr+1]=255;
					datar[i*stepr+j*channelsr+2]=255;



				}
					
			}
		}
	cvNamedWindow("src",1);
	cvShowImage("src",rgbsrc);
	
	return rgbsrc;	

}
IplImage* GetThresholdedImage(IplImage* imgHSV){       
	int ucnt=0;
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	IplImage* imgThresh1=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	IplImage* imgThresh2=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	IplImage* dest=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	

 	
	 cvInRangeS(imgHSV, cvScalar(23,50, 80, 0), cvScalar(35, 255,255, 0), imgThresh); 
	 cvCopy(imgThresh,imgThresh1);
	//-------track

	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(imgThresh, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetCentralMoment(moments, 0, 0);

		 // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	if(area>500 ){
			// calculate the position of the ball
	int posXY = moment10/area;
	int posYY = moment01/area;  
	printf("yELLOW Position  x y %d %d %lf\n",posXY,posYY,area);
	}
	else
	{

		printf("move towards right");
	}
		cvNamedWindow("imgyellow",1);
	cvShowImage("imgyellow",imgThresh);

	 //----------

	cvInRangeS(imgHSV, cvScalar(104,50, 70, 0), cvScalar(130, 255,170, 0), imgThresh);
	cvCopy(imgThresh,imgThresh2);
	cvMoments(imgThresh, moments, 1);
	 moment10 = cvGetSpatialMoment(moments, 1, 0);
	 moment01 = cvGetSpatialMoment(moments, 0, 1);
	 area = cvGetCentralMoment(moments, 0, 0);

		 // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	if(area>500 ){
			// calculate the position of the ball
	int posXB = moment10/area;
	int posYB = moment01/area;  
	printf("Blue Position  x y %d %d %lf\n",posXB,posYB,area);
	}
	else
	{

		printf("move towards left");
	}
			cvNamedWindow("imgBlue",1);
	cvShowImage("imgBlue",imgThresh);
	//black
	//cvInRangeS(imgHSV, cvScalar(0,0,0, 0), cvScalar(180, 35,50, 0), imgThresh);

	cvOr(imgThresh1,imgThresh2,dest,NULL);
	cvNamedWindow("ored",1);
	cvShowImage("ored",dest);

	free(moments);
	
    return imgThresh;
}

