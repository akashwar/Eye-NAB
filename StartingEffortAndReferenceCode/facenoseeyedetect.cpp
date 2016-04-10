#include <stdio.h>
#include<conio.h>
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "EyeBrow.h"
#include "MouthPoint.h"
#include "NosePoints.h"
#define EYE_BOTTOM_DISPLACEMENT -10
#define EYE_TOP_DISPLACEMENT 40
#define EYE_LEFT_DISPLACEMENT -15
#define EYE_RIGHT_DISPLACEMENT 25


CvHaarClassifierCascade *cascade,*cascade_e,*cascade_nose,*cascade_mouth;
CvMemStorage            *storage;
char *face_cascade=".\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
char *eye_cascade=".\\data\\haarcascades\\haarcascade_mcs_eyepair_big.xml";
char *nose_cascade=".\\data\\haarcascades\\haarcascade_mcs_nose.xml";
char *mouth_cascade=".\\data\\haarcascades\\haarcascade_mcs_mouth.xml";
void eyeBrow(int x,int y,int width,int height,IplImage *img);
IplImage* cropRectangle(IplImage *img, CvRect region);
void cropEye();
void eyeFeatures(IplImage * eyeImage);
/*Mouth detect ion*/
void detectMouth( IplImage *img,CvRect *r)
{
   CvSeq *mouth;
   //mouth detecetion - set ROI
   cvSetImageROI(img,/* the source image */ 
                 cvRect(r->x,            /* x = start from leftmost */
                        r->y+(r->height *2/3), /* y = a few pixels from the top */
                        r->width,        /* width = same width with the face */
                        r->height/3    /* height = 1/3 of face height */
                       )
                );
    mouth = cvHaarDetectObjects(img,/* the source image, with the estimated location defined */ 
                                cascade_mouth,      /* the eye classifier */ 
                                storage,        /* memory buffer */
                                1.15, 4, 0,     /* tune for your app */ 
                                cvSize(25, 15)  /* minimum detection scale */
                               );

        for( int i = 0; i < (mouth ? mouth->total : 0); i++ )
        {
      
          CvRect *mouth_cord = (CvRect*)cvGetSeqElem(mouth, i);
		  /* draw a red rectangle */
          
		   cvResetImageROI(img);
		  CvRect mouth = cvRect(r->x+mouth_cord->x,r->y+mouth_cord->y+(r->height *2/3),mouth_cord->width,mouth_cord->height);
		  IplImage *mouthcord = cropRectangle(img,mouth);
		  cvSaveImage( "C:\\tv\\facialexpression\\mouth.jpg", mouthcord );
         
        }
     //end mouth detection
          
}

/*Nose detection*/
void detectNose( IplImage *img,CvRect *r)
{
  CvSeq *nose;
  
  //nose detection- set ROI
  cvSetImageROI(img,                    /* the source image */ 
                cvRect(r->x,            /* x = start from leftmost */
                       r->y , /* y = a few pixels from the top */
                       r->width,        /* width = same width with the face */
                       r->height  /* height = 1/3 of face height */
                      )
               );
          

  nose = cvHaarDetectObjects(img, /* the source image, with the estimated location defined */ 
                             cascade_nose,      /* the eye classifier */ 
                             storage,        /* memory buffer */
                             1.15, 3, 0,     /* tune for your app */ 
                             cvSize(25, 15)  /* minimum detection scale */
                            );

  for( int i = 0; i < (nose ? nose->total : 0); i++ )
      {
          CvRect *nose_cord = (CvRect*)cvGetSeqElem(nose, i);

          /* draw a red rectangle */
         // cvRectangle(img, 
           //           cvPoint(nose_cord->x, nose_cord->y), 
             //         cvPoint(nose_cord->x + nose_cord->width, nose_cord->y + nose_cord->height),
               //       CV_RGB(0,255, 0), 
                 //     1, 8, 0
                   // );
		   cvResetImageROI(img);
		  CvRect nose = cvRect(r->x+nose_cord->x,r->y+nose_cord->y,nose_cord->width,nose_cord->height);
		  IplImage *nosecord = cropRectangle(img,nose);
		  cvSaveImage( "C:\\tv\\facialexpression\\nose.jpg", nosecord );
         

      }
}

