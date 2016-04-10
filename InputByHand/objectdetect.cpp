
#include <stdio.h>

	#include <conio.h>		// For _kbhit() on Windows
	#include <direct.h>		// For mkdir(path) on Windows
	#define snprintf sprintf_s	// Visual Studio on Windows comes with sprintf_s() instead of snprintf()

#include <vector>
#include <string>
//#include <string.h>
#include "cv.h"
#include "cvaux.h"
#include "highgui.h" 

#ifndef BOOL
	#define BOOL bool
#endif

using namespace std;

// Haar Cascade file, used for Face Detection.
    // const char *objCascadeFilename = "C:\\Users\\nikhil\\Documents\\Visual Studio 2008\\Projects\\HelloWorld\\HelloWorld\\powderakshayhome\\cascade2xml\\output.xml";;
//const char *objCascadeFilename = ".\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
const char objCascadeFilename[5][200]={"C:\\Users\\nikhil\\Documents\\Visual Studio 2008\\Projects\\HelloWorld\\HelloWorld\\7up\\cascade2xml\\output.xml","C:\\Users\\nikhil\\Documents\\Visual Studio 2008\\Projects\\HelloWorld\\HelloWorld\\tools\\cascade2xml\\output.xml","C:\\Users\\nikhil\\Documents\\Visual Studio 2008\\Projects\\HelloWorld\\HelloWorld\\powderakshayhome\\cascade2xml\\output.xml"};

int obj_EIGENFACE_IMAGES = 1;		// Set to 0 if you dont want images of the Eigenvectors saved to files (for debugging).
//#define USE_MAHALANOBIS_DISTANCE	// You might get better recognition accuracy if you enable this.


// Global variables
int fileflag=0;
IplImage ** objectImgArr        = 0; // array of face images
CvMat    *  personobjNumTruthMat = 0; // array of person numbers
//#define	MAX_NAME_LENGTH 256		// Give each name a fixed size for easier code.
//char **objNames = 0;			// array of person names (indexed by the person number). Added by Shervin.
vector<string> objNames;			// array of person names (indexed by the person number). Added by Shervin.
int objectWidth = 120;	// Default dimensions for faces in the face recognition database. Added by Shervin.
int objectHeight = 90;	//	"		"		"		"		"		"		"		"
int nobjPersons                  = 0; // the number of people in the training set. Added by Shervin.
int nobjTrainFaces               = 0; // the number of training images
int nobjEigens                   = 0; // the number of eigenvalues
IplImage * pobjAvgTrainImg       = 0; // the average image
IplImage ** eigenobjVectArr      = 0; // eigenvectors
CvMat * eigenobjValMat           = 0; // eigenvalues
CvMat * projectedobjTrainFaceMat = 0; // projected training faces

CvCapture* camobjera = 0;	// The camobjera device.


// Function prototypes
void printobjUsage();
void leaobjrn(const char *szFileTrain);
void doobjPCA();
void storeobjTrainingData();
int  loadobjTrainingData(CvMat ** pTrainPersonNumMat);
int  findobjNearestNeighbor(float * projectedTestFace);
int findobjNearestNeighbor(float * projectedTestFace, float *pConfidence);
int  loadobjobjectImgArray(const char * filename);
void recognizeobjFileList(const char *szFileTest);
void recognizeobjFromCam(void);
IplImage* getcamobjeraFrame(void);
IplImage* convertobjImageToGreyscale(const IplImage *imageSrc);
IplImage* cropobjImage(const IplImage *img, const CvRect region);
IplImage* resizeobjImage(const IplImage *origImg, int newWidth, int newHeight);
IplImage* convertobjFloatImageToUcharImage(const IplImage *srcImg);
void saveobjFloatImage(const char *filename, const IplImage *srcImg);
CvRect detectobjFaceInImage(const IplImage *inputImg, const CvHaarClassifierCascade* cascade );
CvMat* retrainobjOnline(void);

// Show how to use this program from the command-line.
void printobjUsage()
{
	printf(
		"Usage: OnlineFaceRec [<command>] \n"
		"  Valid commands are: \n"
		"    train <train_file> \n"
		"    test <test_file> \n"
		" (if no args are supplied, then online camobjera mode is enabled).\n"
		);
}


// Startup routine.
int objectdetect( )
{
	printobjUsage();

		recognizeobjFromCam();
		return 0;
}

#if defined WIN32 || defined _WIN32
	// Wrappers of kbhit() and getch() for Windows:
	#define changeKeyboardMode
	#define kbhit _kbhit
#else
	// Create an equivalent to kbhit() and getch() for Linux,
	// based on "http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html":
	
	#define VK_ESCAPE 0x1B		// Escape character

	// If 'dir' is 1, get the Linux terminal to return the 1st keypress instead of waiting for an ENTER key.
	// If 'dir' is 0, will reset the terminal back to the original settings.
	void changeKeyboardMode(int dir)
	{
		static struct termios oldt, newt;

		if ( dir == 1 ) {
			tcgetattr( STDIN_FILENO, &oldt);
			newt = oldt;
			newt.c_lflag &= ~( ICANON | ECHO );
			tcsetattr( STDIN_FILENO, TCSANOW, &newt);
		}
		else
			tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	}

	// Get the next keypress.
	int kbhit(void)
	{
		struct timeval tv;
		fd_set rdfs;

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		FD_ZERO(&rdfs);
		FD_SET (STDIN_FILENO, &rdfs);

		select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
		return FD_ISSET(STDIN_FILENO, &rdfs);
	}

	// Use getchar() on Linux instead of getch().
	#define getch() getchar()
