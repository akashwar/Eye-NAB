#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <conio.h>
//#include<stdafx.h>
int findWhitePatch(int,int,IplImage *);
IplImage* color_dst;
IplImage* NormalizeImage(IplImage* theimg);

int main()
{

	//--------------------------Declarative Part -----------------//
	int stepr, channelsr;
	int xmid,ymid;

	uchar *datar;

	float sorty[500];
	IplImage* frame;
	CvCapture *cap=NULL;
	//-----------------------CAPTURE THE FRAME FROM CAMERA----------------------------------
	cap=cvCaptureFromCAM(CV_CAP_ANY);
	for(int i=0;i<8;i++){
		frame = cvQueryFrame( cap);
		cvWaitKey(200);
	}

	printf("start");
	//-------------------------START FOR A LOOP UNTIL PROPER BARCODE SCANNED-------------------------
	while(1)
	{
		frame = cvQueryFrame( cap );
	
		//argv[1]="c:\\projbe\\images\\100rs.jpg";
		if(frame!= 0)
		{
			//printf(" %d %d ",frame->width,frame->height);

			//	-------------MAKING A COPY FROM GRABBED FRAME TO SRC1-----------------------------------
			IplImage *src1=NULL;
			src1=cvCreateImage(cvGetSize(frame), 8, 3 );
			cvCopyImage(frame,src1);


			//-----------SENDING COLOR SRC1 IMAGE FOR NORMALIZATION  --------------------------------
	
			src1=NormalizeImage(src1);
			
			//--------------CREATING A GRAY SCALE IMAGED NAMED WHITEBLACK-----------------------------
			IplImage *whiteblack=NULL;
			whiteblack=cvCreateImage(cvGetSize(src1), 8, 1 );
			

			cvNormalize(whiteblack, whiteblack, 0, 255, CV_MINMAX);

			//---------------Temporary gray copy of grayimage for displaying sample values---------------

			IplImage *Tempgray = cvCreateImage(cvGetSize(whiteblack), 8, 1 );
			cvCopyImage(whiteblack,Tempgray);
		
			//--------------ASSIGNING VARIOUS ATRIBUTES OF WHITEBLACK FOR ACCESING GRAYVALUES  --------------------------------------------------


			stepr=whiteblack->widthStep;
			channelsr=whiteblack->nChannels;
			datar = (uchar *)whiteblack->imageData;

			//-------------CONVERT WHITEBLACK IMAGE IN STEP OF 2 IE WHITE OR BLACK BINARY IMAGE---------------------
			for(int i=0;i < (whiteblack->height);i++){
				for(int j=0;j <(whiteblack->width);j++)
				{
					if(datar[i*stepr+j*channelsr]>128)
					datar[i*stepr+j*channelsr]=255;
					else
					datar[i*stepr+j*channelsr]=0;
			
				}
			}
			//--------------DISPLAYING THE WHITEBLACK IMAGE------------------------
			cvNamedWindow("whiteblack",1);
			cvShowImage("whiteblack",whiteblack);
	
	
			//-------------CREATING A SRC IMAGE FROM SRC AND CONVERT SRC TO GRAYSCALE FOR CANNY EDGE FUNCTION------------
			IplImage* src = cvCreateImage(
			cvGetSize(src1), 8, 1 );
			cvCvtColor(src1, src, CV_RGB2GRAY);
 
			//-------------CREATING A IMAGE NAMED dst FOR EDGE SHOWING ON IT FROM CANNY RESULT--------------------
			IplImage* dst = cvCreateImage(
			cvGetSize(src), 8, 1 );

			//---------------CREATING color_dst IMAGE FOR RESULT OF LINE DISPLAY PURPOSE---------------------------
			color_dst = cvCreateImage(
			cvGetSize(src), 8, 3 );

			//---------------CEATING A STORE POOL FOR LINE PURPOSE AND LINE DECLARATIVE PARAMETER-------------
			CvMemStorage* storage =
			cvCreateMemStorage(0);
			CvSeq* lines = 0;
			int i;

			//---------------APPLYING THE CANNY FUNCTION ON SRC AND STORING THE EDGE RESULTS IN DST-------------
			cvCanny( src, dst,30, 90, 3 );

			// dilate canny output to remove potential
            // holes between edge segments
            cvDilate( dst, dst, 0, 1 );

			//--------------CONVERTING THE CANNY RESULT IMAGE DST INTO RGB AND STORE IT IN COLORDST AND SHOWING A WINDOW OF IT--------------
			cvCvtColor( dst, color_dst, CV_GRAY2BGR );
			cvNamedWindow("colordst",1);
			cvShowImage("colordst",color_dst);

			//--------------NOW APPLYING THE HOUGH TRANSFORM-------------------------------------
			#if 0
			lines = cvHoughLines2( dst, storage,
			CV_HOUGH_STANDARD, 1, CV_PI, 30, 0, 0 );
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

			//------------------APPYING HOUGH PROBABLISTIC METHOD ON THE SINGLE CHANNEL RESULT OF CANNY dst IMAGE GETTING RESULT IN lines----------------
			lines = cvHoughLines2( dst, storage,
					  CV_HOUGH_PROBABILISTIC, 1, CV_PI, 30, 50, 0 );

			//---------------ACCESING THE POINTS OF THE LINES AND DRAWING THOSE LINES ON THE GRAY TO RGB CONVERTED IMAGE NAMED color_dst-------------- 
			for( i = 0; i < lines->total; i++ )
			{
			CvPoint* line =
			(CvPoint*)cvGetSeqElem(lines,i);
			cvLine( color_dst, line[0], line[1],

				   CV_RGB(255,0,0), 3, 8 );
			//printf("\n i = %d x1 = %d y1 = %d x2 = %d y2 = %d ",i,line[0].x,line[0].y,line[1].x,line[1].y);


			}

			//-------------------SORTING THE X CO-ORDINATES OF THE LINES DRAWN ABOVE ----------------

			for(int i1=0; i1<lines->total;i1++)
			{
				for(int j1=i1+1;j1<lines->total;j1++)
				{
					CvPoint* line1 =
					(CvPoint*)cvGetSeqElem(lines,i1);
					CvPoint* line2 =
					(CvPoint*)cvGetSeqElem(lines,j1);
					if(line1[0].x > line2[0].x)
					{
						int temp;
						temp =line1[0].x;
						line1[0].x=line2[0].x;
						line2[0].x=temp;
						
						
						temp =line1[0].y;
						line1[0].y=line2[0].y;
						line2[0].y=temp;
							
						
						temp =line1[1].y;
						line1[1].y=line2[1].y;
						line2[1].y=temp;
					
						temp =line1[1].x;
						line1[1].x=line2[1].x;
						line2[1].x=temp;
						

					}
							
				}


			}





			//---------------	FINDING THE Y CORDINATES MEANS OF CENTER FOR Y AXIS OF LINE ARE  2 AND 6 RESPECTIVELY SO CENTER IS 4 AND FINDING ALL THE CENTER AND TAKING MEAN;
			float avg=0; 
			float sum=0;
			printf("sorted %d",lines->total);
			
			//-------------- DO FOLLOWING OPERATION ONLY IF COUNT OF LINES ON IMAGES IS > 40----------------------
			
			if(lines->total>40)
			{
				for( i = 0; i < lines->total; i++ )
				{
					CvPoint* line =
					(CvPoint*)cvGetSeqElem(lines,i);
					cvLine( color_dst, line[0], line[1],
					CV_RGB(255,0,0), 3, 8 );
					float tempy=(line[0].y+line[1].y)/2;
					sum=sum+(tempy);
					sorty[i]=tempy;  //for sorting y axis centers 
					//printf("\n i = %d x1 = %d y1 = %d x2 = %d y2 = %d ",i,line[0].x,line[0].y,line[1].x,line[1].y);

				}

				//----------SORTING Y AXIS CENTERS------------------
				
				for(int i1=0;i1<lines->total;i1++)
				{
					for(int j1=i1+1;j1<lines->total;j1++)
					{

						if(sorty[i1]>sorty[j1])
						{
							float tempy=sorty[i1];
							sorty[i1]=sorty[j1];
							sorty[j1]=tempy;

						}

					}

				}

				//------SCANNING OF Y AXIS FOR GREATER NO OF RED PIXEL ON COLOR_DST-------------
				int maxval=0;
				for(int i=0;i<color_dst->height;i++)
				{
					int cntmax=0;
					for(int j=0;j<color_dst->width;j++)
					{
						CvScalar RGBval = cvGet2D( color_dst,i, j);
						double r,g,b;

						r = RGBval.val[2];
						g = RGBval.val[1];
						b = RGBval.val[0];
						if(r==255 && g==0 && b==0)
						{
								cntmax++;
						}
						else
						{
					

						}
					}
					if(maxval<cntmax)
					{
						maxval=cntmax;
						ymid=i;
					}

				}

				//printf("ymid= %d",ymid);


				CvScalar s ;
				//-----------FOR PURPOSE OF GETTING THE X MEDIAN------------
				CvPoint* linesmed =
				(CvPoint*)cvGetSeqElem(lines,lines->total/2);

				//--------------------------------Assigning of xmid and ymid-------------------------------------


				ymid=sorty[lines->total/2]; //assingning the sorted medain of y to ymid;
				xmid=linesmed[0].x;// assingning the median value of x axis point to xmid

				//-------------------------------PLOTTING A GREEN CENTER POINT OF MEDIAN OF X AND THAT FOUND OF Y----------------------------------------
				s = cvGet2D(color_dst, linesmed[0].x, sorty[lines->total/2]);
				s.val[0]=0.0;
				s.val[1]=255.0;
				s.val[2]=0.0;
				
				cvSet2D(color_dst,  sorty[lines->total/2],linesmed[0].x, s );
				cvSet2D(color_dst, sorty[lines->total/2], linesmed[0].x+1, s );
				cvSet2D(color_dst, sorty[lines->total/2], linesmed[0].x+2, s );
				cvSet2D(color_dst, sorty[lines->total/2], linesmed[0].x-1, s );
				cvSet2D(color_dst, sorty[lines->total/2], linesmed[0].x-2, s );

				//-------------FINDING A WHITE PATCH ON COLOR_DST WHERE WE BROWSE ON LEFT TO FIND A WHITE BLOCK ON BINARY IMAGE WHITEBLACK--------
				findWhitePatch(linesmed[0].x, sorty[lines->total/2],whiteblack);
			}
			//-------	END IF(>40 )---------------

			//---------------------------------SAMPLE GRAY VALUES AROUND THE CENTER FOR DEBUGGING PURPOSE----------------------------------------------
			// dipslay of  tempgray
			//int  *data;
		
			int step=src1->widthStep;
			int channels=src1->nChannels;
			uchar *data;
			printf("%d  %d",step,channels);
			cvNamedWindow("src1tempwindow",1);
			cvShowImage("src1tempwindow",src1);
			cvWaitKey(3000);
			for(int i1=ymid-5;i1<ymid+5;i1++)
			{
				for(int j1=xmid-20;j1<xmid+20;j1++)
				{
						
					//	data = cvPtr2D(src1, i1, j1, NULL);

					// *******************PART OF CODE NOT WORKING ******************* 
					
				//	int b = ((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3];
			//		int g = ((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3+1];
			//		int r = ((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3+2];
					//**********************************************************

			//		printf("(%3d, %3d, %3d), ", r,g,b);
					//printf("comes here");
			//		((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3]=120;
			//		((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3+1]=140;
			//		((uchar*)(src1->imageData + src1->widthStep*i1))[j1*3+2]=80;

					
				}
				printf("\n\n");

			}

			//------DISPLAYING THE PATCH AROUND THE CENTER ON SRC1 IMAGE------------
			cvNamedWindow("src1patched",1);
			cvShowImage("src1patched",src1);


			//-------------------------------------------------------------------------------

			#endif
			cvNamedWindow( "Source", 1 );
			cvShowImage( "Source", src );
			cvNamedWindow( "Hough", 1 );
			cvShowImage( "Hough", color_dst );

			cvWaitKey(2000);
/*if(lines->total>40)
{
	getch();
	getch();
	break;
	
}*/
		}
	}

}

