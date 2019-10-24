/**
  @file
  Simple CV Lib
  Image Comparison Module

  @author Austin Hoover
  @version 1.0 23/10/2019
*/


#ifndef IMAGECOMPARE_H
#define IMAGECOMPARE_H

/**
  Generates an image that contains the delta of two images
  @param Input1 First image object
  @param Input2 Second image object
  @return An image object whose values comprised the absolute deltas between each corresponding input image.
*/
Image * generateComparisonImage(Image * input1, Image * input2);

#endif