#endif


// Save all the eigenvectors as images, so that they can be checked.
void storeobjEigenfaceImages()
{
	// Store the average image to a file
	printf("Saving the image of the average face as 'out_averageImage.bmp'.\n");
	cvSaveImage("out_averageImage.bmp", pobjAvgTrainImg);
	// Create a large image made of many eigenface images.
	// Must also convert each eigenface image to a normal 8-bit UCHAR image instead of a 32-bit float image.
	printf("Saving the %d eigenvector images as 'out_eigenfaces.bmp'\n", nobjEigens);
	if (nobjEigens > 0) {
		// Put all the eigenfaces next to each other.
		int COLUMNS = 8;	// Put upto 8 images on a row.
		int nCols = min(nobjEigens, COLUMNS);
		int nRows = 1 + (nobjEigens / COLUMNS);	// Put the rest on new rows.
		int w = eigenobjVectArr[0]->width;
		int h = eigenobjVectArr[0]->height;
		CvSize size;
		size = cvSize(nCols * w, nRows * h);
		IplImage *bigImg = cvCreateImage(size, IPL_DEPTH_8U, 1);	// 8-bit Greyscale UCHAR image
		for (int i=0; i<nobjEigens; i++) {
			// Get the eigenface image.
			IplImage *byteImg = convertobjFloatImageToUcharImage(eigenobjVectArr[i]);
			// Paste it into the correct position.
			int x = w * (i % COLUMNS);
			int y = h * (i / COLUMNS);
			CvRect ROI = cvRect(x, y, w, h);
			cvSetImageROI(bigImg, ROI);
			cvCopyImage(byteImg, bigImg);
			cvResetImageROI(bigImg);
			cvReleaseImage(&byteImg);
		}
		cvSaveImage("out_eigenfaces.bmp", bigImg);
		cvReleaseImage(&bigImg);
	}
}

// Train from the data in the given text file, and store the trained data into the file 'objectdata.xml'.
void leaobjrn(const char *szFileTrain)
{
	int i, offset;

	// load training data
	printf("Loading the training images in '%s'\n", szFileTrain);
	nobjTrainFaces = loadobjobjectImgArray(szFileTrain);
	printf("Got %d training images.\n", nobjTrainFaces );
	if( nobjTrainFaces < 2 )
	{
		fprintf(stderr,
		        "Need 2 or more training faces\n"
		        "Input file contains only %d\n", nobjTrainFaces );
		return;
	}

	// do PCA on the training faces
	doobjPCA();

	// project the training images onto the PCA subspace
	projectedobjTrainFaceMat = cvCreateMat( nobjTrainFaces , nobjEigens, CV_32FC1 );
	offset = projectedobjTrainFaceMat->step / sizeof(float);
	for(i=0; i<nobjTrainFaces ; i++)
	{
		//int offset = i * nobjEigens;
		cvEigenDecomposite(
			objectImgArr[i],
			nobjEigens,
			eigenobjVectArr,
			0, 0,
			pobjAvgTrainImg,
			//projectedobjTrainFaceMat->data.fl + i*nobjEigens);
			projectedobjTrainFaceMat->data.fl + i*offset);
	}

	// store the recognition data as an xml file
	storeobjTrainingData();

	// Save all the eigenvectors as images, so that they can be checked.
	if (obj_EIGENFACE_IMAGES) {
		storeobjEigenfaceImages();
	}

}


// Open the training data from the file 'objectdata.xml'.
int loadobjTrainingData(CvMat ** pTrainPersonNumMat)
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( "objectdata.xml", 0, CV_STORAGE_READ );
	if( !fileStorage ) {
		printf("Can't open training database file 'objectdata.xml'.\n");
		return 0;
	}

	// Load the person names. Added by Shervin.
	objNames.clear();	// Make sure it starts as empty.
	nobjPersons = cvReadIntByName( fileStorage, 0, "nobjPersons", 0 );
	if (nobjPersons == 0) {
		printf("No people found in the training database 'objectdata.xml'.\n");
		return 0;
	}
	// Load each person's name.
	for (i=0; i<nobjPersons; i++) {
		string sPersonName;
		char varname[200];
		snprintf( varname, sizeof(varname)-1, "personName_%d", (i+1) );
		sPersonName = cvReadStringByName(fileStorage, 0, varname );
		objNames.push_back( sPersonName );
	}

	// Load the data
	nobjEigens = cvReadIntByName(fileStorage, 0, "nobjEigens", 0);
	nobjTrainFaces = cvReadIntByName(fileStorage, 0, "nobjTrainFaces ", 0);
	*pTrainPersonNumMat = (CvMat *)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
	eigenobjValMat  = (CvMat *)cvReadByName(fileStorage, 0, "eigenobjValMat", 0);
	projectedobjTrainFaceMat = (CvMat *)cvReadByName(fileStorage, 0, "projectedobjTrainFaceMat", 0);
	pobjAvgTrainImg = (IplImage *)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
	eigenobjVectArr = (IplImage **)cvAlloc(nobjTrainFaces *sizeof(IplImage *));
	for(i=0; i<nobjEigens; i++)
	{
		char varname[200];
		snprintf( varname, sizeof(varname)-1, "eigenVect_%d", i );
		eigenobjVectArr[i] = (IplImage *)cvReadByName(fileStorage, 0, varname, 0);
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );

	printf("Training data loaded (%d training images of %d people):\n", nobjTrainFaces , nobjPersons);
	printf("People: ");
	if (nobjPersons > 0)
		printf("<%s>", objNames[0].c_str());
	for (i=1; i<nobjPersons; i++) {
		printf(", <%s>", objNames[i].c_str());
	}
	printf(".\n");

	return 1;
}


