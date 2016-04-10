#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <cvaux.h>
#include <cxcore.h>
#include <conio.h>




typedef struct GandhitplMatch
{
	IplImage *faceimg;
	int height;
	int width;
	float matchval;
	int x,y;

}GandhitplMatch;
int globalmaxindex=0;
void playSoundNumber(int number);

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
float maximumtpl[50];
char Notetpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\new(1).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(2).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(3).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(4).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(5).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(6).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(7).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(8).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(9).jpg"};
char Gandhitpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl1000.jpg"};
GandhitplMatch GlobalGandhiji[50];
char NoteFinal[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\1000final.jpg","C:\\tv\\images\\mixed\\sampletemplate\\500review3morning.jpg","C:\\tv\\images\\mixed\\sampletemplate\\100finalreview3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\50finalreview3.jpg"};
char Numbertpl[10][150]={"C:\\tv\\images\\mixed\\sampletemplate\\numbers\\1000number.jpg","C:\\tv\\images\\mixed\\sampletemplate\\numbers\\500numberreview3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\numbers\\100numberreview3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\numbers\\50numberreview3.jpg"};
IplImage *CroppedImage[50];  // used for cropping whole note
IplImage *CroppedNumber[50]; //used for cropping only the middle number portion  
void gandhijitplMatch(GandhitplMatch,int);
void captureOriginal(IplImage* []);
void processImage(IplImage *,int);
IplImage *NotetplProcessed[50];
float NotetplMatch(IplImage *,int);
void detectGandhiFace(int);
void detectAllFaces( IplImage *img );
IplImage* cropRectangle(IplImage *img, CvRect region);
void sendtoHoughLines(IplImage *);
int croppedimgcnt=0;
float globalmaximum=-999;
int globalcnt=0;
int Notes[4]={1000,500,100,50};
void doBlack(IplImage * []);
int maxnote[5]={0,0,0,0,0};   // array used for total note matching
int maxnumber[5]={0,0,0,0,0}; //array used for only number matching
void detectDenomination(IplImage *img,int,int);
void numberMatch();
int main( int argc, char** argv )
{
	//--------------------Capturing the 20frames of notes in Original Frame IplImage array----------- 
	IplImage  *OriginalFrame[50];

	//--------CALL TO THE FUNCTION TO PERFORM CAPTURE OPERATION--------------------
	captureOriginal(OriginalFrame);
	cvNamedWindow("framevideo1",1);

	//--------EACH FRAME UNDERGOES SOME OPERATIONS USING FOLLOWING LOOP---------
	for(int i=0;i<21;i++)
	{
		//printf(" i= %d",i);

		processImage(OriginalFrame[i],i);

	//	cvShowImage("framevideo1",OriginalFrame[i]);
//		cvWaitKey(100);
	//cvReleaseImage( &OriginalFrame[i] );
	}

//-------------------------------------------------------------------------------
	//to print max matched value for template --commented
	/*printf("\nPrinting of Maximum values\n");
	for(int i=0;i<21;i++)
	{

		printf("\n maximum value = %f",maximumtpl[i]);

	}*/
	//-----------------------------------------------------------------
	//-------THE NotetplProcessed IMAGE ARE DISPLAYED (I.E THE HALF RIGHT PORTION ) IN FRAMEVIDEO1 WINDOW-------------
	

	//---------ALL THE NotetplProcessed IMAGES ARE SEND TO DETECT THE FACE OF GANDHIJI-------------
	for(int i=0;i<21;i++)
	{
		
		detectGandhiFace(i);

	}


		//OriginalFrame=NULL;
	//-----ALL THE IMAGES WHO PASSED ARE SEND FOR FURTHER PROCESSING----------
	
		//--------EXTRACT THE IMAGE PERFECTLY AND DETECT IT--------
		doBlack(OriginalFrame);
		for(int i=0;i<croppedimgcnt;i++)
		{
			//cvNamedWindow("croppedimage",1);
			//cvShowImage("croppedimage",CroppedImage[i]);
			//sendtoHoughLines(CroppedImage[i]);
			//cvWaitKey(500);
		}
		printf("Answer by first attempt %d  %d  %d  %d  ",maxnote[0],maxnote[1],maxnote[2],maxnote[3]);
		
		

		//cvWaitKey(5000);

		numberMatch();
		printf("Answer  attempt %d  %d  %d  %d  ",maxnumber[0],maxnumber[1],maxnumber[2],maxnumber[3]);

		
		int FinalDenomination[4];
		for(int p=0;p<4;p++)
		{
			FinalDenomination[p]=maxnote[p]+maxnumber[p];

		}

		printf("Answer addition  attempt %d  %d  %d  %d  ",FinalDenomination[0],FinalDenomination[1],FinalDenomination[2],FinalDenomination[3]);

		int index_1=0;
		//int maxfinal=-1;
		for(int p=0;p<4;p++)
		{
			if(FinalDenomination[index_1]<FinalDenomination[p])
			{
				index_1=p;
			}

		}
		if(Notes[index_1]!=0)
		playSoundNumber(Notes[index_1]);
		getch();
	   cvDestroyWindow( "framevideo1" );
	return 0;
}
/////////////////////////////////////////////////////////////
void doBlack(IplImage * img[50])
{
	CvRect *r1;
	 CvScalar s;
	 //printf(" cnt = %d", globalcnt);
	 
	for(int i=0;i<21;i++)
	{

		int thresholdminx = (int)(GlobalGandhiji[i].x-(GlobalGandhiji[i].width*172/45));
		int thresholdmaxx = (int)(GlobalGandhiji[i].x+(GlobalGandhiji[i].width*94/45));

 		int thresholdminy =(int)(GlobalGandhiji[i].y-(GlobalGandhiji[i].height*40/45));

		int thresholdmaxy = (int)(GlobalGandhiji[i].y+(GlobalGandhiji[i].height*75/45));
		
		

	int tempc=0;

		if(GlobalGandhiji[i].matchval!=-1 && thresholdminx>0 && thresholdminy>0 &&  thresholdmaxy<= img[i]->height && thresholdmaxx<= img[i]->width)
		{
			CvRect rectcrop;
			rectcrop.x=thresholdminx;
			rectcrop.y=thresholdminy;
			rectcrop.width=thresholdmaxx-thresholdminx;
			rectcrop.height=thresholdmaxy-thresholdminy;
			CroppedImage[croppedimgcnt] = cropRectangle(img[i], rectcrop);
			croppedimgcnt++;
			
			++tempc;
			
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


			//cvNamedWindow("showthres",1);
			//cvShowImage("showthres",img[i]);
			
			//cvWaitKey(500);

			//---------FIRST ATTEMPT TO DETERMINE THE DENOMINATION OF THE NOTE---------
			detectDenomination(img[i],thresholdmaxx-thresholdminx,thresholdmaxy-thresholdminy);
		}

	}

	

}
//--------------------------------HOUGHLINES---------------------------------
void sendtoHoughLines(IplImage * img)
{


	IplImage* src = cvCreateImage(
	cvGetSize(img), 8, 1 );
	cvCvtColor(img, src, CV_RGB2GRAY);

	//-------------CREATING A IMAGE NAMED dst FOR EDGE SHOWING ON IT FROM CANNY RESULT--------------------
	IplImage* dst = cvCreateImage(
	cvGetSize(src), 8, 1 );

	//---------------CREATING color_dst IMAGE FOR RESULT OF LINE DISPLAY PURPOSE---------------------------
	IplImage *color_dst = cvCreateImage(
	cvGetSize(src), 8, 3 );

	//---------------CEATING A STORE POOL FOR LINE PURPOSE AND LINE DECLARATIVE PARAMETER-------------
	CvMemStorage* storage =
	cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i;

	//---------------APPLYING THE CANNY FUNCTION ON SRC AND STORING THE EDGE RESULTS IN DST-------------
	cvCanny( src, dst,30, 90, 3 );

	cvDilate( dst, dst, 0, 1 );

	//--------------CONVERTING THE CANNY RESULT IMAGE DST INTO RGB AND STORE IT IN COLORDST AND SHOWING A WINDOW OF IT--------------
	
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );


	/*
	//-----hough lines algo--------
	lines = cvHoughLines2( dst, storage,
					  CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 30, 30, 0 );

			//---------------ACCESING THE POINTS OF THE LINES AND DRAWING THOSE LINES ON THE GRAY TO RGB CONVERTED IMAGE NAMED color_dst-------------- 
	for( i = 0; i < lines->total; i++ )
	{
	CvPoint* line =
	(CvPoint*)cvGetSeqElem(lines,i);
	cvLine( color_dst, line[0], line[1],

		   CV_RGB(255,0,0), 3, 8 );
	//printf("\n i = %d x1 = %d y1 = %d x2 = %d y2 = %d ",i,line[0].x,line[0].y,line[1].x,line[1].y);


	}
	*/


	lines = cvHoughLines2( dst, storage,
			CV_HOUGH_STANDARD, 1, CV_PI/180, 30, 0, 0 );
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
	cvNamedWindow("HoughLinesShow",1);
	cvShowImage("HoughLinesShow",color_dst);
	cvWaitKey(1000);




}


