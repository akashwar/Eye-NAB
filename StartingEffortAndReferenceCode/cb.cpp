#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{
 /// Read image given by user
	
	CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	cvSetCaptureProperty(capture, CV_CAP_PROP_EXPOSURE, -10);
	//assert(capture);
	IplImage *frame;
	int framecount=0;
	cvNamedWindow("framevideo",1);
	for(int i=0;i<5;i++)
	frame = cvQueryFrame( capture );
	while(1)
	{
		frame = cvQueryFrame( capture );
/*	CvScalar s;

 /// Initialize values
 std::cout<<" Basic Linear Transforms "<<std::endl;
 std::cout<<"-------------------------"<<std::endl;
 std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
 std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;

 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
 
 for(int l=0;l<image->height;l++)
			{
		//printf("....");
				for(int m=0;m<image->width;m++)
				{
					 
					
				
						s = cvGet2D(image, l, m);
		
						s.val[0]=alpha*s.val[0]+beta;
						s.val[1]=alpha*s.val[1]+beta;
						s.val[2]=alpha*s.val[2]+beta;
			
						cvSet2D(image, l, m, s );
	
								
			
				}
//		printf("over j");

			}


 */
 /// Create Windows
 cvNamedWindow("Original Image", 1);
 //cvNamedWindow("New Image", 1);

 /// Show stuff
 cvShowImage("Original Image", frame);
 cvWaitKey(10);
 //imshow("New Image", new_image);
	}
 /// Wait until user press some key
 waitKey();
 return 0;
}