// Save the training data to the file 'objectdata.xml'.
void storeobjTrainingData()
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( "objectdata.xml", 0, CV_STORAGE_WRITE );

	// Store the person names. Added by Shervin.
	cvWriteInt( fileStorage, "nobjPersons", nobjPersons );
	for (i=0; i<nobjPersons; i++) {
		char varname[200];
		snprintf( varname, sizeof(varname)-1, "personName_%d", (i+1) );
		cvWriteString(fileStorage, varname, objNames[i].c_str(), 0);
	}

	// store all the data
	cvWriteInt( fileStorage, "nobjEigens", nobjEigens );
	cvWriteInt( fileStorage, "nobjTrainFaces ", nobjTrainFaces );
	cvWrite(fileStorage, "trainPersonNumMat", personobjNumTruthMat, cvAttrList(0,0));
	cvWrite(fileStorage, "eigenobjValMat", eigenobjValMat, cvAttrList(0,0));
	cvWrite(fileStorage, "projectedobjTrainFaceMat", projectedobjTrainFaceMat, cvAttrList(0,0));
	cvWrite(fileStorage, "avgTrainImg", pobjAvgTrainImg, cvAttrList(0,0));
	for(i=0; i<nobjEigens; i++)
	{
		char varname[200];
		snprintf( varname, sizeof(varname)-1, "eigenVect_%d", i );
		cvWrite(fileStorage, varname, eigenobjVectArr[i], cvAttrList(0,0));
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
}

// Find the most likely person based on a detection. Returns the index, and stores the confidence value into pConfidence.
int findobjNearestNeighbor(float * projectedTestFace, float *pConfidence)
{
	//double leastDistSq = 1e12;
	double leastDistSq = DBL_MAX;
	int i, iTrain, iNearest = 0;

	for(iTrain=0; iTrain<nobjTrainFaces ; iTrain++)
	{
		double distSq=0;

		for(i=0; i<nobjEigens; i++)
		{
			float d_i = projectedTestFace[i] - projectedobjTrainFaceMat->data.fl[iTrain*nobjEigens + i];
#ifdef USE_MAHALANOBIS_DISTANCE
			distSq += d_i*d_i / eigenobjValMat->data.fl[i];  // Mahalanobis distance (might give better results than Eucalidean distance)
#else
			distSq += d_i*d_i; // Euclidean distance.
#endif
		}

		if(distSq < leastDistSq)
		{
			leastDistSq = distSq;
			iNearest = iTrain;
		}
	}

	// Return the confidence level based on the Euclidean distance,
	// so that similar images should give a confidence between 0.5 to 1.0,
	// and very different images should give a confidence between 0.0 to 0.5.
	*pConfidence = 1.0f - sqrt( leastDistSq / (float)(nobjTrainFaces * nobjEigens) ) / 255.0f;

	// Return the found index.
	return iNearest;
}

// Do the Principal Component Analysis, finding the average image
// and the eigenfaces that represent any image in the given dataset.
void doobjPCA()
{
	int i;
	CvTermCriteria calcLimit;
	CvSize faceImgSize;

	// set the number of eigenvalues to use
	nobjEigens = nobjTrainFaces -1;

	// allocate the eigenvector images
	faceImgSize.width  = objectImgArr[0]->width;
	faceImgSize.height = objectImgArr[0]->height;
	eigenobjVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * nobjEigens);
	for(i=0; i<nobjEigens; i++)
		eigenobjVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

	// allocate the eigenvalue array
	eigenobjValMat = cvCreateMat( 1, nobjEigens, CV_32FC1 );

	// allocate the averaged image
	pobjAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

	// set the PCA termination criterion
	calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, nobjEigens, 1);

	// compute average image, eigenvalues, and eigenvectors
	cvCalcEigenObjects(
		nobjTrainFaces ,
		(void*)objectImgArr,
		(void*)eigenobjVectArr,
		CV_EIGOBJ_NO_CALLBACK,
		0,
		0,
		&calcLimit,
		pobjAvgTrainImg,
		eigenobjValMat->data.fl);

	cvNormalize(eigenobjValMat, eigenobjValMat, 1, 0, CV_L1, 0);
}