//------------FUNCTION USED FOR MATCHING THE NUMBER IN THE NOTE------------
void numberMatch()
{

			for(int i=0;i<croppedimgcnt;i++)
			{
			//	printf("entered for %d",globalcnt);
				int minthreshx=(60*CroppedImage[i]->width/177);
				int maxthreshx =(106*CroppedImage[i]->width/147);
				int minthreshy =(22*CroppedImage[i]->height/72);
				int maxthreshy = (42*CroppedImage[i]->height/72);
			//	printf("inside for after initialization function numbermatch()");
				CvRect rectcrop;
				rectcrop.x=minthreshx;
				rectcrop.y=minthreshy;
				rectcrop.width=maxthreshx-minthreshx;
				rectcrop.height=maxthreshy-minthreshy;
				CroppedNumber[i] = cropRectangle(CroppedImage[i], rectcrop);
			//	printf("inside for after cropping function numbermatch()");
				//cvNamedWindow("croppednumber",1);
				//cvShowImage("croppednumber",CroppedNumber[i]);
				//cvWaitKey(5);
			
			

			//--------------NUMBER TEMPLATE MATCH---------------

			CvPoint		minloc, maxloc;
			double		minval, maxval;
			float max=-1;
			int index=-1;
			//--------4 TEMPLATES Number  OF Numbertpl----------
			for(int j=0; j<4;j++)
			{
				//IplImage *tpl=cvLoadImage(Gandhitpl[i]);
				IplImage *source = cvLoadImage(Numbertpl[j]);
				int tempheight=CroppedImage[i]->height*16/72;
				int tempwidth=source->width*tempheight/source->height;
				//declare a destination IplImage object with correct size, depth and channels
				IplImage *destination = cvCreateImage
				( cvSize( tempwidth,tempheight),source->depth, source->nChannels );

				//use cvResize to resize source to a destination image
				cvResize(source, destination);

				//cvNamedWindow("sourcenumber",1);
				//cvNamedWindow("destnumber",1);
				//cvShowImage("sourcenumber",source);
				//cvShowImage("destnumber",destination);

				//cvWaitKey(10);
				//cvDestroyWindow("sourcenumber");
				//cvDestroyWindow("destnumber");
				IplImage *tpl=destination;
		


				int res_width=CroppedNumber[i]->width-tpl->width+1;
				int res_height=CroppedNumber[i]->height-tpl->height+1;


				IplImage *res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
				/* choose template matching method to be used */
				//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
				//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
				//cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
				//cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
				//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
				cvMatchTemplate( CroppedNumber[i], tpl, res, CV_TM_CCOEFF_NORMED );
				cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);
		
				//cvNamedWindow("showtplmatch",1);
				//cvShowImage("showtplmatch",img);
				//printf("\n i= %d, max = %f",i,maxval);
				//	cvWaitKey(100);
				if(max<maxval)
					{
						max=maxval;
						index=j;
					}
				//cvReleaseImage( &source);
				//cvReleaseImage( &destination );
				}
	
	// ----- THE TPL NOTE FOR WHICH THE MATCH WAS MAXIMUM , THAT INDEX COUNT IS INCREMENTED 
				
			maxnumber[index]++;
			}
}


