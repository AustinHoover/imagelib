/**
  @file
  Simple CV Lib
  Image Processing Module

  @author Austin Hoover
  @version 1.0 23/10/2019
*/

#ifndef IMGKERNEL_H
#define IMGKERNEL_H

/**
  @brief Deprecated
  Average values in kernel border behavior
  Defines border behavior in kernel functions
*/
#define BORDER_BEHAVIOR_AVRG 1

/**
  @brief Deprecated
  Zero values in kernel border behavior
  Defines border behavior in kernel functions
*/
#define BORDER_BEHAVIOR_ZERO 2

/**
  @brief Deprecated
  Wrap values in kernel border behavior
  Defines border behavior in kernel functions
*/
#define BORDER_BEHAVIOR_WRAP 3

/**
  @brief Apply linear kernel horizontally
*/
#define HORIZONTAL_KERNEL 0

/**
  @brief Apply linear kernel vertically
*/
#define VERTICAL_KERNEL 1


/**
  @brief Use applySlidingKernelToImage() instead
  Applies a guassian blur kernel to an image. Usage of applySlidingKerneToImage() is prefered as it is significantly more performant.
  @param Image The image to apply the gaussian blur to
  @return A new image object that is a blurred version of \p Image
  */
Image * applyKernelToImage(Image * image);
/**
  @brief Applies a sobel filter
  Applies a sobel kernel to an image.
  @param Image The image to apply the gaussian blur to
  @return A new image object that is a sobel-filtered version of \p Image
  */
Image * applySobelToImage(Image * image);
/**
  @brief Use applySlidingKernelToImage() instead
  Applies a guassian blur kernel to an image. Usage of applySlidingKerneToImage() is prefered as it is significantly more performant.
  @param Image The image to apply the gaussian blur to
  @param horizontalOrVertical Flag that defines how the kernel will be applied to the image
  @return A new image object that is a blurred version of \p Image
  */
Image * applyLinearKernelToImage(Image * image, int horizontalOrVertical);
/**
  @brief Applies a gaussian blur
  Applies a guassian blur kernel to an image. For correct use, must be applied once in one orientation and a second time in the other orientation.
  @param Image The image to apply the gaussian blur to
  @param horizontalOrVertical Flag that defines how the kernel will be applied to the image
  @return A new image object that is a blurred version of \p Image
  */
Image * applySlidingKernelToImage(Image * image, int horizontalOrVertical);
/**
  @brief Deprecated
  */
float kernelMean(void * data, int width);
/**
  @brief Deprecated
  */
float linearKernelMean(void * data, int width);
/**
  @brief Lists out differences in images
  Compares two images of float format and outputs the major differences in values between them. Useful in debugging different logical branches that (should) output the same result.
  @param Input1 The first image to compare
  @param Input2 The second image to compare
  */
void compareImages(Image * img1, Image * img2);
/**
  @brief Integer format variant of compareImages()
  */
void compareIntImages(Image * img1, Image * img2);
/**
  @brief Checks if images are different
  Compares two images and outputs based on whether they have identical values or not
  @param Input1 First image to compare
  @param Input2 Second image to compare
  @return 1 if the images are different, 0 otherwise
  */
int diffImages(Image * img1, Image * img2);
/**
  @brief Applies Matched Spacial Filter
  Applies \p Template as a matched spacial filter to \p Input
  @param Input The image to be filtered against
  @param Template The image to be used as a template
  @return The resultant filtered image
  */
Image * applyMSF(Image * input, Image * template);
/**
  @brief Thresholds image
  Sets all pixels less that \p Threshold equal to zero in \p Image
  @param Image The image to threshold
  @param Threshold The value to threshold at
  @return The resultant thresholded image
  */
Image * threshold(Image * input, float threshold);
/**
  @brief Zero Mean Centers image
  Zero Mean Centers the input image
  @param Input The image to zero mean center
  @return The resultant image that has been zero mean centered
  */
Image * zeroMeanCenterImage(Image * input);
/**
  @brief Normalizes the image
  Normalizes \p Input of arbitrary scale to the range 0-255
  @param Input The image to be normalized
  @return The image scaled to normal RGB values
  */
Image * normalizeImage(Image * input);
/**
  @brief Checks if image contains value above threshold
  Checks every pixel in \p Input to see if a single one is above \p threshold
  @param Input The image to check through
  @param Threshold The value to threshold at
  @return 1 if \p Input contains a value above \p threshold , 0 otherwise
  */
char containsAboveThreshold(Image * input, float threshold);
/**
  @brief Crops a subimage
  Gets a subimage from \p Input
  @param Input The image to pull from
  @param xi The x coordinate of the top left corner's position
  @param yi The y coordinate fo the top left corner's position
  @param Width The width of the cropped image
  @param Height The height of the cropped image
  @return The cropped image
  */
Image * getSubImage(Image * input, int xi, int yi, int width, int height);
/**
  @brief Gets binary image
  Returns a binary image. All values above the threshold are 1, all values below are 0
  @param Input The image to binarify
  @param Threshold The threshold to cutoff at
  @return The binarified image
  */
Image * getBinaryImage(Image * input, float threshold);
/**
  @brief Sharpens image
  Applies a sharpening kernel to an image
  @param Input The image to sharpen
  @return The resultant, sharpened image
  */
Image * applySharpenKernelToImage(Image * input);
/**
  @brief Inverts image
  Inverts the colors of every pixel in an image
  @param Input The image to invert
  @return The inverted image
  */
Image * invertImage(Image * input);
/**
  @brief Applies a single iteration of thinning to a binary image
  @param Input The binary image
  @return The thinned binary image
  */
Image * applyThin(Image * input);
/**
  @brief Scales a binary image to 0-255 range
  @param Input The binary image
  @return The properly scaled image
  */
Image * scaleBinaryImage(Image * input);
/**
  @brief Retrieves the number of endpoints in a binary image
  @param Input The binary image
  @return The number of endpoints
  */
int getEndpoints(Image * input);
/**
  @brief Retrieves the number of branchpoints in a binary image
  @param Input The binary image
  @return The number of branchpoints
  */
int getBranchpoints(Image * input);
/**
  @brief Gets edge->non-edge transitions
  Returns a Image object that contains the number of edge->non-edge transitions in each pixel
  @param Input The binary image
  @return The data-containing image object
  */
Image * getTransitionCount(Image * input);
/**
  @brief Attempts to remove rogue branches
  @param Input A binary image
  @param Counts The getEndpoints() of the input image
  @return The input image with less rogue branchpoints
  */
Image * deleteRogueBranches(Image * input, Image * counts);
/**
  Structure that contains the location of a single active contour point
  */
typedef struct active_contour_point {
  int x;
  int y;
  struct active_contour_point * next;
} active_contour_point;
/**
  @brief Steps the active contour points on the image
  @param Input
  @param pt_list
  @return
  */
active_contour_point * step_active_contour(Image * input, active_contour_point * pt_list);
/**
  @brief Label contour points
  Labels the input image with the points in the contour point list
  @param Input The image to label
  @param pt_list The list of active contour points to label the input with
  @return The input image labeled
  */
Image * label_active_contour(Image * input, active_contour_point * pt_list);
#endif
