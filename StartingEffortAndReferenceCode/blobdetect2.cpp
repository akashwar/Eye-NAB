    #include "cv.h"  
    #include "highgui.h"  
    #include "BlobResult.h"  

    // Get thresholded image in HSV format  
    IplImage* GetThresholdedImageHSV( IplImage* img )  
    {  
        // Create an HSV format image from image passed  
        IplImage* imgHSV = cvCreateImage( cvGetSize( img ),   
                                          8,   
                                          3 );     
      
        cvCvtColor( img, imgHSV, CV_BGR2HSV );  
      
        // Create binary thresholded image acc. to max/min HSV ranges  
        // For detecting blue gloves in "MOV.MPG - HSV mode  
        IplImage* imgThresh = cvCreateImage( cvGetSize( img ),   
                                             8,   
                                             1 );             
      
        cvInRangeS( imgHSV,  
                    cvScalar( 23, 50, 50  ),  
                    cvScalar( 35, 255, 255 ),  
                    imgThresh );  
      
        // Tidy up and return thresholded image  
        cvReleaseImage( &imgHSV );  
        return imgThresh;  
    }  

    int main()  
    {  
        CBlobResult blobs;    
        CBlob *currentBlob;   
        CvPoint pt1, pt2;  
        CvRect cvRect;  
        int key = 0;  
        IplImage* frame = 0;  
      
        // Initialize capturing live feed from video file or camera  
        CvCapture* capture = cvCaptureFromCAM(0);

      
        // Get the frames per second  
        int fps = ( int )cvGetCaptureProperty( capture,  
                                               CV_CAP_PROP_FPS );    
      
        // Can't get device? Complain and quit  
        if( !capture )  
        {  
            printf( "Could not initialize capturing...\n" );  
            return -1;  
        }  
      
        // Windows used to display input video with bounding rectangles  
        // and the thresholded video  
        cvNamedWindow( "video" );  
        cvNamedWindow( "thresh" );        
      
        // An infinite loop  
        while( key != 'x' ) 
        { 
            // If we couldn't grab a frame... quit  
            if( !( frame = cvQueryFrame( capture ) ) )  
                break;        
      
            // Get object's thresholded image (blue = white, rest = black)  
            IplImage* imgThresh = GetThresholdedImageHSV( frame );        
      
            // Detect the white blobs from the black background  
            blobs = CBlobResult( imgThresh, NULL, 0 );    
      
            // Exclude white blobs smaller than the given value (10)    
            // The bigger the last parameter, the bigger the blobs need    
            // to be for inclusion    
            blobs.Filter( blobs,  
                          B_EXCLUDE,  
                          CBlobGetArea(),  
                          B_LESS,  
                          10 );           
      
            // Attach a bounding rectangle for each blob discovered  
            int num_blobs = blobs.GetNumBlobs();  
      
            for ( int i = 0; i < num_blobs; i++ )    
            {                 
                currentBlob = blobs.GetBlob( i );               
                cvRect = currentBlob->GetBoundingBox();  
      
                pt1.x = cvRect.x;  
                pt1.y = cvRect.y;  
                pt2.x = cvRect.x + cvRect.width;  
                pt2.y = cvRect.y + cvRect.height;  
      
                // Attach bounding rect to blob in orginal video input  
                cvRectangle( frame,  
                             pt1,   
                             pt2,  
                             cvScalar(0, 0, 0, 0),  
                             1,  
                             8,  
                             0 );  
            }  
      
            // Add the black and white and original images  
            cvShowImage( "thresh", imgThresh );  
            cvShowImage( "video", frame );  
      
            // Optional - used to slow up the display of frames  
            key = cvWaitKey( 2000 / fps );  
      
            // Prevent memory leaks by releasing thresholded image  
            cvReleaseImage( &imgThresh );        
        }  
      
        // We're through with using camera.   
        cvReleaseCapture( &capture );  
      
        return 0;  
    }  