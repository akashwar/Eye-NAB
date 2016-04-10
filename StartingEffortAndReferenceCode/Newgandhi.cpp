#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <conio.h>
/////////////////////////////////////////////
typedef struct GandhitplMatch
{
	IplImage *faceimg;
	int height;
	int width;
	float matchval;
	int x,y;

}GandhitplMatch;

///////////////////////////////////////////////
int globalmaxindex=0;
CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
float maximumtpl[50];

char Notetpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\new(1).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(2).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(3).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(4).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(5).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(6).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(7).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(8).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(9).jpg"};
char Gandhitpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl1000.jpg"};
GandhitplMatch GlobalGandhiji[50];
char NoteFinal[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\1000final3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\500final3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\100final3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\50final3.jpg"};

void gandhijitplMatch(GandhitplMatch,int);
void captureOriginal(IplImage* []);
void processImage(IplImage *,int);
IplImage *NotetplProcessed[50];
float NotetplMatch(IplImage *,int);
void detectGandhiFace(int);
void detectAllFaces( IplImage *img );
float globalmaximum=-999;
int globalcnt=0;
void doBlack(IplImage * []);
int maxnote[5]={0,0,0,0,0};
char denomination[10][50] ={"1000Rs","500Rs","100Rs","50Rs"};
void detectDenomination(IplImage *img,int,int);
////////////////////////////////////////////////////////////////////////////////////


int main( int argc, char** argv )
{
	 
	IplImage  *OriginalFrame[50];
	captureOriginal(OriginalFrame);
	cvNamedWindow("framevideo1",1);

	for(int i=0;i<40;i++)
	{
		printf(" i= %d",i);
	
	processImage(OriginalFrame[i],i);
	cvShowImage("framevideo1",OriginalFrame[i]);
	cvWaitKey(10);
	//cvReleaseImage( &OriginalFrame[i] );
	}


	printf("\nPrinting of Maximum values\n");
	for(int i=0;i<40;i++)
	{

		printf("\n maximum value = %f",maximumtpl[i]);

	}
	for(int i=0;i<40;i++)
	{
			
		cvShowImage("framevideo1",NotetplProcessed[i]);
		cvWaitKey(10);
	}
	for(int i=0;i<40;i++)
	{

		detectGandhiFace(i);

	}


		//OriginalFrame=NULL;
	for(int i=0;i<40;i++)
	{
		if(GlobalGandhiji[i].matchval!=-1)
		{
			cvShowImage("framevideo1",GlobalGandhiji[i].faceimg);
			cvWaitKey(10);
		}
	}
	
		doBlack(OriginalFrame);
		printf(" %d  %d  %d  %d  ",maxnote[0],maxnote[1],maxnote[2],maxnote[3]);
		int tempmax=0;
		for(int i=0;i<4;i++)
		{

			if(maxnote[tempmax]<maxnote[i])
				tempmax=i;
				
		}
		//printf("%s",denomination[tempmax]);
		getch();
	   cvDestroyWindow( "framevideo1" );
	return 0;
}
/////////////////////////////////////////////////////////////

void doBlack(IplImage * img[50])
{
	CvRect *r1;
	 CvScalar s;
	 printf(" cnt = %d", globalcnt);
	 
	for(int i=0;i<40;i++)
	{

		int thresholdminx = (int)(GlobalGandhiji[i].x-(GlobalGandhiji[i].width*172/45));
		int thresholdmaxx = (int)(GlobalGandhiji[i].x+(GlobalGandhiji[i].width*94/45));

 		int thresholdminy =(int)(GlobalGandhiji[i].y-(GlobalGandhiji[i].height*40/45));

		int thresholdmaxy = (int)(GlobalGandhiji[i].y+(GlobalGandhiji[i].height*75/45));

	int tempc=0;

		if(GlobalGandhiji[i].matchval!=-999 && thresholdminx>0 && thresholdminy>0 &&  thresholdmaxy<= img[i]->height && thresholdmaxx<= img[i]->width)
		{
			printf("entered  %d ",(++tempc));
			
			for(int l=0;l<img[i]->height;l++)
			{
		//printf("....");
				for(int m=0;m<img[i]->width;m++)
				{
					if((m<thresholdminx || m> thresholdmaxx)  || (l< thresholdminy || l> thresholdmaxy))
					{ 
					
				
						s = cvGet2D(img[i], l, m);
		//	printf("hi.....");
			//	s.
			//	s.val[3]=0.0;
						s.val[0]=0.0;
						s.val[1]=0.0;
						s.val[2]=0.0;
			
						cvSet2D(img[i], l, m, s );
	
					}
			
			
				}
//		printf("over j");

			}


			cvNamedWindow("showthres",1);
			cvShowImage("showthres",img[i]);
			cvWaitKey(10);
			detectDenomination(img[i],thresholdmaxx-thresholdminx,thresholdmaxy-thresholdminy);
		}

	}

	

}
////////////////////////////////////////////////////////////////////////////////////////

void detectDenomination(IplImage *img,int thresx,int thresy)
{
	CvPoint		minloc, maxloc;
	double		minval, maxval;
float max=-1;
int index=-1;
	for(int i=0; i<4;i++)
	{
		//IplImage *tpl=cvLoadImage(Gandhitpl[i]);
IplImage *source = cvLoadImage(NoteFinal[i]);



 //declare a destination IplImage object with correct size, depth and channels
      IplImage *destination = cvCreateImage
	  ( cvSize( thresx,thresy),source->depth, source->nChannels );

//use cvResize to resize source to a destination image
	cvResize(source, destination);
	IplImage *tpl=destination;
		


		int res_width=img->width-tpl->width+1;
		int res_height=img->height-tpl->height+1;


	IplImage *res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
	/* choose template matching method to be used */
	//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
	//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
	cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );
	cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
		
	cvNamedWindow("showtplmatch",1);
	cvShowImage("showtplmatch",img);
	printf("\n i= %d, max = %f",i,maxval);
	cvWaitKey(100);
	if(max<maxval)
		{
			max=maxval;
			index=i;
		}
	}
	//if(max!=-999)
	maxnote[index]++;
	
}
/////////////////////////////////////////////////////////////////////////////////