/*Eyes detection*/
void detectEyes( IplImage *img,CvRect *r)
{
    char *eyecascade;
    CvSeq *eyes;
    int eye_detect=0;
    	
	 printf("\n face_x=%d face_y=%d wd=%d ht=%d",r->x,r->y,r->width,r->height);
   //eye detection starts
  /* Set the Region of Interest: estimate the eyes' position */
    
    cvSetImageROI(img,                    /* the source image */ 
          cvRect
          (
              r->x,            /* x = start from leftmost */
              r->y + (r->height/5.5), /* y = a few pixels from the top */
              r->width,        /* width = same width with the face */
              r->height/3.0    /* height = 1/3 of face height */
          )
      );
	
      /* detect the eyes */
      eyes = cvHaarDetectObjects( img,            /* the source image, with the estimated location defined */ 
                                  cascade_e,      /* the eye classifier */ 
                                  storage,        /* memory buffer */
                                  1.15, 3, 0,     /* tune for your app */ 
                                  cvSize(25, 15)  /* minimum detection scale */
                                );
    
      printf("\no of eyes detected are %d",eyes->total);
    
      
        /* draw a rectangle for each detected eye */
        for( int i = 0; i < (eyes ? eyes->total : 0); i++ )
          {
              eye_detect++;
              /* get one eye */
              CvRect *eye = (CvRect*)cvGetSeqElem(eyes, i);
              /* draw a red rectangle */
                       	cvResetImageROI(img);
						printf(" %d %d %d %d",eye->x, eye->y,eye->height,eye->width);
						eyeBrow(r->x+eye->x + EYE_LEFT_DISPLACEMENT,r->y+(r->height/5.5),eye->width+EYE_RIGHT_DISPLACEMENT,eye->y+eye->height,img );
           }

            
}

void eyeBrow(int x, int y, int width, int height, IplImage* img) {
	
    CvRect rect = cvRect(x,y,width,height);
	IplImage *saveEyeBrow = cropRectangle(img,rect);
    cvSaveImage( "C:\\tv\\facialexpression\\eyeBrow.jpg", saveEyeBrow );
	cvRectangle(img, 
      cvPoint(x, y), 
      cvPoint(x +width,y +height),
      CV_RGB(0, 0, 0), 
      1, 8, 0
      );
	
}

void detectFacialFeatures( IplImage *img,IplImage *temp_img,int img_no)
{
    
    char image[100],msg[100],temp_image[100];
    float m[6];
    double factor = 1;
    CvMat M = cvMat( 2, 3, CV_32F, m );
    int w = (img)->width;
    int h = (img)->height;
    CvSeq* faces;
    CvRect *r;

    m[0] = (float)(factor*cos(0.0));
    m[1] = (float)(factor*sin(0.0));
    m[2] = w*0.5f;
    m[3] = -m[1];
    m[4] = m[0];
    m[5] = h*0.5f;
    
    cvGetQuadrangleSubPix(img, temp_img, &M);
    CvMemStorage* storage=cvCreateMemStorage(0);
    cvClearMemStorage( storage );
    
    if( cascade )
        faces = cvHaarDetectObjects(img,cascade, storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(20, 20));
    else
        printf("\nFrontal face cascade not loaded\n");

    printf("\n no of faces detected are %d",faces->total);
    

    /* for each face found, draw a red box */
    for(int i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ )
    {

        r = ( CvRect* )cvGetSeqElem( faces, i );
        cvRectangle( img,cvPoint( r->x, r->y ),cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );    
    
        printf("\n face_x=%d face_y=%d wd=%d ht=%d",r->x,r->y,r->width,r->height);
        
        detectEyes(img,r);
        /* reset region of interest */
        cvResetImageROI(img);
        detectNose(img,r);
        cvResetImageROI(img);
        detectMouth(img,r);
        cvResetImageROI(img);
    }
	printf("Inside all faces");
    /* reset region of interest */
      cvResetImageROI(img);

      if(faces->total>0)
        {
				cvNamedWindow("FaceImage",1);

            sprintf(image,"C:\\tv\\%d.jpg",img_no);
            //cvSaveImage( image, img );
			cvShowImage("FaceImage",img);
        }
}

