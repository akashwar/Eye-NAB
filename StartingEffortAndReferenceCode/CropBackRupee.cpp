#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <cvaux.h>
#include <cxcore.h>
#include <conio.h>
float maxreturnval=0;
char BNotetpl[20][100]={"C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc1000.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc_50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc_100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc_500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\bc_1000.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\3bc50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\3bc100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\3bc500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\3bc1000.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\4bc50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\4bc100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\4bc500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\frntback\\resize\\4bc1000.jpg"};
void processImage(IplImage *);
IplImage *BNotetplProcessed;
CvRect BNotetplMatch(IplImage *);
CvRect detectBackRupee(IplImage * camImg);
CvRect detectBackRupee(IplImage * camImg)
{
	//--------------------Capturing the 20frames of notes in Original Frame IplImage array----------- 
	IplImage  *OriginalFrame;
	CvRect rect;
	printf("2called...");
	OriginalFrame=camImg;
	rect=BNotetplMatch(OriginalFrame);
	if(maxreturnval<0.54)
		rect.width=0;
	return rect;
	
}
/*
void main()
{
	CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);
	IplImage *frame;
	cvNamedWindow("framevideo",1);
	for(int i=0;i<5;i++)
	frame = cvQueryFrame( capture );
	while(1)
	{
		frame = cvQueryFrame( capture );
		if ( !frame ) {
	    fprintf( stderr, "ERROR: frame is null...\n" );
		}
		frame->origin=0;
		detectRupee(frame);
		//-------to show the grabbed frame using framevideo window-----
		cvShowImage("framevideo",frame);
			cvWaitKey(500);
			if(maxreturnval>0.5)
				break;
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow( "framevideo" );
	//cvReleaseImage( &frame );
	


}*/
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//-------THIS FUNCTION MATCHES THE DETECTED FACES WITH GANDHIJI'S TEMPLATE TO FIND THE EXACT HIGHEST FACE MATCH-----


//--------THIS FUNCTION PERFORMS A TEMPLATE MATCH ON THE ORIGINAL IMAGE TO CROP THE IMAGE FROM BACKGROUND-----------
//-------IN ORDER TO IDENTIFY THE REGION WHERE THE NOTE IS PRESENT----------------
CvRect BNotetplMatch(IplImage *img1)
{

	
	//------------BNotetplProcessed STORES THE IMAGE EXTRACTED FROM BACKGROUND-------------
	printf("\nBNotetplMatch");
	BNotetplProcessed=cvCreateImage(cvGetSize(img1),8,3);

	//-------VARIABLES USED FOR TEMPLATE MATCHING----------
	IplImage *img;
	IplImage	*tpl;
	IplImage	*res;
	CvPoint		minloc, maxloc;
	double		minval, maxval;
	int			img_width, img_height;
	int			tpl_width, tpl_height;
	int			res_width, res_height;
	float maxi=-1.0;
	int index=0;
	CvRect croprect;
	//-------TEMPORARY IMAGE-COLOR--------------
	img=cvCreateImage(cvGetSize(img1),8,3);

	//-----------THIS LOOP MATCHES THE IMAGE WITH 9 DIFFERENT TEMPLATES AS STORED IN THE GLOBAL 
	// (continued)VARIBLE BNotetpl[]----------------
	for(int km=0;km<12;km++)
	{
		cvCopy(img1,img,NULL);
		tpl = cvLoadImage(BNotetpl[km]);;
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
		cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );
		cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
		/* draw red rectangle */
		CvScalar s;
			
	//	printf("\nminval= %f  maxval= %f  ",minval,maxval);

		//------AS WE NEED ONLY HALF PART(RIGHT PART ) OF NOTE WHERE GANDHIJI IS PRESENT THIS LOOP ONLY BRINGS IN FOCUS THAT AREA 
		// (continued) OTHER PORTION IS BLACKENED--------
/*		
if(maxval>0.3 && maxval <0.54)
		{

			for(int h=0;h<5;h++)
			{
				
			


 //declare a destination IplImage object with correct size, depth and channels
				IplImage *destination = cvCreateImage
			( cvSize(((tpl->height-10*(h+1))*tpl->width/tpl->height) ,(tpl->height-10*(h+1))),tpl->depth, tpl->nChannels );

//use cvResize to resize source to a destination image
				cvResize(tpl, destination);
				
				


				res_width  = img_width - destination->width + 1;
				res_height = img_height - destination->height + 1;


				res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
				cvMatchTemplate( img, destination, res, CV_TM_CCOEFF_NORMED );
				cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
				printf("\n maxxxvallll = %f",maxval);
				if(maxval>0.54)
				{
					printf("\nEntered resize\n");			
					tpl_width=destination->width;
					tpl_height=destination->height;
					break;

				}


			}
			cvWaitKey(5000);
	
		}
	
		*/

		//---------COMPARES AMONGST NINE TEMPLATES WHICH IS MAX MATCHED TEMPLATE--------
		if(maxval>maxi)
		{
				index=km;
				croprect.x=maxloc.x;
				croprect.y=maxloc.y;
				croprect.width= tpl_width;
				croprect.height= tpl_height;
				maxi=maxval;
				cvCopyImage(img,BNotetplProcessed);
				cvRectangle(BNotetplProcessed, cvPoint(croprect.x, croprect.y), cvPoint(croprect.x + croprect.width-1, croprect.y + croprect.height-1), CV_RGB(0,255,0), 1, 8, 0);
		}

	

		//cvShowImage( "reference", img );
		//cvShowImage( "template", tpl );
		//cvShowImage("result",res);
	//	cvWaitKey(100);	
	}
	//printf("\n\nWaiting for next image to load\n");
	//cvDestroyWindow( "reference" );
	//cvDestroyWindow( "template" );
	//cvDestroyWindow( "result" );
	
	cvNamedWindow("noteprocessed",1);
	cvShowImage("noteprocessed",BNotetplProcessed);
	printf("\nindex= %d , max= %f",index,maxi);
	maxreturnval=maxi;
	return croprect;
}