void captureOriginal(IplImage* img[50])
{

		CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);

	//assert(capture);
	IplImage *frame;
	int framecount=0;
	cvNamedWindow("framevideo",1);
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

		
cvShowImage("framevideo",frame);
if(framecount>20)
			cvFlip(img[framecount], img[framecount], -1 );
		
framecount++;
				
		if(framecount==40)
			break;
		
			cvWaitKey(200);
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow( "framevideo" );
	//cvReleaseImage( &frame );
	
	

}
//////////////////////////////////////////////////////////////////

void processImage(IplImage *img,int i)
{
	maximumtpl[i]=NotetplMatch(img,i);
	printf("max value = %f",maximumtpl[i]);
	


	


}
////////////////////////////////////////////////////////////////

void gandhijitplMatch(GandhitplMatch detectedimg, int index)
{
	CvPoint		minloc, maxloc;
	double		minval, maxval;
	
	for(int i=0; i<4;i++)
	{
		//IplImage *tpl=cvLoadImage(Gandhitpl[i]);
IplImage *source = cvLoadImage(Gandhitpl[i]);



 //declare a destination IplImage object with correct size, depth and channels
      IplImage *destination = cvCreateImage
	  ( cvSize(detectedimg.width ,detectedimg.height),source->depth, source->nChannels );

//use cvResize to resize source to a destination image
	cvResize(source, destination);
	IplImage *tpl=destination;
		


		int res_width=detectedimg.faceimg->width-tpl->width+1;
		int res_height=detectedimg.faceimg->height-tpl->height+1;


	IplImage *res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
	/* choose template matching method to be used */
	//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
	//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
	cvMatchTemplate( detectedimg.faceimg, tpl, res, CV_TM_CCOEFF_NORMED );
	cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
		
	cvNamedWindow("showtplmatch",1);
	cvShowImage("showtplmatch",detectedimg.faceimg);
	printf("\n i= %d, max = %f",i,maxval);
	cvWaitKey(10);
	if(globalmaximum<maxval)
		{
			globalmaximum=maxval;
			globalmaxindex=index;
		}
	}
	

}
////////////////////////////////////////////////////////////////////////////////////


void detectGandhiFace(int no)
{
		
	char      *filename = ".\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	IplImage  *frame;
 
    /* load the classifier
       note that I put the file in the same directory with
       this code */
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
    /* setup memory buffer; needed by the face detector */
    storage = cvCreateMemStorage( 0 );
    /* initialize camera */
       assert( cascade && storage);
 
		frame=NotetplProcessed[no];
 
        /* always check */
        
 
        /* 'fix' frame */
        //cvFlip( frame, frame, -1 );
        frame->origin = 0;
 
        /* detect faces and display video */
        detectAllFaces( frame );
	printf("Global Maximum of %d = %f ",no,globalmaximum);	


}
///////////////////////////////////////////////////////////////////////////