//------FUNCTION USED TO DETERMINE DENOMINATION----------
void detectDenomination(IplImage *img,int thresx,int thresy)
{
	CvPoint		minloc, maxloc;
	double		minval, maxval;
float max=-1;
int index=-1;
	//--------4 TEMPLATES NOTES  OF NoteFinal----------
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
		
	//cvNamedWindow("showtplmatch",1);
	//cvShowImage("showtplmatch",img);
//	printf("\n i= %d, max = %f",i,maxval);
//	cvWaitKey(100);
	if(max<maxval)
		{
			max=maxval;
			index=i;
		}
	}
	
	// ----- THE TPL NOTE FOR WHICH THE MATCH WAS MAXIMUM , THAT INDEX COUNT IS INCREMENTED 
	maxnote[index]++;
	
}

//-----------FUNCTION TO GRAB FRAMES-----------
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
		//-------to show the grabbed frame using framevideo window-----
		cvShowImage("framevideo",frame);
		framecount++;
		
		if(framecount==21)
			break;
	
		cvWaitKey(500);
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow( "framevideo" );
	//cvReleaseImage( &frame );
	
	

}
//////////////////////////////////////////////////////////////////

void processImage(IplImage *img,int i)
{
	//--------------SEND THE ORIGINAL IMAGE TO TEMPLATE MATCH USING FUNTION NotetplMatch--------
	maximumtpl[i]=NotetplMatch(img,i);
	
	//---------THE RETURNED MAXIMUM VALUE FOR FOLLOWING INDEX OF IMAGE FROM NotetplMatch FUNCTION---------
//	printf("max value = %f",maximumtpl[i]);
	


	


}
////////////////////////////////////////////////////////////////
//-------THIS FUNCTION MATCHES THE DETECTED FACES WITH GANDHIJI'S TEMPLATE TO FIND THE EXACT HIGHEST FACE MATCH-----

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
		
