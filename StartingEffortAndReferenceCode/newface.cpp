/*
 * Display video from webcam and detect faces
 */
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "conio.h"
 
CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
 
int detectFaces( IplImage *img );
 
int main( int argc, char** argv )
{
    CvCapture *capture;
 IplImage *frame[30];
 
 IplImage *f;	
    char      *filename = ".\\data\\haarcascades\\haarcascade_eye.xml";
 
    /* load the classifier
       note that I put the file in the same directory with
       this code */
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );
    /* initialize camera */
    capture = cvCaptureFromCAM( 0 );
    /* always check */
    assert( cascade && storage && capture );
    /* create a window */
    cvNamedWindow( "video", 1 );
	int count=0;
	int cnt=0;
    while(1) 
	{
	   /* get a frame */
	    //f = cvQueryFrame( capture );
		frame=cvLoadImage("c:\\tv\\images\\phine\\50rs\\w (15).jpg",1);
        /* always check */
	  //cvShowImage("video1",frame[count]);
        if( !f ) break;
         /* 'fix' frame */
    //    cvFlip( f, f, -1 );
        f->origin = 0;
         /* detect faces and display video */
        if(detectFaces(f ))
		{
			frame[count]=cvCreateImage(cvSize(320,240),8,3);
			cvCopyImage(f,frame[count],NULL);
			count++;
		}
       /* quit if user press 'q' */
		cvWaitKey(200);
		cnt++;
	   if(cnt==21) break;
    }
	printf("count= %d",count);	
		char a[50];
		char count1[2]="a";
	for(int k=0;k<count;k++)
	{
		cvShowImage("video",frame[k]);
		cvWaitKey(300);
	}
	cvDestroyWindow( "result" );
	/* free memory */
    cvReleaseCapture( &capture );
    cvDestroyWindow( "video" );
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
    return 0;
}
 
int detectFaces( IplImage *img )
{
    int i;
	int s=-1;
    /* detect faces */
    CvSeq *faces = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.1,
            1,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
            cvSize(40,40 ) );
 
	 CvRect *r1;
	 int fl=0;
    /* for each face found, draw a red box */
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
			r1 = ( CvRect* )cvGetSeqElem( faces, i );	
		if((r1->height<100)&&(r1->width<100))
	 {
			fl=1;
	
        cvRectangle( img,
                     cvPoint( r1->x, r1->y ),
                     cvPoint( r1->x + r1->width, r1->y + r1->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );
		printf("width %d height  %d\n",r1->width,r1->height);
	}
 
	}   /* display video */
    cvShowImage( "video", img );
	return fl;
}
 