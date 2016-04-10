/**
 * Display video from webcam and detect faces
 */
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
 
CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
 
void detectFaces( IplImage *img );
 
int main( int argc, char** argv )
{
    CvCapture *capture;
    IplImage  *frame;
    int       key;
    char      *filename = ".\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	
 
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
 
    while( 1 ) {
        /* get a frame */
    //    frame = cvQueryFrame( capture );
		frame=cvLoadImage("C:\\tv\\images\\mixed\\mixed(2).jpg",1);
 
        /* always check */
        if( !frame ) break;
 
        /* 'fix' frame */
        //cvFlip( frame, frame, -1 );
        frame->origin = 0;
 
        /* detect faces and display video */
        detectFaces( frame );
 
        /* quit if user press 'q' */
         cvWaitKey( 10 );
		 break;
    }

 
    /* free memory */
    cvReleaseCapture( &capture );
    cvDestroyWindow( "video" );
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
 
    return 0;
}
 
void detectFaces( IplImage *img )
{
    int i;
 	char count[2]="a";
	char a[50];
	int cnt=0;
    /* detect faces */
    CvSeq *faces = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.1,
            1,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
            cvSize( 40, 40 ) );
 
    /* for each face found, draw a red box */
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
/*        cvRectangle( img,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );*/
		printf("\n\n%d   %d ",r->x,r->y);

		printf("%d   %d",r->width,r->height);

CvScalar s;
IplImage *cropImage = cvCreateImage(cvGetSize(img), 8, 3);
cvCopy(img, cropImage, NULL);
for(int i=1;i<cropImage->height;i++)
	{
		for(int j=1;j<cropImage->width;j++)
		{

			if((j<r->x || j>r->x + r->width)  || (i<r->y || i>r->y + r->height))
			{ 
				//printf("hi..");
				s = cvGet2D(cropImage, i, j);
		//	printf("hi.....");
			//	s.
			//	s.val[3]=0.0;
				s.val[0]=0.0;
				s.val[1]=0.0;
				s.val[2]=0.0;
			
	cvSet2D(cropImage, i, j, s );

			}
			

		}

	}

			strcpy(a,"c:\\tv\\images\\mixed\\");
		
		strcat(a,count);
		
		strcat(a,".jpg");
		
		cvSaveImage(a,cropImage,0);
		
		cnt++;
		count[0]++;
	}
	
    /* display video */
    cvShowImage( "video", img );
}
 