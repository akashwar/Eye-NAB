
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include<conio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <direct.h>	
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <vector>
#include <string>
#define snprintf sprintf_s
int gli=0;
const char *cascade_name=".\\data\\haarcascades\\haarcascade_frontalface_default.xml";
void templatematch(IplImage *img);
void playSound()
{
	char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";

		strcat(address,"c:\\tv\\sounds\\bb\\");
	
		strcat(address,"original.mp3");
		system(address);
		system(address);

}
void detect_and_draw( IplImage* img )
{

	int stepr, channelsr;
	uchar *datar;
	
	stepr=img->widthStep;
		channelsr=img->nChannels;
		datar = (uchar *)img->imageData;
		
		for(int i=0;i<img->height;i++)
		{
			for(int j=0;j<img->width;j++)
			{
				
				//if(datar[i*stepr+j*channelsr]-datar[i*stepr+(j-1)*channelsr]>=50||datar[i*stepr+j*channelsr]-datar[(i-1)*stepr+j*channelsr]>=50)
				/*if((datar[i*stepr+j*channelsr]<150) || (datar[i*stepr+j*channelsr]>220))
					datar[i*stepr+j*channelsr]=0;
				else*/
					datar[i*stepr+j*channelsr]-=100;

					
			}
		}


    // Create memory for calculations
    static CvMemStorage* storage = 0;

    // Create a new Haar classifier
    static CvHaarClassifierCascade* cascade = 0;

    // Sets the scale with which the rectangle is drawn with
    int scale = 1;

    // Create two points to represent the hand locations
    CvPoint pt1, pt2;

    // Looping variable
    int i;

    // Load the HaarClassifierCascade
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );

    // Check whether the cascade has loaded successfully. Else report and error and quit
    if( !cascade )
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return;
    }

    // Allocate the memory storage
    storage = cvCreateMemStorage(0);

    // Create a new named window with title: result
    cvNamedWindow( "result", 1 );

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );

    // Find whether the cascade is loaded, to find the hands. If yes, then:
    if( cascade )
    {

        // There can be more than one hand in an image. So create a growable sequence of hands.
        // Detect the objects and store them in the sequence
       CvSeq* hands = cvHaarDetectObjects( img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(100, 100) );

        // Loop the number of hands found.
        for( i = 0; i < (hands ? hands->total : 0); i++ )
        {
           // Create a new rectangle for drawing the hand
            CvRect* r = (CvRect*)cvGetSeqElem( hands, i );

            // Find the dimensions of the hand,and scale it if necessary
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
            cvRectangle( img, pt1, pt2, CV_RGB(230,20,232), 3, 8, 0 );
			//cvSaveImage("c:\\tv\\images\\gadhireal.jpg",img);
			printf("tpcalled");
			templatematch(img);
        }
    }

    // Show the image in the window named "result"
    cvShowImage( "result", img );
	int keyPressed=0;
	char cstr[255];
	if (kbhit())
			keyPressed = getch();
		
		if (keyPressed==97) {	
			snprintf(cstr, sizeof(cstr)-1, "c:/tv/gandhitemp/%d.bmp", gli);	
			puts(cstr);
			cvSaveImage(cstr,img);
			gli++;
			
		}

}
int main()
{

  // Gets the input video stream from camera
  CvCapture* capture = cvCaptureFromCAM( 0 );
cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
  // Checks if the input stream is obtained
  if( !capture )
  {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    getchar();
    return -1;
  }

  // Show the image captured from the camera in the window and repeat
  while( 1 )
  {

    // Get one frame
    IplImage* frame = cvQueryFrame( capture );

    // Cecks if a frame is obtained
    if( !frame )
    {
      fprintf( stderr, "ERROR: frame is null...\n" );
      getchar();
      break;
    }

    // Flips the frame into mirror image
//    cvFlip(frame,frame,1);
		// cvSmooth(frame, frame, CV_GAUSSIAN,3,3);
	    //cvDilate(frame, frame, 0, 2);
	    
//cvAddS(frame, cvScalar(-50,-50,-50), frame);
	// cvScale(frame, frame, 1);
    // Call the function to detect and draw the hand positions
IplImage* grayImg = cvCreateImage(cvGetSize(frame), 8, 1);

	cvCvtColor(frame, grayImg, CV_RGB2GRAY);
/*	IplImage* laplaceImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_16S, 1);

	IplImage* abs_laplaceImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

	cvLaplace(grayImg, laplaceImg, 1);

	cvConvertScaleAbs(laplaceImg, abs_laplaceImg, 1, 0);

	IplImage* dstImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	cvAdd(abs_laplaceImg, grayImg, dstImg, NULL); 

*/

	detect_and_draw(grayImg);

    //If ESC key pressed exit
if( (cvWaitKey(10) & 255) == 27 )
      break;
  }

  // Release the capture device housekeeping
  cvReleaseCapture( &capture );

  return 0;
}
void templatematch(IplImage *img1)
{
	IplImage *temp=cvCreateImage(cvGetSize(img1),8,1);

	//-------VARIABLES USED FOR TEMPLATE MATCHING----------
	IplImage *img;
	IplImage	*tpl1,*tpl;
	IplImage	*res;
	CvPoint		minloc, maxloc;
	double		minval, maxval;
	int			img_width, img_height;
	int			tpl_width, tpl_height;
	int			res_width, res_height;
	float maxi=-1.0;
	int index=0;
	cvNamedWindow( "reference", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "template", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("result",CV_WINDOW_AUTOSIZE);
		//-------TEMPORARY IMAGE-COLOR--------------
	img=cvCreateImage(cvGetSize(img1),8,1);
	
	//-----------THIS LOOP MATCHES THE IMAGE WITH 9 DIFFERENT TEMPLATES AS STORED IN THE GLOBAL 
	// (continued)VARIBLE Notetpl[]----------------
	
		cvCopy(img1,img,NULL);
		tpl1 = cvLoadImage("c:\\tv\\images\\rgtemplate.jpg");
		 tpl = cvCreateImage(cvGetSize(tpl1), 8, 1);

	cvCvtColor(tpl1, tpl, CV_RGB2GRAY);
		img_width  = img->width;
		img_height = img->height;
		tpl_width  = tpl->width;
		tpl_height = tpl->height;
		res_width  = img_width - tpl_width + 1;
		res_height = img_height - tpl_height + 1;


		/* create new image for template matching computation */
		res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
		/* choose template matching method to be used */
		//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
		//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
		//cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
		//cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
		//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
		printf("hi");
		cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );
		printf("done");
		cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
		/* draw red rectangle */
		cvRectangle( img, 
				 cvPoint( maxloc.x, maxloc.y ), 
				 cvPoint( maxloc.x + tpl_width, maxloc.y + tpl_height ),
				 cvScalar( 0, 0, 255, 0 ), 1, 0, 0 );	
		
			
		printf("\nminval= %f  maxval= %f  ",minval,maxval);
		if(maxval>0.7)
		{
			playSound();

		}
		//------AS WE NEED ONLY HALF PART(RIGHT PART ) OF NOTE WHERE GANDHIJI IS PRESENT THIS LOOP ONLY BRINGS IN FOCUS THAT AREA 
		// (continued) OTHER PORTION IS BLACKENED--------
				

		//---------COMPARES AMONGST NINE TEMPLATES WHICH IS MAX MATCHED TEMPLATE--------
		
	

		cvShowImage( "reference", img );
		cvShowImage( "template", tpl );
		cvShowImage("result",res);
		cvWaitKey(2000);	
	
	printf("\n\nWaiting for next image to load\n");
	cvDestroyWindow( "reference" );
	cvDestroyWindow( "template" );
	cvDestroyWindow( "result" );




}
