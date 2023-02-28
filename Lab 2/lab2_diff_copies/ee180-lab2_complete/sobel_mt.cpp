#include <stdio.h>
#include <stdlib.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <err.h>

#include "sobel_alg.h"
#include "pc.h"

using namespace cv;

static ofstream results_file;

// Define image mats to pass between function calls
static Mat img_gray, img_sobel, src; //made src global
static float total_fps, total_ipc, total_epf;
static float gray_total, sobel_total, cap_total, disp_total;
static float sobel_ic_total, sobel_l1cm_total;
//static int i = 0; // # frames counter
//static CvCapture *video_cap; 

/*******************************************
 * Model: runSobelMT
 * Input: None
 * Output: None
 * Desc: This method pulls in an image from the webcam, feeds it into the
 *   sobelCalc module, and displays the returned Sobel filtered image. This
 *   function processes NUM_ITER frames.
 ********************************************/
void *runSobelMT(void *ptr)
{
  // Set up variables for computing Sobel
  string top = "Sobel Top";
  Mat adj_src, adj_gray, adj_sobel; // created two Mats for mt
  uint64_t cap_time, gray_time, sobel_time, disp_time, sobel_l1cm, sobel_ic;
  pthread_t myID = pthread_self();
  counters_t perf_counters;

  // Allow the threads to contest for thread0 (controller thread) status
  pthread_mutex_lock(&thread0);

  // Check to see if this thread is first to this part of the code
  if (thread0_id == 0) {
    thread0_id = myID;
  }
  pthread_mutex_unlock(&thread0);

 // if(thread0_id == myID){
  pc_init(&perf_counters, 0);
//  }
  // Start algorithm
  CvCapture* video_cap;
 // if (thread0_id == myID){
  if (opts.webcam) {
  	 video_cap = cvCreateCameraCapture(-1);
  } else {
   	 video_cap = cvCreateFileCapture(opts.videoFile);
  }

  video_cap = cvCreateFileCapture(opts.videoFile);
  cvSetCaptureProperty(video_cap, CV_CAP_PROP_FRAME_WIDTH, IMG_WIDTH);
  cvSetCaptureProperty(video_cap, CV_CAP_PROP_FRAME_HEIGHT, IMG_HEIGHT);
 // }
  pthread_barrier_wait(&vid_barrier);
//  printf("vid\n");

  // Keep track of the frames
  int i = 0; 

  while (1) {
 // if(thread0_id == myID){
    // Allocate memory to hold grayscale and sobel images
    img_gray = Mat(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);
    img_sobel = Mat(IMG_HEIGHT, IMG_WIDTH, CV_8UC1);

    pc_start(&perf_counters);
    src = cvQueryFrame(video_cap);
    pc_stop(&perf_counters);

    cap_time = perf_counters.cycles.count;
    sobel_l1cm = perf_counters.l1_misses.count;
    sobel_ic = perf_counters.ic.count;
//  }
  pthread_barrier_wait(&while_barrier); 
  // printf("while\n");

    // set img up for different threads
    if (thread0_id == myID) {
	adj_src = src(Range(0, IMG_HEIGHT/2), Range::all()); // top half
	adj_gray = img_gray(Range(0, IMG_HEIGHT/2), Range::all());
    } else {
	adj_src = src(Range(IMG_HEIGHT/2, IMG_HEIGHT), Range::all()); // bottom half
	adj_gray = img_gray(Range(IMG_HEIGHT/2, IMG_HEIGHT), Range::all());
    }
    pthread_barrier_wait(&pregray_barrier);
   // printf("pregray\n");

    // LAB 2, PART 2: Start parallel section
    /* grayScale call */

 // if(thread0_id == myID){
    pc_start(&perf_counters);
 // }
    grayScale(adj_src, adj_gray);
    pthread_barrier_wait(&gray_barrier); // gray_barrier = barrier for grayScale
   // printf("after gray\n");

// kill second thread
/*    if( myID != thread0_id){
	printf("killed\n");
	pthread_barrier_wait(&endSobel);
    }*/

 // if(thread0_id == myID){
    pc_stop(&perf_counters);
	
    // update counters with grayScale performance
    gray_time = perf_counters.cycles.count;
    sobel_l1cm += perf_counters.l1_misses.count;
    sobel_ic += perf_counters.ic.count;
 // }
    /* sobelCalc call */
    // set img up for different threads
    if (thread0_id == myID) {
	adj_gray = img_gray(Range(0, (IMG_HEIGHT/2)), Range::all()); // top half
 	adj_sobel = img_sobel(Range(0, (IMG_HEIGHT/2)), Range::all());
    } else {
	adj_gray = img_gray(Range((IMG_HEIGHT/2), IMG_HEIGHT), Range::all()); // bottom half
	adj_sobel = img_sobel(Range((IMG_HEIGHT/2) , IMG_HEIGHT), Range::all());
    }

   // printf("presobel\n");
    pthread_barrier_wait(&presobel_barrier);
    
 // if(thread0_id == myID){
    pc_start(&perf_counters);				
 // }
    sobelCalc(adj_gray, adj_sobel);
    pthread_barrier_wait(&sobel_barrier); // sobel_barrier = barrier for sobelCalc
   // printf("after sobel \n");
    
 // if(thread0_id == myID){
    // update counters with sobelCalc performance
    pc_stop(&perf_counters);		
    sobel_time = perf_counters.cycles.count;
    sobel_l1cm += perf_counters.l1_misses.count;
    sobel_ic += perf_counters.ic.count;
   
    // LAB 2, PART 2: End parallel section

    pc_start(&perf_counters);
    namedWindow(top, CV_WINDOW_AUTOSIZE);
    imshow(top, img_sobel);
    pc_stop(&perf_counters);

    disp_time = perf_counters.cycles.count;
    sobel_l1cm += perf_counters.l1_misses.count;
    sobel_ic += perf_counters.ic.count;

    cap_total += cap_time;
    gray_total += gray_time;
    sobel_total += sobel_time;
    sobel_l1cm_total += sobel_l1cm;
    sobel_ic_total += sobel_ic;
    disp_total += disp_time;
    total_fps += PROC_FREQ/float(cap_time + disp_time + gray_time + sobel_time);
    total_ipc += float(sobel_ic/float(cap_time + disp_time + gray_time + sobel_time));

    i++; 
 //}
    // Press q to exit
    char c = cvWaitKey(10);
    pthread_barrier_wait(&q_barrier);
    if (c == 'q' || i >= opts.numFrames) {
      break;
    }
  }


 // if(thread0_id == myID){
  total_epf = PROC_EPC*NCORES/(total_fps/i);
  float total_time = float(gray_total + sobel_total + cap_total + disp_total);

  results_file.open("mt_perf.csv", ios::out);
  results_file << "Percent of time per function" << endl;
  results_file << "Capture, " << (cap_total/total_time)*100 << "%" << endl;
  results_file << "Grayscale, " << (gray_total/total_time)*100 << "%" << endl;
  results_file << "Sobel, " << (sobel_total/total_time)*100 << "%" << endl;
  results_file << "Display, " << (disp_total/total_time)*100 << "%" << endl;
  results_file << "\nSummary" << endl;
  results_file << "Frames per second, " << total_fps/i << endl;
  results_file << "Cycles per frame, " << total_time/i << endl;
  results_file << "Energy per frames (mJ), " << total_epf*1000 << endl;
  results_file << "Total frames, " << i << endl;
  results_file << "\nHardware Stats (Cap + Gray + Sobel + Display)" << endl;
  results_file << "Instructions per cycle, " << total_ipc/i << endl;
  results_file << "L1 misses per frame, " << sobel_l1cm_total/i << endl;
  results_file << "L1 misses per instruction, " << sobel_l1cm_total/sobel_ic_total << endl;
  results_file << "Instruction count per frame, " << sobel_ic_total/i << endl;

  //if(thread0_id == myID){
  cvReleaseCapture(&video_cap);
  
  results_file.close();
// }
  pthread_barrier_wait(&endSobel); // final barrier (given)
  return NULL;
}
