#include <stdlib.h>
#include <stdio.h>
#include "../pixel.h"
#include "../imageio.h"

Image * generateComparisonImage(Image * input1, Image * input2){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input1->width;
  rVal->height = input1->height;
  rVal->type = input1->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  float maxVal = 0;
  for(x = 0; x < input1->width;x++){
    for(y = 0; y < input1->height; y++){
      float delta = 0;
      float pix1 = safeGetPixel(input1,x,y);
      float pix2 = safeGetPixel(input2,x,y);
      delta = abs(pix1 - pix2);
      if(delta > maxVal){
        maxVal = delta;
      }
    }
  }
  float amplify = 255/maxVal;
  for(x = 0; x < input1->width; x++){
    for(y = 0;y < input1->height; y++){
      float delta = 0;
      float pix1 = safeGetPixel(input1,x,y);
      float pix2 = safeGetPixel(input2,x,y);
      delta = abs(pix1 - pix2);
      safeSetPixel(rVal,x,y,delta * amplify);
    }
  }
  return rVal;
}


