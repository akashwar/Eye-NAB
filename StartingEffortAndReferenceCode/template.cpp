/*
 * TemplateMatching 1.0
 *
 * @author 	  Nashruddin Amin <me@nashruddin.com>
 * @version   1.0
 * @website	  http://www.nashruddin.com
 */

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <conio.h>
int i,j;
int main( int argc, char** argv )
{
	IplImage	*img;
	IplImage	*tpl;
	IplImage	*res;
	CvPoint		minloc, maxloc;
	double		minval, maxval;
	int			img_width, img_height;
	int			tpl_width, tpl_height;
	int			res_width, res_height;
	char tplname[10][50]={"C:\\tv\\images\\mixed\\bright(1).jpg","C:\\tv\\images\\mixed\\bright(2).jpg","C:\\tv\\images\\mixed\\bright(4).jpg","C:\\tv\\images\\mixed\\bright(5).jpg"};
	//IplImage *source = cvLoadImage( "C:\\tv\\images\\mixed\\g.jpg");
// Here we retrieve a percentage value to a integer


// declare a destination IplImage object with correct size, depth and channels
      /*IplImage *destination = cvCreateImage
( cvSize(43 ,43),source->depth, source->nChannels );
*/
//use cvResize to resize source to a destination image
//cvResize(source, destination);
	//char tplname[10][50]=
	char imagename[10][50]={"C:\\tv\\images\\mixed\\sample(2).jpg","C:\\tv\\images\\mixed\\sample(3).jpg","C:\\tv\\images\\mixed\\sample(4).jpg","C:\\tv\\images\\mixed\\sample(5).jpg","C:\\tv\\images\\mixed\\sample(6).jpg","C:\\tv\\images\\mixed\\sample(7).jpg","C:\\tv\\images\\mixed\\sample(8).jpg","C:\\tv\\images\\mixed\\sample(9).jpg",};
	/* check for arguments */
	float maxi=-1.0;
for(int lm=0;lm<1;lm++)
{
	
	/* load reference image */
	img = cvLoadImage(imagename[lm]);

	
	/* always check */
	if( img == 0 ) {
		fprintf( stderr, "Cannot load file %s!\n", argv[1] );
		return 1; 
	}

	for(int km=0;km<4;km++)
	{
			img = cvLoadImage(imagename[lm]);
	/* load template image */
		//img = cvLoadImage("C:\\tv\\images\\phine\\100rs\\100rs2.jpg");

		tpl = cvLoadImage(tplname[km]);;
	
	
/*IplImage *img = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U,1);
cvCvtColor(img1,img,CV_RGB2GRAY);
IplImage *tpl = cvCreateImage(cvGetSize(tpl1),IPL_DEPTH_8U,1);
cvCvtColor(tpl1,tpl,CV_RGB2GRAY);*/


	/* always check */
	if( tpl == 0 ) {
		fprintf( stderr, "Cannot load file %s!\n", argv[2] );
		return 1;
	}

	/* get image's properties */
	img_width  = img->width;
	img_height = img->height;
	tpl_width  = tpl->width;
	tpl_height = tpl->height;
	res_width  = img_width - tpl_width + 1;
	res_height = img_height - tpl_height + 1;

	/* create new image for template matching computation */
	res = cvCreateImage( cvSize( res_width, res_height ), IPL_DEPTH_32F, 1 );
//printf("NIKHIL");
	/* choose template matching method to be used */
	//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF );
	
//cvMatchTemplate( img, tpl, res, CV_TM_SQDIFF_NORMED );
	// cvMatchTemplate( img, tpl, res, CV_TM_CCORR );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCORR_NORMED );
	//cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF );
	cvMatchTemplate( img, tpl, res, CV_TM_CCOEFF_NORMED );
//printf("matched");
	cvMinMaxLoc( res, &minval, &maxval,&minloc,&maxloc, 0);

	/* draw red rectangle */
	cvRectangle( img, 
				 cvPoint( maxloc.x, maxloc.y ), 
				 cvPoint( maxloc.x + tpl_width, maxloc.y + tpl_height ),
				 cvScalar( 0, 0, 255, 0 ), 1, 0, 0 );	
		CvScalar s;
		double d=26.999999;
		printf("\nminval= %f  maxval= %f   d=%f ",minval,maxval,d);
	for(i=1;i<img_height;i++)
	{
		for(j=1;j<img_width;j++)
		{

			if((j<maxloc.x || j>maxloc.x + tpl_width)  || (i<maxloc.y || i>maxloc.y + tpl_height))
			{ 
				//printf("hi..");
				s = cvGet2D(img, i, j);
		//	printf("hi.....");
			//	s.
			//	s.val[3]=0.0;
				s.val[0]=0.0;
				s.val[1]=0.0;
				s.val[2]=0.0;
			
	cvSet2D(img, i, j, s );

			}
			

		}

	}
if(maxval>maxi)
	{//printf("hi.................................");
	cvSaveImage("C:\\tv\\images\\phine\\500rs\\result.jpg",img,0);
	maxi=maxval;
	}
	
	/* display images */
	cvNamedWindow( "reference", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "template", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("result",CV_WINDOW_AUTOSIZE);

	cvShowImage( "reference", img );
	cvShowImage( "template", tpl );
	cvShowImage("result",res);
	cvSaveImage("C:\\tv\\images\\phine\\500rs\\template.jpg",tpl,0);
	
	/* wait until user press a key to exit */
	
	//getch();
	cvWaitKey(3000);
	//printf("\nkm=%d",km);
	}
	printf("\n\n\nWaiting for new image to load with lm=%d",lm);

	cvWaitKey(2000);
		}

	/* free memory */
	cvDestroyWindow( "reference" );
	cvDestroyWindow( "template" );
	cvReleaseImage( &img );
	cvReleaseImage( &tpl );
	cvReleaseImage( &res );

	return 0;
}

