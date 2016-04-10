#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "conio.h"
int main( int argc, char** argv )
{

	IplImage *img[1000];
	CvCapture *capture;
	capture = cvCaptureFromCAM(0);
	cvNamedWindow( "display", 1);
	int count=0;
	for(int i=0;i<1000;i++)
	{
		IplImage*x=cvQueryFrame(capture);
		//img[i]=x;
		img[i]=cvCreateImage(cvSize(320,240),8,3);
		cvCopy(x,img[i],0);
		cvWaitKey(10);
		cvShowImage("display",img[i]);
	}
	cvNamedWindow( "display1", 1);
	for(int i=0;i<1000;i++)
	{
				cvShowImage("display1",img[i]);
	cvWaitKey(2000);
	}
	
	/*while( 1 )
	{
	
		img[count]=  cvQueryFrame( capture );
		cvShowImage("display",img[count]);
		count++;
		if(count==21)break;
		cvWaitKey(100);

	}
	for(int i=0;i<count;i++)
	{
		cvShowImage("display",img[i]);

	}*/

	return 0;
}