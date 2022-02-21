//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD  subject 3º GTI
//  version 1. Sept. 2012. Fernando Diaz del Rio
//
//  - OPENCV TEST TO SHOW A VIDEO OR AN IMAGE 
//		(based on several examples from "Learning OpenCV. OREILLY BOOK"; page numbers are referred to this book)
//  - TEST TIMING OF OPENCV AND OWN FUNCTION TO FILTER FRAMES FROM A VIDEO 
//
//-----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <fstream>



//------------------------------------------------------------------
// project def and includes
#define __VIDEO_ON___   
// if this constant were not defined, execution will be faster

#include "QueryPerformanceTiming_rdtsc.h"

using namespace std;


// BAsed on example 3 page 20 of OReilly Learning OpenCV.pdf:
int g_slider_position = 0;
int g_slider_position_counter = 0;
CvCapture* g_capture = NULL;

void onTrackbarSlide(int pos) {
	cvSetCaptureProperty(
		g_capture,
		CV_CAP_PROP_POS_FRAMES,
		//pos
		g_slider_position
		);

	//refresh variable that counts the slider positiion
	g_slider_position_counter = g_slider_position ; 

}

//-----------------------------------------------------
// My own blurring functions

void asd_blurring_unrolled( IplImage* img );
void asd_blurring_simplest ( IplImage* img , int kernel_radius);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on example 2 page 18 of OReilly Learning OpenCV.pdf:
int main(int argc, char** argv) {

	char *file_name = "Megamind.avi";
	// Alternatives ways to read a file name:
	//file_name=argv[1] ;
	//cout << "Name of avi file: " ; char file_name[255]; gets(file_name);
#ifdef __VIDEO_ON
	cvNamedWindow("asd_P1-in", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("asd_P1-out-OPENCV", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("asd_P1-out-MY-OWN-BLURRING", CV_WINDOW_AUTOSIZE);

#endif 
#ifndef __VIDEO_ON
	cout << "Drawing a point for each processed frame:" << endl;
#endif
	g_capture = cvCreateFileCapture( file_name );
    int frames = (int) cvGetCaptureProperty(
		g_capture,
		CV_CAP_PROP_FRAME_COUNT
		);

#ifdef __VIDEO_ON
	if( frames!= 0 ) {
		cvCreateTrackbar(
			"Position",
			"asd_P1-in",
			&g_slider_position,
			frames,
			onTrackbarSlide
			);
	}
	//the scroll slide does not appear in the window if it is small (e.g.128x128)???
#endif 

	// structures to point to a frame of a video, and capturing the first frame
	IplImage* newframe;
	IplImage* oldframe;
	newframe = cvQueryFrame( g_capture );
	char key = cvWaitKey(1); //wait several microseconds to show correctly the images
	//(this is not real time, in fact it lasts a lot and images are shown very slowly)

	QPTimer crono_opencv_lib;
	QPTimer crono_my_own_blurring;
	crono_opencv_lib.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	crono_my_own_blurring.Calibrate(); //calibrates timer overhead and set cronometer to zero	

	// main loop where frames are processed
	while(newframe && (key != 27 ) ) {

		oldframe=newframe;

#ifdef __VIDEO_ON
		cvShowImage( "asd_P1-in", oldframe );
		// the colour change sometimes for the same avi video??? 
#endif 
		// example 4 page 23:  Create an image to hold the smoothed output
		IplImage* out = cvCreateImage(
			cvGetSize(oldframe),
			IPL_DEPTH_8U,
			3
			);
				
		// start timer
		crono_opencv_lib.Start();
		
		// Do the smoothing using OpenCV
		cvSmooth( oldframe, out, CV_BLUR , 3, 3 );  
		//cvSmooth( oldframe, out, CV_GAUSSIAN, 3, 3 ); //another filters (see OpenCV user manual)
		
		// stop timer
		crono_opencv_lib.Stop();
		crono_opencv_lib.Reset();
		
		// Do the smoothing using one of my own functions
		crono_my_own_blurring.Start();

		//UNCOMMENT THIS FUNCTION (and coment the other) WHEN NECESSARY :
		asd_blurring_unrolled  ( oldframe  ); //this function modify image (parameter)
		//asd_blurring_simplest ( oldframe , 1 ); //this function modify image (parameter)

		crono_my_own_blurring.Stop();
		crono_my_own_blurring.Reset();

#ifdef __VIDEO_ON
		// Show the smoothed image in the output window
		cvShowImage( "asd_P1-out-OPENCV", out );
		cvReleaseImage( &out );

		cvShowImage( "asd_P1-out-MY-OWN-BLURRING", oldframe);
		/*
		//		cvReleaseImage( &oldframe );
		Amazing!! FROM opencv2refman.pdf "4.2. Reading and Writing Images and Video":
		It is not allowed to modify or release the image! You can copy the frame using cvCloneImage() and
		then do whatever you want with the copy.
		*/
		// based on exercise pag 21 footnote.
		g_slider_position_counter ++;
		cvSetTrackbarPos(
			"Position",
			"asd_P1-in",
			g_slider_position_counter 
			); 
#endif

#ifndef __VIDEO_ON
		cout <<".";
#endif

		newframe = cvQueryFrame( g_capture );
		key = cvWaitKey(1);
	}  // end of:   while(newframe && (key != 27 ) ) {

	cout << endl << "-Number of measures: " << crono_opencv_lib.NumberOfMeasures() << endl;

	cout << "-OPENCV LIBRARY TIMES " << endl;
	crono_opencv_lib.PrintMinimumTime   (" Minimum time in seconds for a filtered frame is:   ");
	crono_opencv_lib.PrintMeanTime      (" Mean time in seconds for a filtered frame is   :   ");
	crono_opencv_lib.PrintMinimumCycles (" Minimum time in cycles for a filtered frame is :   ");
	crono_opencv_lib.PrintMeanCycles    (" Mean time in cycles for a filtered frame is    :   ");

	cout << endl << "-MY OWN BLURRING FUNCTION TIMES " << endl;
	crono_my_own_blurring.PrintMinimumTime   (" Minimum time in seconds for a filtered frame is:   ");
	crono_my_own_blurring.PrintMeanTime      (" Mean time in seconds for a filtered frame is   :   ");
	crono_my_own_blurring.PrintMinimumCycles (" Minimum time in cycles for a filtered frame is :   ");
	crono_my_own_blurring.PrintMeanCycles    (" Mean time in cycles for a filtered frame is    :   ");
	
	//if (oldframe) cvReleaseImage( &oldframe );
	/*
	//I do not why , but this cvReleaseImage crahses, and it contains a valid pointer to IplImage struct (and it is similar to that of ex. 1)!!!@
	amazing!! FROM opencv2refman.pdf "4.2. Reading and Writing Images and Video":
	It is not allowed to modify or release the image! You can copy the frame using cvCloneImage() and
	then do whatever you want with the copy.
	*/
	cvReleaseCapture( &g_capture );
	
	getchar();
	
#ifdef __VIDEO_ON
	cvDestroyWindow( "asd_P1-in" );
	cvDestroyWindow( "asd_P1-out" );
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// My own blurring function (the "simplest")
void asd_blurring_simplest ( IplImage* img , int kernel_radius) {
	int number_of_neighbours = (1+2*kernel_radius)*(1+2*kernel_radius);
	// Remark: this loop has been  chosen to avoid boundary conditions be checked. 
	// in fact two rows and two columns are not processed (this is a little difference: less than 0.3%)
	for( int y=1; y<(img->height)-1; y++ ) {

		// for( int y=0; y<img->height; y++ ) {   

		//THIS POINTER ptr (to unsigned char) will point to the 
		// first byte of a row in a RGB image 
		// Do not worry about the fields of this structure; they are given by OpenCV
    	uchar* ptr = (uchar*) (
			img->imageData + y * img->widthStep
			); //@comment
	
		int image_width = img->width;
		// Remark: this loop has been  chosen to avoid boundary conditions be checked. 
		// in fact two rows and two columns are not processed (this is a little difference: less than 0.3%)
		for( int x=1; x<(image_width -1); x++ ) {
	
			//	for( int x=0; x<image_width ; x++ ) {  
			for( int color=0; color<3; color++ ) {

				int sum=0;
				for( int yshift = -kernel_radius; yshift <= kernel_radius; yshift++ ) {
					for( int xshift = -kernel_radius; xshift <= kernel_radius; xshift++ ) {
						sum += ptr[3*(x+xshift)+3*image_width*yshift +color];
					}
				}
				ptr[3*x+color] = sum/number_of_neighbours ;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// My own blurring function (UNROLLED). 
// another way to write the filter
// ONLY VALID FOR 9 NEIGHBOURS !!... BUT VERY MUCH FASTER THAN THE GENERIC FILTER !!
void asd_blurring_unrolled ( IplImage* img ) {
	// Remark: this loop has been  chosen to avoid boundary conditions be checked. 
	// in fact two rows and two columns are not processed (this is a little difference: less than 0.3%)
	for( int y=1; y<(img->height)-1; y++ ) {
	
		// if you would like to use the next loop like, remember that boundary lines should be processed independently
		//for( int y=0; y<img->height; y++ ) {
		uchar* ptr = (uchar*) (
			img->imageData + y * img->widthStep
			);
		int image_width = img->width;

		// Remark: this loop has been  chosen to avoid boundary conditions be checked. 
		// in fact two rows and two columns are not processed (this is a little difference: less than 0.3%)
		for( int x=1; x<(image_width -1); x++ ) {

			// if you would like to use the next loop like, remember that boundary lines should be processed independently
			//for( int x=0; x<image_width ; x++ ) {
			for( int color=0; color<3; color++ ) {
				int sum=0;
				sum += ptr[3*x+color];
				sum += ptr[3*(x-1)+color];
				sum += ptr[3*(x+1)+color];
				
				sum += ptr[3*x+3*image_width +color];
				sum += ptr[3*(x-1)+3*image_width +color];
				sum += ptr[3*(x+1)+3*image_width +color];
				
				sum += ptr[3*x-3*image_width +color];
				sum += ptr[3*(x-1)-3*image_width +color];
				sum += ptr[3*(x+1)-3*image_width +color];
				
				ptr[3*x+color] = sum/9;
			}
		}
	}
}
