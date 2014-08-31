#include "opencv/cv.h"
#include "opencv/highgui.h"
 
#include <stdlib.h>
#include <stdio.h>
 
void drawEdges(int threshold1, int threshold2, int apertureSize, IplImage *image, CvVideoWriter *writer) {

        IplImage *edges;

        edges = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);

	// Convert image from color to gray
        cvCvtColor(image, edges, CV_BGR2GRAY);
	// Mirror
        //cvFlip(edges, edges, 1);
	// Find edges
	cvCanny(edges, edges, threshold1, threshold2, apertureSize);	
	// Save the image
	IplImage *final = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
	cvMerge(edges, edges, edges, NULL, final); 
	cvWriteFrame(writer, final);
	// Display the image with edges
        cvShowImage("Edges", final);
	// Release the image
	cvReleaseImage(&edges);
}

int main(int argc, char** argv) {
 
	// Image
	IplImage *image = 0;
	// Video Capture
	CvCapture *capture = 0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	// Keyboard event
	char key;

	int cannyThreshold1 = 50, cannyThreshold2 = 50, cannyApertureSize = 3;

	// Initializes HighGUI
	cvInitSystem(argc, argv);

	// Initialize the video capture
    	if (!(capture = cvCaptureFromCAM(0))) {
		printf("Can't initialize the video capture.\n");
        	return -1;
 	}
 
	// Create the window
   	cvNamedWindow("Edges", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Edges", 0, 50);

	// Create trackbars
	cvCreateTrackbar( "Threshold1", "Edges", &cannyThreshold1, 255,  NULL);
	cvCreateTrackbar( "Threshold2", "Edges", &cannyThreshold2, 255,  NULL);
	
	CvSize size = cvSize ((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter *writer = cvCreateVideoWriter(
	   "edges.avi",
	   CV_FOURCC('D','I','V','X'),
	   15,
	    size, 1);

	// While we don't want to quit
	while (key != 'Q' && key != 'q') {
 
		// Get the current image
		// from the video capture
		if (!(image = cvQueryFrame(capture)))
			continue;
			
		drawEdges(cannyThreshold1, cannyThreshold2, cannyApertureSize, image, writer);

		// We wait 10 ms
		key = cvWaitKey(10);
	}
 
	// Destroy the window
	cvDestroyWindow("Edges");
 
	// Destroy the writer
	cvReleaseVideoWriter(&writer);
 
	// Destroy the capture
	cvReleaseCapture(&capture);
 
	return 0;
}