// Read the names & image filenames of people from a text file, and load all those images listed.
int loadobjobjectImgArray(const char * filename)
{
	FILE * imgListFile = 0;
	char imgFilename[512];
	int iFace, nFaces=0;
	int i;

	// open the input file
	if( !(imgListFile = fopen(filename, "r")) )
	{
		fprintf(stderr, "Can\'t open file %s\n", filename);
		return 0;
	}

	// count the number of faces
	while( fgets(imgFilename, sizeof(imgFilename)-1, imgListFile) ) ++nFaces;
	rewind(imgListFile);

	// allocate the face-image array and person number matrix
	objectImgArr        = (IplImage **)cvAlloc( nFaces*sizeof(IplImage *) );
	personobjNumTruthMat = cvCreateMat( 1, nFaces, CV_32SC1 );

	objNames.clear();	// Make sure it starts as empty.
	nobjPersons = 0;

	// store the face images in an array
	for(iFace=0; iFace<nFaces; iFace++)
	{
		char personName[256];
		string sPersonName;
		int personNumber;

		// read person number (beginning with 1), their name and the image filename.
		fscanf(imgListFile, "%d %s %s", &personNumber, personName, imgFilename);
		sPersonName = personName;
		//printf("Got %d: %d, <%s>, <%s>.\n", iFace, personNumber, personName, imgFilename);

		// Check if a new person is being loaded.
		if (personNumber > nobjPersons) {
			// Allocate memory for the extra person (or possibly multiple), using this new person's name.
			for (i=nobjPersons; i < personNumber; i++) {
				objNames.push_back( sPersonName );
			}
			nobjPersons = personNumber;
			//printf("Got new person <%s> -> nobjPersons = %d [%d]\n", sPersonName.c_str(), nobjPersons, objNames.size());
		}

		// Keep the data
		personobjNumTruthMat->data.i[iFace] = personNumber;

		// load the face image
		objectImgArr[iFace] = cvLoadImage(imgFilename, CV_LOAD_IMAGE_GRAYSCALE);

		if( !objectImgArr[iFace] )
		{
			fprintf(stderr, "Can\'t load image from %s\n", imgFilename);
			return 0;
		}
	}

	fclose(imgListFile);

	printf("Data loaded from '%s': (%d images of %d people).\n", filename, nFaces, nobjPersons);
	printf("People: ");
	if (nobjPersons > 0)
		printf("<%s>", objNames[0].c_str());
	for (i=1; i<nobjPersons; i++) {
		printf(", <%s>", objNames[i].c_str());
	}
	printf(".\n");

	return nFaces;
}


// Recognize the face in each of the test images given, and compare the results with the truth.
void recognizeobjFileList(const char *szFileTest)
{
	int i, nTestFaces  = 0;         // the number of test images
	CvMat * trainPersonNumMat = 0;  // the person numbers during training
	float * projectedTestFace = 0;
	const char *answer;
	int nCorrect = 0;
	int nWrong = 0;
	double timeFaceRecognizeStart;
	double tallyFaceRecognizeTime;
	float confidence;

	// load test images and ground truth for person number
	nTestFaces = loadobjobjectImgArray(szFileTest);
	printf("%d test faces loaded\n", nTestFaces);

	// load the saved training data
	if( !loadobjTrainingData( &trainPersonNumMat ) ) return;

	// project the test images onto the PCA subspace
	projectedTestFace = (float *)cvAlloc( nobjEigens*sizeof(float) );
	timeFaceRecognizeStart = (double)cvGetTickCount();	// Record the timing.
	for(i=0; i<nTestFaces; i++)
	{
		int iNearest, nearest, truth;

		// project the test image onto the PCA subspace
		cvEigenDecomposite(
			objectImgArr[i],
			nobjEigens,
			eigenobjVectArr,
			0, 0,
			pobjAvgTrainImg,
			projectedTestFace);

		iNearest = findobjNearestNeighbor(projectedTestFace, &confidence);
		truth    = personobjNumTruthMat->data.i[i];
		nearest  = trainPersonNumMat->data.i[iNearest];

		if (nearest == truth) {
			answer = "Correct";
			nCorrect++;
		}
		else {
			answer = "WRONG!";
			nWrong++;
		}
		printf("nearest = %d, Truth = %d (%s). Confidence = %f\n", nearest, truth, answer, confidence);
	}
	tallyFaceRecognizeTime = (double)cvGetTickCount() - timeFaceRecognizeStart;
	if (nCorrect+nWrong > 0) {
		printf("TOTAL ACCURACY: %d%% out of %d tests.\n", nCorrect * 100/(nCorrect+nWrong), (nCorrect+nWrong));
		printf("TOTAL TIME: %.1fms average.\n", tallyFaceRecognizeTime/((double)cvGetTickFrequency() * 1000.0 * (nCorrect+nWrong) ) );
	}

}