//	cvNamedWindow("showtplmatch",1);
//	cvShowImage("showtplmatch",detectedimg.faceimg);
//	printf("\n i= %d, max = %f",i,maxval);
//	cvWaitKey(100);
	if(globalmaximum<maxval)
		{
			globalmaximum=maxval;
			globalmaxindex=index;
		}
	}
	

}
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
		// ---------- NOW THE FACES DETECT ARE SENT FOR A TEMPLATE MATCH WITH FACE OF GANDHIJI USING detectallfaces FUNCTION-------------- 
        detectAllFaces( frame );
		//printf("Global Maximum of %d = %f ",no,globalmaximum);	


}
//-------DETECT FACE OF GADHIJI ---------------
void detectAllFaces( IplImage *img )
{
	// -------THIS STRUCTURE TO GIVES DETAILS OF DETECTED FACE----------- 
	GandhitplMatch detectedimg[10];
	//cvNamedWindow("displayface",1);
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
				//cvWaitKey(100);
	
				
			/*cvRectangle( img,
						 cvPoint( r1->x, r1->y ),
						 cvPoint( r1->x + r1->width, r1->y + r1->height ),
						 CV_RGB( 255, 0, 0 ), 1, 8, 0 ); */
				//printf("facedetection called");
//				printf("width= %d height= %d",detectedimg[k].faceimg->width,detectedimg[k].faceimg->height);
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

	//cvShowImage("displayface",detectedimg[k].faceimg);
	//cvWaitKey(10);
		




//			printf("width %d height  %d\n",r1->width,r1->height);
		}
		//-------SEND THE DETECTED FACE TO MATCH WITH FACE OF GANDHIJI---------
		gandhijitplMatch(detectedimg[k],k);	
 
	}
		//------KEEP THE MATCHED IMAGE WHOSE MATCH IS GREATER THAN 0.62-----------
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
		GlobalGandhiji[globalcnt].matchval=-1;//TO ELIMINATE THE IMAGES

	}
	globalcnt++;
    /* display video */
  //  cvShowImage( "video", img );
	//cvWaitKey(100);
}