// --------FINDING THE WHITE PATCH ON BINARY IMAGE AND SHOWING LINES DRAWN IMAGE COLOR_DST--------------

int findWhitePatch(int xmid,int ymid,IplImage *whiteblack)  //this function will traverse frm xmid to left to find whiite patch
{
	int i=xmid;
	int j=ymid;
	int stepr, channelsr;
	uchar *datar;
	int flag=0;
	while(1)
	{
		
		int flg=0;
		stepr=whiteblack->widthStep;
		channelsr=whiteblack->nChannels;
		datar = (uchar *)whiteblack->imageData;
		
		for(i=xmid;i>xmid-20;i--)
		{
			for(j=ymid-30;j<ymid;j++)
			{
				
				if(datar[i*stepr+j*channelsr]==255)
				{;
				}
				else
				{
					flg=1;
					break;
				}

			}
			if(flg==1)
				break;


		}
		if(flg!=1)
		{
			flag=1;
			
			break;
		}

		xmid--;
		if(xmid==19)
			break;

	}
	


	if(flag==1)
	{
		for(i=xmid;i>xmid-20;i--)
			{
				for(j=ymid-30;j<ymid;j++)
				{
					CvScalar s = cvGet2D(color_dst,j,i);
					s.val[0]=0.0;
					s.val[1]=255.0;
					s.val[2]=0.0;
				
					cvSet2D(color_dst, j, i, s );

					
					

				}
			}

					cvNamedWindow("whitepatch",1);
					cvShowImage("whitepatch",color_dst);
					//cvWaitKey(1000);
			

		printf("detected");	
		return 1;
	}
	else
	{
		printf("badluck");
	return 0;

	}



}

//--------------FUNCTION TO NORMALIZE RGB IMAGE------------------
IplImage* NormalizeImage(IplImage* theimg)
{
	IplImage* redchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* greenchannel = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* bluechannel = cvCreateImage(cvGetSize(theimg), 8, 1);
	IplImage* redavg = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* greenavg = cvCreateImage(cvGetSize(theimg), 8, 1);
    IplImage* blueavg= cvCreateImage(cvGetSize(theimg), 8, 1);
 
    IplImage* imgavg = cvCreateImage(cvGetSize(theimg), 8, 3);
	cvSplit(theimg, bluechannel, greenchannel, redchannel, NULL);
	cvNormalize(redchannel, redavg, 0, 255, CV_MINMAX);
	cvNormalize(greenchannel, greenavg, 0, 255, CV_MINMAX);
	cvNormalize(bluechannel, blueavg, 0, 255, CV_MINMAX);
            
	
	cvMerge(blueavg, greenavg, redavg, NULL, imgavg);
	cvNamedWindow("redwali",1);
	cvShowImage("redwali",redavg);
	cvWaitKey(1000);
		
    return imgavg;



}
