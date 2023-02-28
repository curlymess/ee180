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
void grayScale(Mat& img, Mat& img_gray_out)
{
  //Image Size 640x480
  uint8x16x3_t intlv_rgb; 
  uint8_t *grey_ptr = img_gray_out.data;
  unsigned char * ptr = img.data;
  float i_end = (img_gray_out.rows*img_gray_out.cols) >> 4;

  for (int i=0; i < i_end; i++) {
	intlv_rgb = vld3q_u8(ptr); // De-interleaving 
	// Loading values
	 uint8x16_t r = intlv_rgb.val[0];	
	 uint8x16_t g = intlv_rgb.val[1];
	 uint8x16_t b = intlv_rgb.val[2];
	// Applying weight	
	r  = vshrq_n_u8(r,3); //2^-3 = 0.125		
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
  unsigned short sobel;

  // Calculate both convolutions
  for (int i=1; i<img_gray.rows-1; i++) {
    for (int j=1; j<img_gray.cols; j++) {
      sobel = abs((
      img_gray.data[IMG_WIDTH*(i-1) + (j-1)] +
      img_gray.data[IMG_WIDTH*(i-1) + (j)] -

      img_gray.data[IMG_WIDTH*(i+1) + (j)] -
      img_gray.data[IMG_WIDTH*(i+1) + (j+1)] +

      img_gray.data[IMG_WIDTH*(i) + (j-1)] -
      img_gray.data[IMG_WIDTH*(i) + (j+1)]) << 1 );
      
      sobel = (sobel > 255) ? 255 : sobel;
      img_sobel_out.data[IMG_WIDTH*(i) + (j)] = sobel;
    }
  }
}
