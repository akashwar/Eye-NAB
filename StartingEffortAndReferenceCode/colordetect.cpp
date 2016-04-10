/*ALL the necessary header files*/
#include"math.h"
#include"conio.h"
#include"cv.h"
#include"highgui.h"
#include"stdio.h"
void captureOriginal(IplImage* []);
int main()
{
int i,j,k;
int height,width,step,channels;
int stepr, channelsr;
int temp=0;
uchar *data,*datar;
i=j=k=0;
	IplImage  *OriginalFrame[50];
	captureOriginal(OriginalFrame);
	int count=0;
	
		IplImage *frame=OriginalFrame[10];
		IplImage *result=cvCreateImage( cvGetSize(frame), 8, 1 );
		if(frame==NULL ) {
		puts("unable to load the frame");exit(0);}
		printf("frame loaded");
		cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("Result",CV_WINDOW_AUTOSIZE);

		height = frame->height;
		width = frame->width;
		step =frame->widthStep;
		channels = frame->nChannels;
		data = (uchar *)frame->imageData;
		/*Here I use the Suffix r to diffenrentiate the result data and the frame data
		Example :stepr denotes widthStep of the result IplImage and step is for frame IplImage*/

		stepr=result->widthStep;
		channelsr=result->nChannels;
		datar = (uchar *)result->imageData;

		for(i=0;i < (height);i++) for(j=0;j <(width);j++)

		/*As I told you previously you need to select pixels which are
		more red than any other color
		Hence i select a difference of 29(which again depends on the scene).
		(you need to select randomly and test*/
		/*if(((data[i*step+j*channels+2]) > (29+data[i*step+j*channels]))
		&& ((data[i*step+j*channels+2]) > (29+data[i*step+j*channels+1])))
		datar[i*stepr+j*channelsr]=255;
		else
		datar[i*stepr+j*channelsr]=0;*/
		/*If you want to use cvErode you may...*/
		/*Destroying all the Windows*/
		if((data[i*step+j*channels+1])>150) 
		count++;			
		cvShowImage("original",frame);
		cvShowImage("Result",result);
		cvSaveImage("result.jpg",result);
		cvWaitKey(200);
		cvDestroyWindow("original");
		cvDestroyWindow("Result");
	
	printf("%d ", count);
	getch();
return 0;
}
void captureOriginal(IplImage* img[50])
{

		CvCapture *capture=NULL;
		capture = cvCaptureFromCAM(CV_CAP_ANY);

		//assert(capture);
		IplImage *frame;
		int framecount=0;
		cvNamedWindow("Im",1);
		for(int i=0;i<5;i++)
		frame = cvQueryFrame( capture );
		while(1)
		{
			frame = cvQueryFrame( capture );
			if ( !frame ) {
			fprintf( stderr, "ERROR: frame is null...\n" );
			}
			printf("\ncap i= %d",framecount);
			img[framecount]=cvCreateImage(cvGetSize(frame),8,3);
			cvCopyImage(frame,img[framecount]);
			frame->origin=0;
			framecount++;	
			if(framecount==21)
			break;
			cvWaitKey(200);
			cvShowImage("Im",frame);
		}
		//cvReleaseImage( &frame );
	
	

}