// Grab the next camobjera frame. Waits until the next frame is ready,
// and provides direct access to it, so do NOT modify the returned image or free it!
// Will automatically initialize the camobjera on the first frame.
IplImage* getcamobjeraFrame(void)
{
	IplImage *frame;

	// If the camobjera hasn't been initialized, then open it.
	if (!camobjera) {
		printf("Acessing the camobjera ...\n");
		camobjera = cvCaptureFromCAM( 0 );
		if (!camobjera) {
			printf("ERROR in getcamobjeraFrame(): Couldn't access the camobjera.\n");
			exit(1);
		}
		// Try to set the camobjera resolution
		cvSetCaptureProperty( camobjera, CV_CAP_PROP_FRAME_WIDTH, 320 );
		cvSetCaptureProperty( camobjera, CV_CAP_PROP_FRAME_HEIGHT, 240 );
		// Wait a little, so that the camobjera can auto-adjust itself
		#if defined WIN32 || defined _WIN32
			Sleep(1000);	// (in milliseconds)
		#endif
		frame = cvQueryFrame( camobjera );	// get the first frame, to make sure the camobjera is initialized.
		if (frame) {
			printf("Got a camobjera using a resolution of %dx%d.\n", (int)cvGetCaptureProperty( camobjera, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty( camobjera, CV_CAP_PROP_FRAME_HEIGHT) );
		}
	}

	frame = cvQueryFrame( camobjera );
	if (!frame) {
		fprintf(stderr, "ERROR in recognizeobjFromCam(): Could not access the camobjera or video file.\n");
		exit(1);
		//return NULL;
	}
	return frame;
}

// Return a new image that is always greyscale, whether the input image was RGB or Greyscale.
// Remember to free the returned image using cvReleaseImage() when finished.
IplImage* convertobjImageToGreyscale(const IplImage *imageSrc)
{
	IplImage *imageGrey;
	// Either convert the image to greyscale, or make a copy of the existing greyscale image.
	// This is to make sure that the user can always call cvReleaseImage() on the output, whether it was greyscale or not.
	if (imageSrc->nChannels == 3) {
		imageGrey = cvCreateImage( cvGetSize(imageSrc), IPL_DEPTH_8U, 1 );
		cvCvtColor( imageSrc, imageGrey, CV_BGR2GRAY );
	}
	else {
		imageGrey = cvCloneImage(imageSrc);
	}
	return imageGrey;
}

// Creates a new image copy that is of a desired size.
// Remember to free the new image later.
IplImage* resizeobjImage(const IplImage *origImg, int newWidth, int newHeight)
{
	IplImage *outImg = 0;
	int origWidth;
	int origHeight;
	if (origImg) {
		origWidth = origImg->width;
		origHeight = origImg->height;
	}
	if (newWidth <= 0 || newHeight <= 0 || origImg == 0 || origWidth <= 0 || origHeight <= 0) {
		printf("ERROR in resizeobjImage: Bad desired image size of %dx%d\n.", newWidth, newHeight);
		exit(1);
	}

	// Scale the image to the new dimensions, even if the aspect ratio will be changed.
	outImg = cvCreateImage(cvSize(newWidth, newHeight), origImg->depth, origImg->nChannels);
	if (newWidth > origImg->width && newHeight > origImg->height) {
		// Make the image larger
		cvResetImageROI((IplImage*)origImg);
		cvResize(origImg, outImg, CV_INTER_LINEAR);	// CV_INTER_CUBIC or CV_INTER_LINEAR is good for enlarging
	}
	else {
		// Make the image smaller
		cvResetImageROI((IplImage*)origImg);
		cvResize(origImg, outImg, CV_INTER_AREA);	// CV_INTER_AREA is good for shrinking / decimation, but bad at enlarging.
	}

	return outImg;
}

// Returns a new image that is a cropped version of the original image. 
IplImage* cropobjImage(const IplImage *img, const CvRect region)
{
	IplImage *imageTmp;
	IplImage *imageRGB;
	CvSize size;
	size.height = img->height;
	size.width = img->width;

	if (img->depth != IPL_DEPTH_8U) {
		printf("ERROR in cropobjImage: Unknown image depth of %d given in cropobjImage() instead of 8 bits per pixel.\n", img->depth);
		exit(1);
	}

	// First create a new (color or greyscale) IPL Image and copy contents of img into it.
	imageTmp = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(img, imageTmp, NULL);

	// Create a new image of the detected region
	// Set region of interest to that surrounding the face
	cvSetImageROI(imageTmp, region);
	// Copy region of interest (i.e. face) into a new iplImage (imageRGB) and return it
	size.width = region.width;
	size.height = region.height;
	imageRGB = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(imageTmp, imageRGB, NULL);	// Copy just the region.

    cvReleaseImage( &imageTmp );
	return imageRGB;		
}

// Get an 8-bit equivalent of the 32-bit Float image.
// Returns a new image, so remember to call 'cvReleaseImage()' on the result.
IplImage* convertobjFloatImageToUcharImage(const IplImage *srcImg)
{
	IplImage *dstImg = 0;
	if ((srcImg) && (srcImg->width > 0 && srcImg->height > 0)) {

		// Spread the 32bit floating point pixels to fit within 8bit pixel range.
		double minVal, maxVal;
		cvMinMaxLoc(srcImg, &minVal, &maxVal);

		//cout << "FloatImage:(minV=" << minVal << ", maxV=" << maxVal << ")." << endl;

		// Deal with NaN and extreme values, since the DFT seems to give some NaN results.
		if (cvIsNaN(minVal) || minVal < -1e30)
			minVal = -1e30;
		if (cvIsNaN(maxVal) || maxVal > 1e30)
			maxVal = 1e30;
		if (maxVal-minVal == 0.0f)
			maxVal = minVal + 0.001;	// remove potential divide by zero errors.

		// Convert the format
		dstImg = cvCreateImage(cvSize(srcImg->width, srcImg->height), 8, 1);
		cvConvertScale(srcImg, dstImg, 255.0 / (maxVal - minVal), - minVal * 255.0 / (maxVal-minVal));
	}
	return dstImg;
}

// Store a greyscale floating-point CvMat image into a BMP/JPG/GIF/PNG image,
// since cvSaveImage() can only handle 8bit images (not 32bit float images).
void saveobjFloatImage(const char *filename, const IplImage *srcImg)
{
	//cout << "Saving Float Image '" << filename << "' (" << srcImg->width << "," << srcImg->height << "). " << endl;
	IplImage *byteImg = convertobjFloatImageToUcharImage(srcImg);
	cvSaveImage(filename, byteImg);
	cvReleaseImage(&byteImg);
}

// Perform face detection on the input image, using the given Haar cascade classifier.
// Returns a rectangle for the detected region in the given image.
CvRect detectobjFaceInImage(const IplImage *inputImg, const CvHaarClassifierCascade* cascade )
{
	const CvSize minFeatureSize = cvSize(10, 10);
	const int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;	// Only search for 1 face.
	const float search_scale_factor = 1.1f;
	IplImage *detectImg;
	IplImage *greyImg = 0;
	CvMemStorage* storage;
	CvRect rc;
	double t;
	CvSeq* rects;
	int i;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );

	// If the image is color, use a greyscale copy of the image.
	detectImg = (IplImage*)inputImg;	// Assume the input image is to be used.
	if (inputImg->nChannels > 1) 
	{
		greyImg = cvCreateImage(cvSize(inputImg->width, inputImg->height), IPL_DEPTH_8U, 1 );
		cvCvtColor( inputImg, greyImg, CV_BGR2GRAY );
		detectImg = greyImg;	// Use the greyscale version as the input.
	}

	// Detect all the faces.
	t = (double)cvGetTickCount();
	rects = cvHaarDetectObjects( detectImg, (CvHaarClassifierCascade*)cascade, storage,
				search_scale_factor, 3, flags, minFeatureSize );
	t = (double)cvGetTickCount() - t;
	printf("[Detection took %d ms and found %d objects]\n", cvRound( t/((double)cvGetTickFrequency()*1000.0) ), rects->total );

	// Get the first detected face (the biggest).
	if (rects->total > 0) {
        rc = *(CvRect*)cvGetSeqElem( rects, 0 );
    }
	else
		rc = cvRect(-1,-1,-1,-1);	// Couldn't find the face.

	//cvReleaseHaarClassifierCascade( &cascade );
	//cvReleaseImage( &detectImg );
	if (greyImg)
		cvReleaseImage( &greyImg );
	cvReleaseMemStorage( &storage );

	return rc;	// Return the biggest face found, or (-1,-1,-1,-1).
}

