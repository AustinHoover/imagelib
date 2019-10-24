#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../pixel.h"
#include "../imageio.h"

Image * readPPM(char * path){
  FILE *file;
  int ROWS,COLS,BYTES;
  char header[80];
  Image * rVal = NULL;
  
  file = fopen(path,"rb");
  if(file == NULL){
    printf("Failed to open %s!\n",path);
    exit(0);
  }
  
  int i = fscanf(file, "%s %d %d %d", header, &COLS, &ROWS, &BYTES);
  char space;
  fread(&space,1,1,file);
  if(i == 4 && strcmp(header,"P5") == 0 && BYTES == 255){
    rVal = (Image *)calloc(1,sizeof(Image));
    if(rVal != NULL){
      rVal->width = COLS;
      rVal->height = ROWS;
      rVal->type = 1;
      rVal->pixels = (void *)calloc(ROWS * COLS,sizeof(unsigned char));
      void * pixelData = rVal->pixels;
      fread(pixelData,1,ROWS*COLS,file);
      fclose(file);
    } else {
      printf("Failed to allocate space for image!\n");
      fclose(file);
      exit(0);
    }
  } else {
    printf("Failed to read header!\n");
    fclose(file);
    exit(0);
  }
  rVal = convertImageType(rVal, IMAGE_TYPE_FLOAT);
  return rVal;
}

int writePPM(char * path, Image * img){
  Image * output = convertImageType(img,IMAGE_TYPE_CHAR);
  int rVal = 0;
  FILE *file;
  int ROWS = output->height;
  int COLS = output->width;
  int type = output->type;
  file = fopen(path,"w");
  if(file == NULL){
    printf("Unable to open file for writing!\n");
    rVal = 1;
  } else {
    fprintf(file,"P5 %d %d 255\n",COLS,ROWS);
    fwrite(output->pixels,1,ROWS*COLS,file);
    fclose(file);
  }
  return rVal;
}

Image * convertImageType(Image * input, int type){
  Image * rVal = malloc(sizeof(Image));
  rVal->height = input->height;
  rVal->width = input->width;
  rVal->type = type;
  int newDataSize = sizeof(unsigned char);
  if(type == IMAGE_TYPE_CHAR){
  } else if(type == IMAGE_TYPE_FLOAT){
    newDataSize = sizeof(float);
  }
  rVal->pixels = calloc(rVal->height * rVal->width,newDataSize);
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->height; x++){
    for(int y = 0; y < rVal->width; y++){
      float value = 0;
      if(input->type == IMAGE_TYPE_CHAR){
        value = ((unsigned char *)(input->pixels))[x*rVal->width+y];
      } else if(input->type == IMAGE_TYPE_FLOAT){
        value = ((float *)(input->pixels))[x*rVal->width+y];
      }
      if(type == IMAGE_TYPE_CHAR){
        if(round(value) != floor(value)){
          value = round(value);
        } else {
          value = floor(value);
        }
        if(value < 256){
          ((unsigned char *)(rVal->pixels))[x*rVal->width+y] = value;
        } else {
          ((unsigned char *)(rVal->pixels))[x*rVal->width+y] = 255;
        }
      } else if(type == IMAGE_TYPE_FLOAT){
        ((float *)(rVal->pixels))[x*rVal->width+y] = value;
      }
    }
  }
  return rVal;
}

char isInBounds(Image * input, int x, int y){
  if(x >= 0 && x < input->width && y >= 0 && y < input->height){
    return 1;
  } else {
    return 0;
  }
}

float getPixel(Image * input, int x, int y){
  return ((float *)input->pixels)[y*input->width+x];
}

float safeGetPixel(Image * input, int x, int y){
  if(isInBounds(input, x, y)){
    return getPixel(input, x, y);
  } else {
    return 0;
  }
}

void setPixel(Image * input, int x, int y, float value){
  ((float *)input->pixels)[y*input->width+x] = value;
}

void safeSetPixel(Image * input, int x, int y, float value){
  if(isInBounds(input, x, y)){
    ((float *)input->pixels)[y*input->width+x] = value;
  }
}
