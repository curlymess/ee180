#include "opencv2/imgproc/imgproc.hpp"
#include "sobel_alg.h"
#include <arm_neon.h>
using namespace cv;

/*******************************************
 * Model: grayScale
 * Input: Mat img
 * Output: None directly. Modifies a ref parameter img_gray_out
 * Desc: This module converts the image to grayscale
 ********************************************/
/*
void grayScale(Mat& img, Mat& img_gray_out)
{
  int color;
  unsigned char * ptr = img.data;
 // printf("%p", ptr );
  // Convert to grayscale
  for (int i=0; i<img.rows; i++) {
	int step_i = STEP0*i;
	int width_i = IMG_WIDTH*i;
    for (int j=0; j<img.cols; j++) {
	int step_ij = step_i + STEP1*j;
	color = (57*img.data[step_ij] + 274*img.data[step_ij +1] + 150*img.data[step_ij + 2]) << 1;
	color = color/1000;
      img_gray_out.data[width_i + j] = color;
    }
  }
}

*/





void grayScale(Mat& img, Mat& img_gray_out)
{
  //Image Size 640x480
  uint8x16x3_t intlv_rgb; 
  uint8_t *grey_ptr = img_gray_out.data;
  unsigned char * ptr = img.data;
 	for (int i=0; i<19200; i++) {
	 	intlv_rgb = vld3q_u8(ptr); // De-interleaving 
		// Loading values
	        uint8x16_t r = intlv_rgb.val[0];	
	        uint8x16_t g = intlv_rgb.val[1];
	        uint8x16_t b = intlv_rgb.val[2];
		// Applying weight	
		r = vshrq_n_u8(r,3); //2^-3 = 0.125		
		g = vshrq_n_u8(g,1); //2^-1 = 0.5		
		b = vshrq_n_u8(b,2); //2^-2 = 0.25		
		// Sum of modified RGB
		uint8x16_t sum = vaddq_u8(r,g);
		sum = vaddq_u8(sum,b);
		// Storing Greyscale
		vst1q_u8(grey_ptr, sum);
		ptr = ptr + 48;
		grey_ptr = grey_ptr + 16;
	}
}

/*******************************************
 * Model: sobelCalc
 * Input: Mat img_in
 * Output: None directly. Modifies a ref parameter img_sobel_out
 * Desc: This module performs a sobel calculation on an image. It first
 *  converts the image to grayscale, calculates the gradient in the x
 *  direction, calculates the gradient in the y direction and sum it with Gx
 *  to finish the Sobel calculation
 ********************************************/
void sobelCalc(Mat& img_gray, Mat& img_sobel_out)
{
  // Apply Sobel filter to black & white image
  signed short sobel_x;
  unsigned short sobel_y;
  for (int i=1; i<img_gray.rows; i++) {
	int width_i = IMG_WIDTH*i;
	int width_iplus = width_i + IMG_WIDTH;
	int width_iminus = width_i - IMG_WIDTH;
    for (int j=1; j<img_gray.cols; j++) {
	unsigned char width_iplusj1 = img_gray.data[width_iplus  + (j+1)];
	unsigned char width_iminusjm1= img_gray.data[width_iminus + (j-1)] ;
  	unsigned char width_iminusjp1 = img_gray.data[width_iminus + (j+1)];
	unsigned char width_iplusjm1 = img_gray.data[width_iplus + (j-1)];// Calculate the x convolution
      sobel_x = (width_iminusjm1  -  width_iplusjm1  +  ((img_gray.data[width_iminus + (j)]) << 1) -
		  ((img_gray.data[width_iplus + (j)]) << 1) +
		  width_iminusjp1 -
		  width_iplusj1) ;
     sobel_x = sobel_x > 0 ? sobel_x : sobel_x*-1;	

  // Calc the y convolution
       sobel_y = abs(width_iminusjm1 -
		   width_iminusjp1 +
		   ((img_gray.data[width_i + (j-1)]) << 1)  -
		   ((img_gray.data[width_i + (j+1)]) << 1) +
		   width_iplusjm1  -
		   width_iplusj1);
  // Combine the two convolutions into the output image
      sobel_x = sobel_x + sobel_y;
      img_sobel_out.data[width_i + j] = (sobel_x > 255) ? 255 : sobel_x;
    }
  }
}
