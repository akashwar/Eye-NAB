/* This is a stand-alone program. Pass an image name as a
first parameter of the program.
Switch between standard and probabilistic Hough
transform by changing "#if 1" to "#if 0" and back */
#include <cv.h>
#include <highgui.h>
#include <math.h>
//#include<stdafx.h>
int main()
{
IplImage* src;

CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);

	//assert(capture);

	
//argv[1]="c:\\projbe\\images\\100rs.jpg";
while(1)
{
		printf("hello");
		cvWaitKey(200);
		src = cvQueryFrame( capture );
	IplImage* src1 = cvCreateImage(
	cvGetSize(src), 8, 1 );

		cvCvtColor( src, src1, CV_RGB2GRAY );

	IplImage* dst = cvCreateImage(
	cvGetSize(src), 8, 1 );
	IplImage* color_dst = cvCreateImage(
	cvGetSize(src), 8, 3 );
	CvMemStorage* storage =
	cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i;
	cvCanny( src1, dst,50, 150, 3 );
	cvDilate( dst, dst, 0, 1 );
	cvNamedWindow("edgedest",1);
	cvShowImage("edgedest",dst);
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );
	#if 0
	lines = cvHoughLines2( dst, storage,
	CV_HOUGH_STANDARD, 1, CV_PI, 50, 0, 0 );
	for( i = 0; i < MIN(lines->total,100); i++ )
	{
	float* line =
	(float*)cvGetSeqElem(lines,i);
	float rho = line[0];
	float theta = line[1];

	printf("theta = %f",(theta*180/3.142));
	CvPoint pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	printf("a= %f  b=%f  x0=%f  y0=%f roh=%f\n", a,b,x0,y0,rho);
	pt1.x = cvRound(x0 + 1000*(-b));
	pt1.y = cvRound(y0 + 1000*(a));
	pt2.x = cvRound(x0 - 1000*(-b));
	pt2.y = cvRound(y0 - 1000*(a));
	printf("    x1 = %d, y1 = %d",pt1.x,pt1.y);
	printf("    x2 = %d, y2 = %d\n\n",pt2.x,pt2.y);

	//if((theta*180/3.142) < 100 && (theta*180/3.142) > 79 )
	cvLine( color_dst, pt1, pt2,
	CV_RGB(255,0,0), 3, 8 );
	}
	#else
	lines = cvHoughLines2( dst, storage,
	CV_HOUGH_PROBABILISTIC, 1, 10*CV_PI/180, 30, 50, 10 );
	for( i = 0; i < lines->total; i++ )
	{
	CvPoint* line =
	(CvPoint*)cvGetSeqElem(lines,i);
	cvLine( color_dst, line[0], line[1],
	CV_RGB(255,0,0), 3, 8 );
	}
	#endif
	cvNamedWindow( "Source", 1 );
	cvShowImage( "Source", src );
	cvNamedWindow( "Hough", 1 );
	cvShowImage( "Hough", color_dst );
	//cvWaitKey(0);
	}
} 