void detectAllFaces( IplImage *img )
{
	GandhitplMatch detectedimg[10];
	cvNamedWindow("displayface",1);
    int k;
 	int cnt=0;
	 CvRect *r1;
	 CvScalar s;
	 int fl=0;
    /* detect faces */
    CvSeq *faces = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.1,
            1,
            0 /*CV_HAAR_DO_CANNY_PRUNNING*/,
            cvSize( 30, 30 ) );
	globalmaximum=-999;
	globalmaxindex=-1;
    /* for each face found, draw a red box */
    for( k = 0 ; k < ( faces ? faces->total : 0 ) ; k++ ) {
			r1 = ( CvRect* )cvGetSeqElem( faces, k );	
		if((r1->height<100)&&(r1->width<100))
		 {
				fl=1;
				detectedimg[k].faceimg=cvCreateImage(cvGetSize(img),8,3);
				cvCopyImage(img,detectedimg[k].faceimg);
				detectedimg[k].height=r1->height;
				detectedimg[k].width=r1->width;
				detectedimg[k].x=r1->x;
				detectedimg[k].y=r1->y;
				
			//	cvShowImage("displayface",detectedimg[k].faceimg);
				cvWaitKey(10);
	
				
			/*cvRectangle( img,
						 cvPoint( r1->x, r1->y ),
						 cvPoint( r1->x + r1->width, r1->y + r1->height ),
						 CV_RGB( 255, 0, 0 ), 1, 8, 0 ); */
				//printf("facedetection called");
				printf("width= %d height= %d",detectedimg[k].faceimg->width,detectedimg[k].faceimg->height);
		for(int i=0;i<img->height;i++)
	{
		//printf("....");
		for(int j=0;j<img->width;j++)
		{
		//	printf("hi %d",j);
			if((j<r1->x || j>r1->x + r1->width)  || (i<r1->y || i>r1->y + r1->height))
			{ 
					
				
				s = cvGet2D(detectedimg[k].faceimg, i, j);
		//	printf("hi.....");
			//	s.
			//	s.val[3]=0.0;
				s.val[0]=0.0;
				s.val[1]=0.0;
				s.val[2]=0.0;
			
	cvSet2D(detectedimg[k].faceimg, i, j, s );
	
			}
			
			
		}
//		printf("over j");

	}

	cvShowImage("displayface",detectedimg[k].faceimg);
	cvWaitKey(10);
		




			printf("width %d height  %d\n",r1->width,r1->height);
		}
		gandhijitplMatch(detectedimg[k],k);	
 
	}
	if(faces->total>0 && globalmaximum>0.62)
	{
		GlobalGandhiji[globalcnt].faceimg=detectedimg[globalmaxindex].faceimg;
		GlobalGandhiji[globalcnt].x=detectedimg[globalmaxindex].x;
		GlobalGandhiji[globalcnt].y=detectedimg[globalmaxindex].y;
		GlobalGandhiji[globalcnt].width=detectedimg[globalmaxindex].width;
		GlobalGandhiji[globalcnt].height=detectedimg[globalmaxindex].height;
		GlobalGandhiji[globalcnt].matchval=globalmaximum;

	}
	else
	{
		GlobalGandhiji[globalcnt].matchval=-999;

	}
	globalcnt++;
    /* display video */
    cvShowImage( "video", img );
	cvWaitKey(10);
}
//////////////////////////////////////////////////////////////////////////////////////////

float NotetplMatch(IplImage *img1,int no)
{
	NotetplProcessed[no]=cvCreateImage(cvGetSize(img1),8,3);
	
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
	cvNamedWindow( "reference", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "template", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("result",CV_WINDOW_AUTOSIZE);
	img=cvCreateImage(cvGetSize(img1),8,3);
	for(int km=0;km<9;km++)
	{
		cvCopy(img1,img,NULL);
		tpl = cvLoadImage(Notetpl[km]);;
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
	cvRectangle( img, 
				 cvPoint( maxloc.x, maxloc.y ), 
				 cvPoint( maxloc.x + tpl_width, maxloc.y + tpl_height ),
				 cvScalar( 0, 0, 255, 0 ), 1, 0, 0 );	
		CvScalar s;
		
		printf("\nminval= %f  maxval= %f  ",minval,maxval);
		for(int i=1;i<img_height;i++)
		{
			for(int j=1;j<img_width;j++)
			{

				if((j<maxloc.x+(tpl_width/2) || j>maxloc.x + tpl_width)  || (i<maxloc.y || i>maxloc.y + tpl_height))
				{	 
					s = cvGet2D(img, i, j);
					s.val[0]=0.0;
					s.val[1]=0.0;
					s.val[2]=0.0;
				
					cvSet2D(img, i, j, s );

				}
			

			}

		}
		if(maxval>maxi)
		{
				maxi=maxval;
				cvCopyImage(img,NotetplProcessed[no]);
				
		}
	

		cvShowImage( "reference", img );
		cvShowImage( "template", tpl );
		cvShowImage("result",res);
		cvWaitKey(10);	
	}
	printf("\n\nWaiting for next image to load\n");
	cvDestroyWindow( "reference" );
	cvDestroyWindow( "template" );
	cvDestroyWindow( "result" );


	return maxi;
}
