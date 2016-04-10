#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <cvaux.h>
#include <cxcore.h>
#include <conio.h>


int globalmaxindex=0;
int tplindex=-1;
float maxval=-1;
char NoteFinal[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\1000final.jpg","C:\\tv\\images\\mixed\\sampletemplate\\500review3morning.jpg","C:\\tv\\images\\mixed\\sampletemplate\\100finalreview3.jpg","C:\\tv\\images\\mixed\\sampletemplate\\50finalreview3.jpg"};
void detectDenomination(IplImage *img,int thresx,int thresy);
typedef struct GandhitplMatch
{
	IplImage *faceimg;
	int height;
	int width;
	float matchval;
	int x,y;

}GandhitplMatch;


CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
float maximumtpl;
char Notetpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\new(1).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(2).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(3).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(4).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(5).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(7).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(8).jpg","C:\\tv\\images\\mixed\\sampletemplate\\new(9).jpg"};
//char Notetpl[10][100]={"C:\\tv\\images\\mixed\\frntnote\\new(1).jpg","C:\\tv\\images\\mixed\\frntnote\\new(2).jpg","C:\\tv\\images\\mixed\\frntnote\\new(3).jpg","C:\\tv\\images\\mixed\\frntnote\\new(4).jpg","C:\\tv\\images\\mixed\\frntnote\\new(5).jpg","C:\\tv\\images\\mixed\\frntnote\\new(6).jpg","C:\\tv\\images\\mixed\\frntnote\\new(7).jpg","C:\\tv\\images\\mixed\\frntnote\\new(8).jpg"};

char Gandhitpl[10][100]={"C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl50.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl100.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl500.jpg","C:\\tv\\images\\mixed\\sampletemplate\\gandhitpl1000.jpg"};
GandhitplMatch GlobalGandhiji;
void gandhijitplMatch(GandhitplMatch,int);
void processImage(IplImage *);
IplImage *NotetplProcessed;
float NotetplMatch(IplImage *);
void detectGandhiFace();
IplImage* cropRectangle(IplImage *img, CvRect region);
void detectAllFaces( IplImage *img );
void rotate(IplImage *img);
float globalmaximum=-999;
int globalcnt=0;
CvRect detectRupee(IplImage * camImg);
CvRect doBlack(IplImage *);
CvRect detectRupee(IplImage * camImg)
{
	//--------------------Capturing the 20frames of notes in Original Frame IplImage array----------- 
	IplImage  *OriginalFrame;
	CvRect rect;
	printf("2called...");
	int k=0;
	OriginalFrame=camImg;
	while(k<2)
	{
	
	processImage(OriginalFrame);
	detectGandhiFace();
//	rotate(OriginalFrame);
	rect=doBlack(OriginalFrame);
	if(rect.width!=0)
		break;
	else
		cvFlip(camImg,OriginalFrame,-1);

	k++;
	}
	return rect;
	
}

void rotate(IplImage *img)
{
	CvRect rect;
	IplImage *imgLine;
	rect.x=GlobalGandhiji.x+GlobalGandhiji.width;
	rect.y=GlobalGandhiji.y-5;
	rect.width=(int)((GlobalGandhiji.width)-5);
	rect.height=GlobalGandhiji.height+15;
	if(GlobalGandhiji.matchval!=-1 && rect.x>0 && rect.y>0 &&  rect.y+rect.height<= img->height && rect.x+rect.width<= img->width)
	{	
	
		imgLine=cropRectangle(img,rect);
		cvNamedWindow("imgLine",1);
		cvShowImage("imgLine",imgLine);
		IplImage* src1 = cvCreateImage(
		cvGetSize(imgLine), 8, 1 );

		cvCvtColor( imgLine, src1, CV_RGB2GRAY );

		IplImage* dst = cvCreateImage(
		cvGetSize(src1), 8, 1 );
		IplImage* color_dst = cvCreateImage(
		cvGetSize(src1), 8, 3 );
		CvMemStorage* storage =
		cvCreateMemStorage(0);
		CvSeq* lines = 0;
		int i;
		cvCanny( src1, dst,50, 150, 3 );
		//cvDilate( dst, dst, 0, 1 );
		cvNamedWindow("edgedest",1);
		cvShowImage("edgedest",dst);
		cvCvtColor( dst, color_dst, CV_GRAY2BGR );
		#if 1
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
		#else
		lines = cvHoughLines2( dst, storage,
		CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 30, 0, 0 );
		for( i = 0; i < lines->total; i++ )
		{
		CvPoint* line =
		(CvPoint*)cvGetSeqElem(lines,i);
		cvLine( color_dst, line[0], line[1],
		CV_RGB(255,0,0), 3, 8 );
		}
		#endif
		
		cvNamedWindow( "Hough", 1 );
		cvShowImage( "Hough", color_dst );



	}
	


	/*
	




*/




}
/////////////////////////////////////////////////////////////
CvRect doBlack(IplImage * img)
{
	CvRect *r1;
	CvRect rectcrop;
	 CvScalar s;
	 //printf(" cnt = %d", globalcnt);
	
		int thresholdminx = (int)(GlobalGandhiji.x-(GlobalGandhiji.width*172/45));
		int thresholdmaxx = (int)(GlobalGandhiji.x+(GlobalGandhiji.width*94/45));

 		int thresholdminy =(int)(GlobalGandhiji.y-(GlobalGandhiji.height*40/45));

		int thresholdmaxy = (int)(GlobalGandhiji.y+(GlobalGandhiji.height*75/45));
		
		

		int tempc=0;
		
		if(GlobalGandhiji.matchval!=-1 && thresholdminx>0 && thresholdminy>0 &&  thresholdmaxy<= img->height && thresholdmaxx<= img->width)
		{
			printf("gandhi detected....");
			
			rectcrop.x=thresholdminx+20;
			rectcrop.y=thresholdminy;
			rectcrop.width=thresholdmaxx-thresholdminx-20;
			rectcrop.height=thresholdmaxy-thresholdminy;
			detectDenomination(img,thresholdmaxx-thresholdminx,thresholdmaxy-thresholdminy);
			
		}
		else
		{
			rectcrop.width=0;

		}

	printf("%d ",rectcrop.width);
return rectcrop;
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
						tplindex=i;
					}
	}
	printf("\n index matched: %d\n",tplindex);
	
}