int main( int argc, char** argv )
{
    IplImage  *img,*temp_img;
    int       key;

    char image[100],temp_image[100];
    

    /* load the classifier 
       note that I put the file in the same directory with
       this code */
    storage = cvCreateMemStorage( 0 );
    cascade = ( CvHaarClassifierCascade* )cvLoad( face_cascade, 0, 0, 0 );
    cascade_e = ( CvHaarClassifierCascade* )cvLoad( eye_cascade, 0, 0, 0 );
    cascade_nose = ( CvHaarClassifierCascade* )cvLoad( nose_cascade, 0, 0, 0 );
    cascade_mouth = ( CvHaarClassifierCascade* )cvLoad( mouth_cascade, 0, 0, 0 );
	
    
    if( !(cascade || cascade_e ||cascade_nose||cascade_mouth) )
        {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
        }
    int j=0;
	img=cvLoadImage("c:\\tv\\facialexpression\\people.jpg");
    temp_img=img;
        
    if(!img)
    {
    printf("Could not load image file and trying once again: %s\n",image);
    }
    printf("\n curr_image = %s",image);
  
    detectFacialFeatures(img,temp_img,j);
	blackWhiteEyeBrow();
	//plotMouthPoints();
	//plotNosePoints();
	//cropEye();
	
	j++;
	cvWaitKey();
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseHaarClassifierCascade( &cascade_e );   
    cvReleaseHaarClassifierCascade( &cascade_nose );
    cvReleaseHaarClassifierCascade( &cascade_mouth );
    cvReleaseMemStorage( &storage );
    
     cvReleaseImage(&img);
     cvReleaseImage(&temp_img);
     return 0;
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
void cropEye(){
	printf("hi");
	IplImage *eyeImage = cvLoadImage("c:/tv/facialexpression/eyeBrow.jpg");
	cvSetImageROI(eyeImage, cvRect(0, eyeImage->height/2 - eyeImage->height%2+5, eyeImage->width, eyeImage->height));
IplImage *onlyEye = cvCreateImage(cvGetSize(eyeImage),
                           eyeImage->depth,
                           eyeImage->nChannels);
printf("hello");
cvCopy(eyeImage, onlyEye, NULL);
cvResetImageROI(eyeImage);
	cvNamedWindow("cropeye",1);
	cvShowImage("cropeye",onlyEye);
	eyeFeatures(onlyEye);
	cvWaitKey();
}

void eyeFeatures(IplImage * eyeImage){
	IplImage *graymouth = cvCreateImage(cvGetSize(eyeImage),8,1);
	
	cvCvtColor(eyeImage, graymouth, CV_RGB2GRAY);
	cvDilate(graymouth,graymouth,0,1);
	
	cvCanny(graymouth, graymouth, 100,150 , 3);
	cvNamedWindow("eyeImage",1);
	cvShowImage("eyeImage",graymouth);
	

	int stepr, channelsr;
	uchar *datar;
	stepr=graymouth->widthStep;
	channelsr=graymouth->nChannels;
	datar = (uchar *)graymouth->imageData;
	int lefteyelessPoint = 999;
	int row;
	printf("width %d height%d",graymouth->width/2,graymouth->height);
	// left eye left point
	for(int i = 10; i< graymouth->height ; i++) {
		for(int j = 10 ; j<graymouth->width/2; j++) {
			
			if(datar[i*stepr+j*channelsr]== 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(j < lefteyelessPoint){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	CvScalar s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	// left eye right point
	lefteyelessPoint = 0;
		for(int i = 10; i< graymouth->height ; i++) {
		for(int j = 10 ; j<graymouth->width/2; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(j > lefteyelessPoint){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	 s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );

	row =0;
		for(int i = 5; i< graymouth->height ; i++) {
		for(int j = 10 ; j<graymouth->width/2; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(i > row){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	
	row =999;
		for(int i = 0; i< graymouth->height ; i++) {
		for(int j = 10 ; j<graymouth->width/2; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(i < row){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	

/// Now the next functions will be for righjt eye
	lefteyelessPoint = 999;
	for(int i = 10; i< graymouth->height ; i++) {
		for(int j = graymouth->width/2 ; j<graymouth->width; j++) {
			
			if(datar[i*stepr+j*channelsr]== 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(j < lefteyelessPoint){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	 s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	// left eye right point
	lefteyelessPoint = 0;
		for(int i = 10; i< graymouth->height ; i++) {
		for(int j = graymouth->width/2 ; j<graymouth->width; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(j > lefteyelessPoint){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	 s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );

	row =0;
		for(int i = 5; i< graymouth->height ; i++) {
		for(int j = graymouth->width/2 ; j<graymouth->width; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(i > row){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	
	row =999;
		for(int i = 0; i< graymouth->height ; i++) {
		for(int j = graymouth->width/2 ; j<graymouth->width; j++) {
			
			if(datar[i*stepr+j*channelsr] == 255) {
				printf(" color i %d %d  %d",i,j,datar[i*stepr+j*channelsr]);
				if(i < row){
					printf("j  %d ",j);
				lefteyelessPoint = j;
				printf("less eye point %d\n",lefteyelessPoint);
				row = i;
				}
			}
		}
	}
	printf(" feature point %d %d",row,lefteyelessPoint);
	s = cvGet2D(eyeImage,row, lefteyelessPoint);
	s.val[0]=0.0;
	s.val[1]=255.0;
	s.val[2]=0.0;
	cvSet2D(eyeImage, row,lefteyelessPoint, s );
	






	cvNamedWindow("eyePointsFeature",1);
	cvShowImage("eyePointsFeature",eyeImage);
}