//--------THIS FUNCTION PERFORMS A TEMPLATE MATCH ON THE ORIGINAL IMAGE TO CROP THE IMAGE FROM BACKGROUND-----------
//-------IN ORDER TO IDENTIFY THE REGION WHERE THE NOTE IS PRESENT----------------
float NotetplMatch(IplImage *img1,int no)
{

	//------------NotetplProcessed STORES THE IMAGE EXTRACTED FROM BACKGROUND-------------
	NotetplProcessed[no]=cvCreateImage(cvGetSize(img1),8,3);

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
	//cvNamedWindow( "reference", CV_WINDOW_AUTOSIZE );
	//cvNamedWindow( "template", CV_WINDOW_AUTOSIZE );
	//cvNamedWindow("result",CV_WINDOW_AUTOSIZE);
	
	//-------TEMPORARY IMAGE-COLOR--------------
	img=cvCreateImage(cvGetSize(img1),8,3);

	//-----------THIS LOOP MATCHES THE IMAGE WITH 9 DIFFERENT TEMPLATES AS STORED IN THE GLOBAL 
	// (continued)VARIBLE Notetpl[]----------------
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
			
	//	printf("\nminval= %f  maxval= %f  ",minval,maxval);

		//------AS WE NEED ONLY HALF PART(RIGHT PART ) OF NOTE WHERE GANDHIJI IS PRESENT THIS LOOP ONLY BRINGS IN FOCUS THAT AREA 
		// (continued) OTHER PORTION IS BLACKENED--------
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

		//---------COMPARES AMONGST NINE TEMPLATES WHICH IS MAX MATCHED TEMPLATE--------
		if(maxval>maxi)
		{
				maxi=maxval;
				cvCopyImage(img,NotetplProcessed[no]);
				
		}
	

		//cvShowImage( "reference", img );
		//cvShowImage( "template", tpl );
		//cvShowImage("result",res);
		//cvWaitKey(100);	
	}
	//printf("\n\nWaiting for next image to load\n");
	//cvDestroyWindow( "reference" );
	//cvDestroyWindow( "template" );
	//cvDestroyWindow( "result" );


	return maxi;
}
IplImage* cropRectangle(IplImage *img, CvRect region)
{
	IplImage *imageTmp,*imageRGB;
	CvSize size;
	size.height = img->height;
	size.width = img-> width;

	if (img->depth != IPL_DEPTH_8U) {
		//std::cerr << "ERROR: Unknown image depth of " << img->depth << " given in cropRectangle() instead of 8." << std::endl;
		exit(1);
	}

	// First create a new (color or greyscale) IPL Image and copy contents of img into it.
	imageTmp = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(img, imageTmp);

	// Create a new image of the detected region
	//printf("Cropping image at x = %d, y = %d...", faces[i].x, faces[i].y);
	//printf("Setting region of interest...");
	// Set region of interest to that surrounding the face
	cvSetImageROI(imageTmp, region);
	// Copy region of interest (i.e. face) into a new iplImage (imageRGB) and return it
	size.width = region.width;
	size.height = region.height;
	imageRGB = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(imageTmp, imageRGB);	// Copy just the region.

    cvReleaseImage( &imageTmp );
	return imageRGB;		
}
void playSoundNumber(int number)
{

	char a[2];
		int digit[5];
	int cntplay=0;
	while(number>0)
	{
		digit[cntplay]=number%10;
		number=number/10;
		cntplay++;



	}
	
	for(int i=cntplay-1;i>=0;i--)
	{
		char address[500]="c:\\tv\\vlc\\vlc.exe --play-and-exit ";

		char temp[50]="c:\\tv\\sounds\\";
		a[0]=digit[i]+48;
		a[1]='\0';
		strcat(temp,a);
		strcat(address,temp);
		strcat(address,".mp3 ");
		
		//system("exit");
		//system("taskkill /IM wmplayer.exe");

		//PlaySound(TEXT("c:\\tv\\love.mp3"),NULL,SND_FILENAME);
		printf("%s",address);
		system(address);

		//cvWaitKey(5000);
		
	}
	


}