void processImage(IplImage *img)
{
	//--------------SEND THE ORIGINAL IMAGE TO TEMPLATE MATCH USING FUNTION NotetplMatch--------
	printf("Entered ProcessImage");
	maximumtpl=NotetplMatch(img);
	
	//---------THE RETURNED MAXIMUM VALUE FOR FOLLOWING INDEX OF IMAGE FROM NotetplMatch FUNCTION---------
//	printf("max value = %f",maximumtpl[i]);
	


	


}
////////////////////////////////////////////////////////////////
//-------THIS FUNCTION MATCHES THE DETECTED FACES WITH GANDHIJI'S TEMPLATE TO FIND THE EXACT HIGHEST FACE MATCH-----

void gandhijitplMatch(GandhitplMatch detectedimg,int index)
{

	CvPoint		minloc, maxloc;
	double		minval, maxval;
	
	for(int i=0; i<4;i++)
	{
		//IplImage *tpl=cvLoadImage(Gandhitpl[i]);
IplImage *source = cvLoadImage(Gandhitpl[i]);

	printf("gandhitplmatchloaded");

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
//	printf("\n i= %d, max = %f",i,maxval);
//	cvWaitKey(100);
	if(globalmaximum<maxval)
		{
			globalmaximum=maxval;
			globalmaxindex=index;
		}
	}
	

}
void detectGandhiFace()
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
 
		frame=NotetplProcessed;
 
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
	printf("hello");
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
				cvNamedWindow("displayface",1);
	
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
				
	//			cvShowImage("displayface",detectedimg[k].faceimg);
				//cvWaitKey(100);
	
				
			/*cvRectangle( img,
						 cvPoint( r1->x, r1->y ),
						 cvPoint( r1->x + r1->width, r1->y + r1->height ),
						 CV_RGB( 255, 0, 0 ), 1, 8, 0 ); */
				//printf("facedetection called");
//				printf("width= %d height= %d",detectedimg[k].faceimg->width,detectedimg[k].faceimg->height);
		/*for(int i=0;i<img->height;i++)
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

	}*/

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
		GlobalGandhiji.faceimg=detectedimg[globalmaxindex].faceimg;
		GlobalGandhiji.x=detectedimg[globalmaxindex].x;
		GlobalGandhiji.y=detectedimg[globalmaxindex].y;
		GlobalGandhiji.width=detectedimg[globalmaxindex].width;
		GlobalGandhiji.height=detectedimg[globalmaxindex].height;
		GlobalGandhiji.matchval=globalmaximum;

	}
	else
	{
		GlobalGandhiji.matchval=-1;//TO ELIMINATE THE IMAGES

	}
    /* display video */
  //  cvShowImage( "video", img );
	//cvWaitKey(100);
}

//--------THIS FUNCTION PERFORMS A TEMPLATE MATCH ON THE ORIGINAL IMAGE TO CROP THE IMAGE FROM BACKGROUND-----------
//-------IN ORDER TO IDENTIFY THE REGION WHERE THE NOTE IS PRESENT----------------
float NotetplMatch(IplImage *img1)
{

	
	//------------NotetplProcessed STORES THE IMAGE EXTRACTED FROM BACKGROUND-------------
	printf("\nNoteTplMatch");
	NotetplProcessed=cvCreateImage(cvGetSize(img1),8,3);

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
	for(int km=0;km<8;km++)
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
				cvCopyImage(img,NotetplProcessed);
				
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