// Re-train the new face rec database without shutting down.
// Depending on the number of images in the training set and number of people, it might take 30 seconds or so.
CvMat* retrainobjOnline(void)
{
	CvMat *trainPersonNumMat;
	int i;

	// Free & Re-initialize the global variables.
	if (objectImgArr) {
		for (i=0; i<nobjTrainFaces ; i++) {
			if (objectImgArr[i])
				cvReleaseImage( &objectImgArr[i] );
		}
	}
	cvFree( &objectImgArr ); // array of face images
	cvFree( &personobjNumTruthMat ); // array of person numbers
	objNames.clear();			// array of person names (indexed by the person number). Added by Shervin.
	nobjPersons = 0; // the number of people in the training set. Added by Shervin.
	nobjTrainFaces = 0; // the number of training images
	nobjEigens = 0; // the number of eigenvalues
	cvReleaseImage( &pobjAvgTrainImg ); // the average image
	for (i=0; i<nobjTrainFaces ; i++) {
		if (eigenobjVectArr[i])
			cvReleaseImage( &eigenobjVectArr[i] );
	}
	cvFree( &eigenobjVectArr ); // eigenvectors
	cvFree( &eigenobjValMat ); // eigenvalues
	cvFree( &projectedobjTrainFaceMat ); // projected training faces

	// Retrain from the data in the files
	printf("Retraining with the new person ...\n");
	leaobjrn("objecttrain.txt");
	printf("Done retraining.\n");

	// Load the previously saved training data
	if( !loadobjTrainingData( &trainPersonNumMat ) ) {
		printf("ERROR in recognizeobjFromCam(): Couldn't load the training data!\n");
		exit(1);
	}

	return trainPersonNumMat;
}

