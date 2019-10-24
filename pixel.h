/**
  @file
  Simple CV Lib
  Pixel Header

  @author Austin Hoover
  @version 1.0 23/10/2019
*/

#ifndef PIXEL_H
#define PIXEL_H


/**
  @brief Constant defining an image object backed by chars
*/
#define IMAGE_TYPE_CHAR 1
/**
  @brief Constant defining an image object backed by floats
*/
#define IMAGE_TYPE_FLOAT 2

/**
  @brief Image-containing structure
  Stored metadata about a set of image data.
*/
typedef struct Image {
  void * pixels;
  int height;
  int width;
  char type;
} Image;

#endif
