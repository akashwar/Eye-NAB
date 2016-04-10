#include "cv.h"
#include "highgui.h"
 
int main()
{
    IplImage* img  = cvLoadImage("C:\\tv\\images\\mixed\\samples\\1000final2.jpg");
 
 /* Always check if the program can find a file */
 if( !img )
   return -1;
 
 IplImage* channel = cvCreateImage( cvGetSize(img), 8, 1 );
        IplImage *hist_img = cvCreateImage(cvSize(300,240), 8, 3);
 cvSet( hist_img, cvScalarAll(255), 0 );
 
 CvHistogram *hist_red;
 CvHistogram *hist_green;
 CvHistogram *hist_blue;
 
    int hist_size = 256;      
    float range[]={0,256};
    float* ranges[] = { range };
     
    float max_value = 0.0;
    float max = 0.0;
    float w_scale = 0.0;
 
 /* Create a 1-D Arrays to hold the histograms */
 hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 
 /* Set image to obtain RED as Channel of Interest (COI) */
  cvSetImageCOI(img,3);
  cvCopy(img,channel);
  cvResetImageROI(img);
 
 /* Calculate histogram of the Image and store it in the array */
  cvCalcHist( &channel, hist_red, 0, NULL );
 
 /* Calculate and Plot the histograms Green and Blue channels as well */
 /* Green channel */
  cvSetImageCOI(img,2);
  cvCopy(img,channel);
  cvResetImageROI(img);
 
  cvCalcHist( &channel, hist_green, 0, NULL );
  
 /* Blue channel */
  cvSetImageCOI(img,1);
  cvCopy(img,channel);
  cvResetImageROI(img);
 
  cvCalcHist( &channel, hist_blue, 0, NULL );
 
 
        /* Find the minimum and maximum values of the histograms */
        cvGetMinMaxHistValue( hist_red, 0, &max_value, 0, 0 );
 cvGetMinMaxHistValue( hist_green, 0, &max, 0, 0 );
 
 max_value = (max > max_value) ? max : max_value;
 
 cvGetMinMaxHistValue( hist_blue, 0, &max, 0, 0 );
 
 max_value = (max > max_value) ? max : max_value;    
                        // The variable max_value has the maximum of the three histograms
 
 /* Using the maximum value, Scale/Squeeze the histogram (to fit the image) */
        cvScale( hist_red->bins, hist_red->bins, ((float)hist_img->height)/max_value, 0 );
 cvScale( hist_green->bins, hist_green->bins, ((float)hist_img->height)/max_value, 0 );
 cvScale( hist_blue->bins, hist_blue->bins, ((float)hist_img->height)/max_value, 0 );
 
 printf("Scale: %4.2f pixels per 100 units\n", max_value*100/((float)hist_img->height));                         
                                                   //A scale to estimate the number of pixels
 
    /* Scale/Squeeze the histogram range to image width */
    w_scale = ((float)hist_img->width)/hist_size;
 
 /* Plot the Histograms */
    for( int i = 0; i < hist_size; i++ )
    {
      cvRectangle( hist_img, cvPoint((int)i*w_scale , hist_img->height),
        cvPoint((int)(i+1)*w_scale, hist_img->height - cvRound(cvGetReal1D(hist_red->bins,i))),
        CV_RGB(255,0,0), -1, 8, 0 );
      cvRectangle( hist_img, cvPoint((int)i*w_scale , hist_img->height),
       cvPoint((int)(i+1)*w_scale, hist_img->height - cvRound(cvGetReal1D(hist_green->bins,i))),
       CV_RGB(0,255,0), -1, 8, 0 );
      cvRectangle( hist_img, cvPoint((int)i*w_scale , hist_img->height),
       cvPoint((int)(i+1)*w_scale, hist_img->height - cvRound(cvGetReal1D(hist_blue->bins,i))),
       CV_RGB(0,0,255), -1, 8, 0 );
    }
 
    cvNamedWindow( "Image", 1 );
    cvShowImage( "Image",img);
 
    /* create a window to show the histogram of the image */
    cvNamedWindow("Histogram", 1);
    cvShowImage( "Histogram", hist_img);
 
    cvWaitKey(0);
 
 cvDestroyWindow( "Image" );
 cvDestroyWindow( "Histogram" );
    cvReleaseImage( &img );
    return 0;
}