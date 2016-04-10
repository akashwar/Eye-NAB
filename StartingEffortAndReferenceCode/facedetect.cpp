/**
 * Display video from webcam and detect faces
 */
#include <stdio.h>
#include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "string.h"
 
CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
 
void detectFaces( IplImage *img );
 
int main( int argc, char** argv )
{
    CvCapture *capture;
    IplImage  *frame;
	char a[50];
    
    char      *filename = ".\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
 
    /* load the classifier
       note that I put the file in the same directory with
       this code */
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
 
    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );
 
    /* initialize camera */
    capture = cvCaptureFromCAM(0);
		
    /* always check */
    assert( cascade && storage && capture );

 
    /* create a window */
    cvNamedWindow( "video", 1 );
	
	int cnt=0;
	char count[2]="a";
    while( 1 ) {
			count[0]++;
		//char temp=;
		//const char *count='a'+cnt;
        /* get a frame */
	//	frame = cvLoadImage("c:\\tv\\images\\r18.jpg",1);
			frame=  cvQueryFrame( capture );
        /* always check */
 //       if( !frame ) break;
 
        /* 'fix' frame */
       // cvFlip( f, f, -1 );
        frame->origin = 0;
 
        /* detect faces and display video */
        detectFaces( frame );
		//getch();
//		printf("hello");
		/*strcpy(a,"c:\\tv\\images\\outputimage\\");
		printf("copied");
		strcat(a,count);
		printf("copied");
		strcat(a,".jpg");
		printf("copied");
		cvSaveImage(a,frame,0);
		cnt++; //quit if user press 'q' 
        
		if(cnt==21)
			break; */
		int key = cvWaitKey(10);
		
    }
 //getch();
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
	int s=-1;
    /* detect faces */
    CvSeq *faces = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.1,
            1,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
            cvSize( 30, 30 ) );
	
	 CvRect *r[50];
	 CvRect *r1;
	 //printf("%d",faces->total);
	 int flagu=0;
 for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
      

	 
	 flagu=1;
	r[i] = ( CvRect* )cvGetSeqElem( faces, i );
	//printf("x  =  %d\n",r[i]->x);
	//	printf("y  =  %d\n",r[i]->y);
	//	printf("pontx  =  %d\n",r[i]->x + r[i]->width);
	//	printf("ponty  =  %d\n",r[i]->x + r[i]->height);
		
	 if(s==-1 && ((r[i]->height<100)&&(r[i]->width<100)))
	 {
		 s=i;
		 //printf("r value on y initial= %d",r[i]->y );
		

	 }
	 else
		 if((r[i]->y < r[s]->y)&&((r[i]->height<100)&&(r[i]->width<100)))
	 {
		
		 //printf("r value on y= %d",r[i]->y );
		
		s=i;
	 }
		   cvRectangle( img,
                     cvPoint( r[i]->x, r[i]->y ),
                     cvPoint( r[i]->x + r[i]->width, r[i]->y + r[i]->height ),
                     CV_RGB( 0,255, 0 ), 1, 8, 0 );


 }
 if(flagu==1)
 {
 
r1= ( CvRect* )cvGetSeqElem( faces, s);

printf("Detected rectangle");
printf("x  =  %d\n",r1->x);
		printf("y  =  %d\n",r1->y);
		printf("pontx  =  %d\n",r1->x + r1->width);
		printf("ponty  =  %d\n",r1->y + r1->height);
			// if(r1->x >((img->width)/2)+20)
	 {
		
		 printf("width= %d     height= %d",r1->width,r1->height);
        cvRectangle( img,
                     cvPoint( r1->x, r1->y ),
                     cvPoint( r1->x + r1->width, r1->y + r1->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );
	 }
 }   
    /* for each face found, draw a red box */
    /*
		printf("x  =  %d\n",r->x);
		printf("y  =  %d\n",r->y);
		printf("pontx  =  %d\n",r->x + r->width);
		printf("ponty  =  %d\n",r->x + r->height); */
		
		
		
    
 
    /* display video */
    cvShowImage( "video", img );
}