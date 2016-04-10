/*ALL the necessary header files*/
/*If you want to enhance the Videos then instead of loading frames from an
Image you may load frames from a  camera.. by using the capture structures*/
 #include"math.h"
 #include"conio.h"
 #include"cv.h"
 #include"highgui.h"
 #include"stdio.h"
int main()
{

int i,j,k;
int heightc,widthc,stepc,channelsc;


IplImage *frame=cvLoadImage("c:\\tv\\images\\result7.jpg",1);
cvNamedWindow("frame");
cvShowImage("frame", frame);
//cvSaveImage("c:\\tv\\images\\result7.jpg",convert);

 cvWaitKey(0);
 cvDestroyWindow("frame");

 
 return 0;
 }