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
//		PRÁCTICAS ASD - ALEJANDRO FERNÁNDEZ TRIGO	
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


// Based on example 3 page 20 of OReilly Learning OpenCV.pdf:
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
void asd_blurring_simplest_own( IplImage* img );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on example 2 page 18 of OReilly Learning OpenCV.pdf:
int main(int argc, char** argv) {

	char *file_name = "Megamind.avi";

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
	
#endif 

	// structures to point to a frame of a video, and capturing the first frame
	IplImage* newframe;
	IplImage* oldframe;
	newframe = cvQueryFrame( g_capture );
	char key = cvWaitKey(1); //wait several microseconds to show correctly the images

	QPTimer crono_opencv_lib;
	QPTimer crono_my_own_blurring;
	crono_opencv_lib.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	crono_my_own_blurring.Calibrate(); //calibrates timer overhead and set cronometer to zero	

	// main loop where frames are processed
	while(newframe && (key != 27 ) ) {

		oldframe=newframe;

#ifdef __VIDEO_ON
		cvShowImage( "asd_P1-in", oldframe );
		 
#endif 

		IplImage* out = cvCreateImage(
			cvGetSize(oldframe),
			IPL_DEPTH_8U,
			3
			);
				
		// start timer
		crono_opencv_lib.Start();
		
		// Do the smoothing using OpenCV
		cvSmooth( oldframe, out, CV_BLUR , 3, 3 );  
		
		// stop timer
		crono_opencv_lib.Stop();
		crono_opencv_lib.Reset();
		
		// Do the smoothing using one of my own functions
		crono_my_own_blurring.Start();

		// LLAMADA A LA FUNCIÓN DE DESENROLLADO DE CÓDIGO COMPLETO
		asd_blurring_simplest_own ( oldframe ); 

		crono_my_own_blurring.Stop();
		crono_my_own_blurring.Reset();

#ifdef __VIDEO_ON
		// Show the smoothed image in the output window
		cvShowImage( "asd_P1-out-OPENCV", out );
		cvReleaseImage( &out );

		cvShowImage( "asd_P1-out-MY-OWN-BLURRING", oldframe);

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
	
	cvReleaseCapture( &g_capture );
	
	getchar();
	
#ifdef __VIDEO_ON
	cvDestroyWindow( "asd_P1-in" );
	cvDestroyWindow( "asd_P1-out" );
#endif
}

// CÓDIGO COMPLETAMENTE DESENROLLADO POR MI - PRÁCTICA 3
void asd_blurring_unrolled_own ( IplImage* img ) {
	for( int y=1; y<(img->height)-1; y++ ) {
	
		uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
		int image_width = img->width;

		for( int x=1; x<(image_width -1); x++ ) {

				int sum0=0;
				int sum1=0;
				int sum2=0;

				// ###################################

				sum0 += ptr[3*x+0];
				sum0 += ptr[3*(x-1)+0];
				sum += ptr[3*(x+1)+0];

				sum1 += ptr[3*x+1];
				sum1 += ptr[3*(x-1)+1];
				sum1 += ptr[3*(x+1)+1];

				sum2 += ptr[3*x+2];
				sum2 += ptr[3*(x-1)+2];
				sum2 += ptr[3*(x+1)+2];
				
				// ###################################

				sum0 += ptr[3*x+3*image_width +0];
				sum0 += ptr[3*(x-1)+3*image_width +0];
				sum0 += ptr[3*(x+1)+3*image_width +0];

				sum1 += ptr[3*x+3*image_width +1];
				sum1 += ptr[3*(x-1)+3*image_width +1];
				sum1 += ptr[3*(x+1)+3*image_width +1];

				sum2 += ptr[3*x+3*image_width +2];
				sum2 += ptr[3*(x-1)+3*image_width +2];
				sum2 += ptr[3*(x+1)+3*image_width +2];

				// ###################################
				
				sum0 += ptr[3*x-3*image_width +0];
				sum0 += ptr[3*(x-1)-3*image_width +0];
				sum0 += ptr[3*(x+1)-3*image_width +0];

				sum1 += ptr[3*x-3*image_width +1];
				sum1 += ptr[3*(x-1)-3*image_width +1];
				sum1 += ptr[3*(x+1)-3*image_width +1];

				sum2 += ptr[3*x-3*image_width +2];
				sum2 += ptr[3*(x-1)-3*image_width +2];
				sum2 += ptr[3*(x+1)-3*image_width +2];

				// ###################################
				
				ptr[3*x+0] = sum0/9;
				ptr[3*x+1] = sum1/9;
				ptr[3*x+2] = sum2/9;

		}
	}
}
