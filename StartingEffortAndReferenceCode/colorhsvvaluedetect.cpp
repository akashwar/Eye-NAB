#include <cstdio>	// Used for "printf"
#include <string>	// Used for C++ strings
#include <iostream>	// Used for C++ cout print statements
//#include <cmath>	// Used to calculate square-root for statistics

// Include OpenCV libraries
#include <cv.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
using namespace std;
enum                             {cBLACK=0,cWHITE, cGREY, cRED, cORANGE, cYELLOW, cGREEN, cAQUA, cBLUE, cPURPLE, cPINK,  NUM_COLOR_TYPES};
char* sCTypes[NUM_COLOR_TYPES] = {"Black", "White","Grey","Red","Orange","Yellow","Green","Aqua","Blue","Purple","Pink"};
uchar cCTHue[NUM_COLOR_TYPES] =    {0,       0,      0,     0,     20,      30,      55,    85,   115,    138,     161};
uchar cCTSat[NUM_COLOR_TYPES] =    {0,       0,      0,    255,   255,     255,     255,   255,   255,    255,     255};
uchar cCTVal[NUM_COLOR_TYPES] =    {0,      255,    120,   255,   255,     255,     255,   255,   255,    255,     255};
int getPixelColorType(int H, int S, int V)
{
	int color;
	if (V < 75)
		color = cBLACK;
	else if (V > 190 && S < 7)
		color = cWHITE;
	else if (S < 53 && V < 185)
		color = cGREY;
	else {	// Is a color
		if (H < 14)
			color = cRED;
		else if (H < 25)
			color = cORANGE;
		else if (H < 34)
			color = cYELLOW;
		else if (H < 73)
			color = cGREEN;
		else if (H < 102)
			color = cAQUA;
		else if (H < 127)
			color = cBLUE;
		else if (H < 149)
			color = cPURPLE;
		else if (H < 175)
			color = cPINK;
		else	// full circle 
			color = cRED;	// back to Red
	}
	return color;
}

int main()
{
	CvCapture *capture=NULL;
	capture = cvCaptureFromCAM(CV_CAP_ANY);


	IplImage *frame;

	for(int i=0;i<5;i++)
		frame = cvQueryFrame( capture );

		while(1)
	{
		frame = cvQueryFrame( capture );
		cvNamedWindow("captured",1);
		cvShowImage("captured",frame);

		IplImage *frameHSV = cvCreateImage(cvGetSize(frame), 8, 3);
		cvCvtColor(frame,frameHSV, CV_BGR2HSV);	// ( OpenCV stores RGB images in B,G,R order.
		
		cvShowImage("framevideo",frameHSV);
		IplImage* imageDisplayHSV = cvCreateImage(cvGetSize(frame), 8, 3);	// Create an empty HSV image

		int hIn = imageDisplayHSV->height;
		int wIn = imageDisplayHSV->width;
		int rowSizeIn = imageDisplayHSV->widthStep;		// Size of row in bytes, including extra padding
		char *imOfsDisp = imageDisplayHSV->imageData;	// Pointer to the start of the image HSV pixels.
		char *imOfsIn = frameHSV->imageData;	// Pointer to the start of the input image HSV pixels.
		int tallyColors[NUM_COLOR_TYPES];
			for (int i=0; i<NUM_COLOR_TYPES; i++)
				tallyColors[i] = 0;

		for (int y=0; y<hIn; y++) {
			for (int x=0; x<wIn; x++) {
				// Get the HSV pixel components
				uchar H = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 0);	// Hue
				uchar S = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 1);	// Saturation
				uchar V = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 2);	// Value (Brightness)
				// Determine what type of color the HSV pixel is.
				int ctype = getPixelColorType(H, S, V);
				tallyColors[ctype]++;
				//ctype = x / 60;
				// Show the color type on the displayed image, for debugging.
				//cCTHue[ctype]=H;
				//cCTSat[ctype]=S;
				//cCTVal[ctype]=V;
				//printf("%d %d %d\n",H ,S, V);
				*((uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 0)) = H;	// Hue
				*((uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 1)) = 150;	// Full Saturation (except for black & white)
				*((uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 2)) = 150;
			}
		}
		cvNamedWindow("hsvimage",1);
		cvShowImage("hsvimage",imageDisplayHSV);

					int tallyMaxIndex = 0;
			int tallyMaxCount = -1;
			int pixels = wIn * hIn;
			for (int i=0; i<NUM_COLOR_TYPES; i++) {
				int v = tallyColors[i];
				cout << sCTypes[i] << " " << (v*100/pixels) << "%, ";
				if (v > tallyMaxCount) {
					tallyMaxCount = tallyColors[i];
					tallyMaxIndex = i;
				}
			}
			
			int percentage =  (tallyMaxCount * 100 / pixels);
			cout << "Color of shirt: " << sCTypes[tallyMaxIndex] << " (" << percentage << "% confidence)." << endl << endl;
				// Display the HSV debugging image
			IplImage *imageDisplayHSV_RGB = cvCreateImage(cvGetSize(imageDisplayHSV), 8, 3);
			cvCvtColor(imageDisplayHSV, imageDisplayHSV_RGB, CV_HSV2BGR);	// (note that OpenCV stores RGB images in B,G,R order.
			cvNamedWindow("Colors", 1);
			cvShowImage("Colors", imageDisplayHSV_RGB);

			cvWaitKey(1000);
	}

	

	return 0;
}