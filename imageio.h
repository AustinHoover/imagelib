/**
  @file
  Simple CV Lib
  Image Loading Module

  @author Austin Hoover
  @version 1.0 23/10/2019
*/

#ifndef IMAGEIO_H
#define IMAGEIO_H

/**
  @brief Reads in a PPM P5 format image file
  @param Path The path to the file
  @return An image object containing the image data and metadata
*/
Image * readPPM(char * path);

/**
  @brief Writes an image object to disk as a PPM P5 format image file
  @param Path The path the image should be written to
  @param Image The image object
  @return 0 if the file was written, 1 if there was an error opening the file
*/
int writePPM(char * path, Image * img);

/**
  @brief Converts an image type
  Converts the data type backing the image float->char or char->float
  @param Input The image object input
  @param Type The type the output should be
  @return The input in the type specified
*/
Image * convertImageType(Image * input, int type);

/**
  @brief Checks if the position (X,Y) is within bounds of the input image
  @param Input The image to check bounds of
  @param X The x value of the coordinate to check
  @param Y The y value of the coordinate to check
  @return 1 if in bounds, 0 otherwise
*/
char isInBounds(Image * input, int x, int y);

/**
  @brief Gets the value of the pixel at (X,Y) in Input
  @param Input The image to pull from
  @param X The x coordinate to pull from
  @param Y The y coordinate to pull from
  @return The value of the pixel
*/
float getPixel(Image * input, int x, int y);

/**
  @brief Bounds safe variant of getPixel()
  @param Input The image to pull from
  @param X The x coordinate to pull from
  @param Y The y coordinate to pull from
  @return The value of the pixel
 */
float safeGetPixel(Image * input, int x, int y);

/**
  @brief Sets the value of the pixel at (X,Y) in Input to Value
  @param Input The image to change the pixel in
  @param X The x coordinate to set
  @param Y The y coordinate to set
  @param Value The value to set the pixel at (X,Y) to
*/
void setPixel(Image * input, int x, int y, float value);

/**
  @brief Bounds safe variant of setPixel()
  @param Input The image to change the pixel in
  @param X The x coordinate to set
  @param Y The y coordinate to set
  @param Value The value to set the pixel at (X,Y) to
*/
void safeSetPixel(Image * input, int x, int y, float value);

#endif