// Continuously recognize the person in the camobjera.
void recognizeobjFromCam(void)
{
	int i;
	CvMat * trainPersonNumMat;  // the person numbers during training
	float * projectedTestFace;
	double timeFaceRecognizeStart;
	double tallyFaceRecognizeTime;
	CvHaarClassifierCascade* faceCascade;
	CvHaarClassifierCascade* faceCascade1;
	CvHaarClassifierCascade* faceCascade2;
	CvHaarClassifierCascade* faceCascade3;
	char cstr[256];
	BOOL saveNextFaces = FALSE;
	char newPersonName[256];
	int newPersonFaces;

	trainPersonNumMat = 0;  // the person numbers during training
	projectedTestFace = 0;
	saveNextFaces = FALSE;
	newPersonFaces = 0;

	printf("hello Recognizing person in the camobjera ...\n");

	// Load the previously saved training data
	if( loadobjTrainingData( &trainPersonNumMat ) ) {
		objectWidth = pobjAvgTrainImg->width;
		objectHeight = pobjAvgTrainImg->height;
	}
	else {
		//printf("ERROR in recognizeobjFromCam(): Couldn't load the training data!\n");
		//exit(1);
	}

	// Project the test images onto the PCA subspace
	projectedTestFace = (float *)cvAlloc( nobjEigens*sizeof(float) );

	// Create a GUI window for the user to see the camobjera image.
	cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);

	// Make sure there is a "data" folder, for storing the new person.
	#if defined WIN32 || defined _WIN32
		mkdir("data");
	#else
		// For Linux, make the folder to be Read-Write-Executable for this user & group but only Readable for others.
		mkdir("data", S_IRWXU | S_IRWXG | S_IROTH);
	#endif
		faceCascade1 = (CvHaarClassifierCascade*)cvLoad(objCascadeFilename[0], 0, 0, 0 );
		faceCascade2 = (CvHaarClassifierCascade*)cvLoad(objCascadeFilename[1], 0, 0, 0 );
		faceCascade3 = (CvHaarClassifierCascade*)cvLoad(objCascadeFilename[2], 0, 0, 0 );
	changeKeyboardMode(1);

	timeFaceRecognizeStart = (double)cvGetTickCount();	// Record the timing.

	while (1)
	{
	// Load the HaarCascade classifier for face detection.
		if(fileflag==0)
		{
			faceCascade=faceCascade1;		
			fileflag=1;
							printf("LODED 7UP\n");
		}
		else
			if(fileflag==1)
			{

				faceCascade=faceCascade2;				
				fileflag=2;
				printf("LODED mIRINDA\n");
			}
			else
			if(fileflag==2)
			{

				faceCascade=faceCascade3;
				fileflag=0;
								printf("LODED powder");
			}
	if( !faceCascade ) {
		printf("ERROR in recognizeobjFromCam(): Could not load Haar cascade Face detection classifier in '%s'.\n", objCascadeFilename);
		exit(1);
	}

	// Tell the Linux terminal to return the 1st keypress instead of waiting for an ENTER key.
	
		int iNearest, nearest, truth;
		IplImage *camImg;
		IplImage *greyImg;
		IplImage *faceImg;
		IplImage *sizedImg;
		IplImage *equalizedImg;
		IplImage *processedFaceImg;
		CvRect faceRect;
		IplImage *shownImg;
		int keyPressed = 0;
		FILE *trainFile;
		float confidence;

		// Handle non-blocking keyboard input in the console.
		if (kbhit())
			keyPressed = getch();
		
		if (keyPressed == VK_ESCAPE) {	// Check if the user hit the 'Escape' key
			break;	// Stop processing input.
		}
		switch (keyPressed) {
			case 'n':	// Add a new person to the training set.
				// Train from the following images.
				printf("Enter your name: ");
				strcpy(newPersonName, "newPerson");

				// Read a string from the console. Waits until they hit ENTER.
				changeKeyboardMode(0);
				fgets(newPersonName, sizeof(newPersonName)-1, stdin);
				changeKeyboardMode(1);
				// Remove 1 or 2 newline characters if they were appended (eg: Linux).
				i = strlen(newPersonName);
				if (i > 0 && (newPersonName[i-1] == 10 || newPersonName[i-1] == 13)) {
					newPersonName[i-1] = 0;
					i--;
				}
				if (i > 0 && (newPersonName[i-1] == 10 || newPersonName[i-1] == 13)) {
					newPersonName[i-1] = 0;
					i--;
				}
				
				if (i > 0) {
					printf("Collecting all images until you hit 't', to start Training the images as '%s' ...\n", newPersonName);
					newPersonFaces = 0;	// restart training a new person
					saveNextFaces = TRUE;
				}
				else {
					printf("Did not get a valid name from you, so will ignore it. Hit 'n' to retry.\n");
				}
				break;
			case 't':	// Start training
				saveNextFaces = FALSE;	// stop saving next faces. 
				// Store the saved data into the training file.
				printf("Storing the training data for new person '%s'.\n", newPersonName);
				// Append the new person to the end of the training data.
				trainFile = fopen("objecttrain.txt", "a");
				for (i=0; i<newPersonFaces; i++) {
					snprintf(cstr, sizeof(cstr)-1, "data/%d_%s%d.pgm", nobjPersons+1, newPersonName, i+1);
					fprintf(trainFile, "%d %s %s\n", nobjPersons+1, newPersonName, cstr);
				}
				fclose(trainFile);

				// Now there is one more person in the database, ready for retraining.
				//nobjPersons++;

				//break;
			//case 'r':

				// Re-initialize the local data.
				projectedTestFace = 0;
				saveNextFaces = FALSE;
				newPersonFaces = 0;

				// Retrain from the new database without shutting down.
				// Depending on the number of images in the training set and number of people, it might take 30 seconds or so.
				cvFree( &trainPersonNumMat );	// Free the previous data before getting new data
				trainPersonNumMat = retrainobjOnline();
				// Project the test images onto the PCA subspace
				cvFree(&projectedTestFace);	// Free the previous data before getting new data
				projectedTestFace = (float *)cvAlloc( nobjEigens*sizeof(float) );

				printf("Recognizing person in the camobjera ...\n");
				continue;	// Begin with the next frame.
				break;
		}

		// Get the camobjera frame
		camImg = getcamobjeraFrame();
		if (!camImg) {
			printf("ERROR in recognizeobjFromCam(): Bad input image!\n");
			exit(1);
		}
		// Make sure the image is greyscale, since the Eigenfaces is only done on greyscale image.
		greyImg = convertobjImageToGreyscale(camImg);

		// Perform face detection on the input image, using the given Haar cascade classifier.
		faceRect = detectobjFaceInImage(greyImg, faceCascade );
		// Make sure a valid face was detected.
				printf("Entered");
		
		if (faceRect.width > 0 && faceRect.height>=100) {
			faceImg = cropobjImage(greyImg, faceRect);	// Get the detected face image.
			// Make sure the image is the same dimensions as the training images.
			sizedImg = resizeobjImage(faceImg, objectWidth, objectHeight);
			// Give the image a standard brightness and contrast, in case it was too dark or low contrast.
			equalizedImg = cvCreateImage(cvGetSize(sizedImg), 8, 1);	// Create an empty greyscale image
			cvEqualizeHist(sizedImg, equalizedImg);
			processedFaceImg = equalizedImg;
			if (!processedFaceImg) {
				printf("ERROR in recognizeobjFromCam(): Don't have input image!\n");
				exit(1);
			}
			printf("entered2");
			// If the face rec database has been loaded, then try to recognize the person currently detected.
			if (nobjEigens > 0) {
				// project the test image onto the PCA subspace
				cvEigenDecomposite(
					processedFaceImg,
					nobjEigens,
					eigenobjVectArr,
					0, 0,
					pobjAvgTrainImg,
					projectedTestFace);

				// Check which person it is most likely to be.
				iNearest = findobjNearestNeighbor(projectedTestFace, &confidence);
				nearest  = trainPersonNumMat->data.i[iNearest];

				printf("Most likely person in camobjera: '%s' (confidence=%f).\n", objNames[nearest-1].c_str(), confidence);

			}//endif nobjEigens

			// Possibly save the processed face to the training set.
			if (saveNextFaces) {
// MAYBE GET IT TO ONLY TRAIN SOME IMAGES ?
				// Use a different filename each time.
				snprintf(cstr, sizeof(cstr)-1, "data/%d_%s%d.pgm", nobjPersons+1, newPersonName, newPersonFaces+1);
				printf("Storing the current face of '%s' into image '%s'.\n", newPersonName, cstr);
				cvSaveImage(cstr, processedFaceImg, NULL);
				newPersonFaces++;
			}

			// Free the resources used for this frame.
			cvReleaseImage( &greyImg );
			cvReleaseImage( &faceImg );
			cvReleaseImage( &sizedImg );
			cvReleaseImage( &equalizedImg );
		}
printf("entered3");
		// Show the data on the screen.
		shownImg = cvCloneImage(camImg);
		if (faceRect.width > 0&&faceRect.height>=100) {	// Check if a face was detected.
			// Show the detected face region.
			printf("entered4");
			cvRectangle(shownImg, cvPoint(faceRect.x, faceRect.y), cvPoint(faceRect.x + faceRect.width-1, faceRect.y + faceRect.height-1), CV_RGB(0,255,0), 1, 8, 0);
			if (nobjEigens > 0) {	// Check if the face recognition database is loaded and a person was recognized.
				// Show the name of the recognized person, overlayed on the image below their face.
				CvFont font;
				cvInitFont(&font,CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0,1,CV_AA);
				CvScalar textColor = CV_RGB(0,255,255);	// light blue text
				char text[256];
				snprintf(text, sizeof(text)-1, "Name: '%s'", objNames[nearest-1].c_str());
				cvPutText(shownImg, text, cvPoint(faceRect.x, faceRect.y + faceRect.height + 15), &font, textColor);
				//snprintf(text, sizeof(text)-1, "Confidence: %f", confidence);
				//cvPutText(shownImg, text, cvPoint(faceRect.x, faceRect.y + faceRect.height + 30), &font, textColor);
			}
		}

		// Display the image.
		cvShowImage("Input", shownImg);

		// Give some time for OpenCV to draw the GUI and check if the user has pressed something in the GUI window.
		keyPressed = cvWaitKey(200);
		if (keyPressed == VK_ESCAPE) {	// Check if the user hit the 'Escape' key in the GUI window.
			break;	// Stop processing input.
		}

		cvReleaseImage( &shownImg );
	}
	tallyFaceRecognizeTime = (double)cvGetTickCount() - timeFaceRecognizeStart;

	// Reset the Linux terminal back to the original settings.
	changeKeyboardMode(0);

	// Free the camobjera and memory resources used.
	cvReleaseCapture( &camobjera );
	cvReleaseHaarClassifierCascade( &faceCascade );
}
