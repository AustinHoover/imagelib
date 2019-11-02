#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../pixel.h"
#include "../imageio.h"
#include "../imagecompare.h"
#include "../imagekernel.h"

struct timespec time_start,time_end;

Image * applyKernelToImage(Image * image){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = image->width;
  rVal->height = image->height;
  rVal->type = image->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
    int x = 0;
    int y = 0;
    printf("Timing 2D kernel\n");
    clock_gettime(CLOCK_REALTIME,&time_start);
    printf("%ld %ld\n",(long int)time_start.tv_sec,time_start.tv_nsec);
    for(x = 0; x < image->width; x++){
      for(y = 0; y < image->height; y++){
        int iMin = x - 3;
        int iMax = x + 3;
        int jMin = y - 3;
        int jMax = y + 3;
        int i = 0;
        int j = 0;
        int increment = 0;
        float endVal = 0;
        for(i = iMin; i <= iMax; i++){
          for(j = jMin; j <= jMax; j++){
            if(i > -1 && i < image->width && j > -1 && j < image->height){
              endVal = endVal + safeGetPixel(image,i,j);
            }
          }
        }
        endVal = endVal / 49;
        ((float *)(rVal->pixels))[y*rVal->width+x] = endVal;
      }
    }
    clock_gettime(CLOCK_REALTIME,&time_end);
    printf("%ld %ld\n\n",(long int)time_end.tv_sec,time_end.tv_nsec);
    printf("delta: %ld %ld\n",(long int)(time_end.tv_sec - time_start.tv_sec),time_end.tv_nsec - time_start.tv_nsec);
  return rVal;
}


Image * applySobelToImage(Image * image){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = image->width;
  rVal->height = image->height;
  rVal->type = image->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
    int x = 0;
    int y = 0;
    for(x = 0; x < image->width; x++){
      for(y = 0; y < image->height; y++){
        float endVal = 0;
        float sobelX = 0;
        float sobelY = 0;
        //y-major sobel
        sobelY = sobelY + safeGetPixel(image,x-1,y-1) * -1;
        sobelY = sobelY + safeGetPixel(image,x-0,y-1) * -2;
        sobelY = sobelY + safeGetPixel(image,x+1,y-1) * -1;
        sobelY = sobelY + safeGetPixel(image,x-1,y+1) * 1;
        sobelY = sobelY + safeGetPixel(image,x-0,y+1) * 2;
        sobelY = sobelY + safeGetPixel(image,x+1,y+1) * 1;
        //x-major sobel
        sobelX = sobelX + safeGetPixel(image,x-1,y-1) * -1;
        sobelX = sobelX + safeGetPixel(image,x-1,y-0) * -2;
        sobelX = sobelX + safeGetPixel(image,x-1,y+1) * -1;
        sobelX = sobelX + safeGetPixel(image,x+1,y-1) * 1;
        sobelX = sobelX + safeGetPixel(image,x+1,y+0) * 2;
        sobelX = sobelX + safeGetPixel(image,x+1,y+1) * 1;
        endVal = sqrt(pow(sobelX,2) + pow(sobelY,2));
        safeSetPixel(rVal,x,y,endVal);
        //((float *)(rVal->pixels))[y*rVal->width+x] = endVal;
      }
    }
  return rVal;
}


Image * applyLinearKernelToImage(Image * image, int horizontalOrVertical){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = image->width;
  rVal->height = image->height;
  rVal->type = image->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  printf("Timing linear kernel\n");
  clock_gettime(CLOCK_REALTIME,&time_start);
  printf("%ld %ld\n",(long int)time_start.tv_sec,time_start.tv_nsec);
  for(x = 0; x < image->width; x++){
    for(y = 0; y < image->height; y++){
      int iMin = x - 3;
      int iMax = x + 3;
      int jMin = y - 3;
      int jMax = y + 3;
      int i = 0;
      int j = 0;
      float endVal = 0;
      if(horizontalOrVertical == VERTICAL_KERNEL){
        for(i = iMin; i <= iMax; i++){
          if(i > -1 && i < image->width){
            endVal = endVal + safeGetPixel(image,i,y);
          }
        }
      } else {
        for(j = jMin; j <= jMax; j++){
          if(j > -1 && j < image->height){
            endVal = endVal + safeGetPixel(image,x,j);
          }
        }
      }
      endVal = endVal / 7.0;
      safeSetPixel(rVal,x,y,endVal);
    }
  }
  clock_gettime(CLOCK_REALTIME,&time_end);
  printf("%ld %ld\n",(long int)time_end.tv_sec,time_end.tv_nsec);
  printf("delta: %ld %ld\n",(long int)(time_end.tv_sec - time_start.tv_sec),time_end.tv_nsec - time_start.tv_nsec);
  return rVal;
}

Image * applySlidingKernelToImage(Image * image, int horizontalOrVertical){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = image->width;
  rVal->height = image->height;
  rVal->type = image->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  float endVal;
  float total;
  printf("Timing sliding kernel\n");
  clock_gettime(CLOCK_REALTIME,&time_start);
  printf("%ld %ld\n",(long int)time_start.tv_sec,time_start.tv_nsec);
  if(horizontalOrVertical == VERTICAL_KERNEL){
    for(x = 0; x < image->width; x++){
      endVal = (float)0;
      endVal = endVal + safeGetPixel(image,x,0);
      endVal = endVal + safeGetPixel(image,x,1);
      endVal = endVal + safeGetPixel(image,x,2);
      for(y = 0; y < image->height; y++){
        if(y >= 4){
          endVal = endVal - safeGetPixel(image,x,y-4);
        }
        if(y < image->height - 3){
          endVal = endVal + safeGetPixel(image,x,y+3);
        }
        total = endVal / 7.0;
        safeSetPixel(rVal,x,y,total);
      }
    }
  } else {
    for(y = 0; y < image->height; y++){
      endVal = (float)0;
      endVal = endVal + safeGetPixel(image,0,y);
      endVal = endVal + safeGetPixel(image,1,y);
      endVal = endVal + safeGetPixel(image,2,y);
      for(x = 0; x < image->width; x++){
        if(x >= 4){
          endVal = endVal - safeGetPixel(image,x-4,y);
        }
        if(x < image->width - 3){
          endVal = endVal + safeGetPixel(image,x+3,y);
        }


        total = endVal / 7.0;
        safeSetPixel(rVal,x,y,endVal / 7.0);
      }
    }
  }
  clock_gettime(CLOCK_REALTIME,&time_end);
  printf("%ld %ld\n",(long int)time_end.tv_sec,time_end.tv_nsec);
  printf("delta: %ld %ld\n",(long int)(time_end.tv_sec - time_start.tv_sec),time_end.tv_nsec - time_start.tv_nsec);
  return rVal;
}

float kernelMean(void * data, int width){
  int x = 0;
  int y = 0;
  float rVal = 0;
  for(x = 0; x < width; x++){
    for(y = 0; y < width; y++){
      rVal = rVal + (((float *)data)[x*width+y]);
    }
  }
  rVal = rVal / (width * width);
  return rVal;
}

float linearKernelMean(void * data, int width){
  int x = 0;
  int y = 0;
  float rVal = 0;
  for(x = 0; x < width; x++){
    rVal = rVal + (((float *)data)[x]);
  }
  rVal = rVal / width;
  return rVal;
}

void compareImages(Image * img1, Image * img2){
  int x = 0;
  int y = 0;
  int numMismatch = 0;
  for(x = 0; x < img1->width; x++){
    for(y = 0; y < img1->height; y++){
      float val1 = safeGetPixel(img1,x,y);
      float val2 = safeGetPixel(img2,x,y);
      if((float)abs(val1 - val2) > (float)0.00001){
        printf("Big mismatch: %f vs %f at %d,%d\n",val1,val2,x,y);
        numMismatch++;
      }
    }
  }
  printf("Number mismatches: %d\n",numMismatch);
}



void compareIntImages(Image * img1, Image * img2){
  int x = 0;
  int y = 0;
  int numMismatch = 0;
  for(x = 0; x < img1->width; x++){
    for(y = 0; y < img1->height; y++){
      unsigned char val1 = ((unsigned char *)(img1->pixels))[y*img1->width+x];
      unsigned char val2 = ((unsigned char *)(img2->pixels))[y*img1->width+x];
      if(val1 != val2){
        printf("Mismatch: %d vs %d at %d,%d\n",val1,val2,x,y);
        numMismatch++;
      }
    }
  }
  printf("Number mismatches: %d\n",numMismatch);
}


int diffImages(Image * img1, Image * img2){
  int x = 0;
  int y = 0;
  for(x = 0; x < img1->width; x++){
    for(y = 0; y < img1->height; y++){
      if(safeGetPixel(img1,x,y)!=safeGetPixel(img2,x,y)){
        return 1;
      }
    }
  }
  return 0;
}


Image * applyMSF(Image * input, Image * template){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  int templateWidth = template->width;
  int templateHeight = template->height;
  int halfWidth = templateWidth/2;
  int halfHeight = templateHeight/2;
  float sum;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      sum = 0;
      int i = 0;
      int j = 0;
      for(i = 0; i < template->width; i++){
        for(j = 0; j < template->height; j++){
          int inputCoordX = x - (template->width/2) + i;
          int inputCoordY = y - (template->height/2) + j;
          sum += safeGetPixel(template,i,j) * safeGetPixel(input,inputCoordX,inputCoordY);
        }
      }
      if(sum < 0){
        sum = 0;
      }
      safeSetPixel(rVal,x,y,sum);
    }
  }
  return rVal;
}


Image * threshold(Image * input, float threshold){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      if(value < threshold){
        value = 0;
      }
      safeSetPixel(rVal,x,y,value);
    }
  }
  return rVal;
}


Image * zeroMeanCenterImage(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  double sum = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      sum = sum + value;
    }
  }
  double mean = sum / (rVal->width * rVal->height);
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      value = value - mean;
      safeSetPixel(rVal,x,y,value);
    }
  }
  return rVal;
}

Image * normalizeImage(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  float max = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      if(value > max){
        max = value;
      }
    }
  }
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      if(max > 255){
        value = (value / max) * 255.0;
      }
      safeSetPixel(rVal,x,y,value);
    }
  }
  return rVal;
}


char containsAboveThreshold(Image * input, float threshold){
  char rVal = 0;
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float value = safeGetPixel(input,x,y);
      if(value > threshold){
        rVal = 1;
      }
    }
  }
  return rVal;
}

Image * getSubImage(Image * input, int xi, int yi, int width, int height){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = width;
  rVal->height = height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      int inputX = xi+x;
      int inputY = yi+y;
      float value = 0;
      value = safeGetPixel(input,inputX,inputY);
      safeSetPixel(rVal,x,y,value);
    }
  }
  return rVal;
}


Image * getBinaryImage(Image * input, float threshold){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      if(safeGetPixel(input,x,y) < threshold){
        safeSetPixel(rVal,x,y,0);
      } else {
        safeSetPixel(rVal,x,y,1);
      }
    }
  }
  return rVal;
}



Image * applySharpenKernelToImage(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      float endVal = 0;
      endVal = endVal + 5 * safeGetPixel(input,x,y);
      endVal = endVal - safeGetPixel(input,x-1,y  );
      endVal = endVal - safeGetPixel(input,x+1,y  );
      endVal = endVal - safeGetPixel(input,x  ,y-1);
      endVal = endVal - safeGetPixel(input,x  ,y+1);
      if(endVal < 0){
        endVal = 0;
      }
      if(endVal > 255){
        endVal = 255;
     }
      safeSetPixel(rVal,x,y,endVal);
    }
  }
  return rVal;
}

Image * invertImage(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      safeSetPixel(rVal,x,y,255-safeGetPixel(input,x,y));
    }
  }
  return rVal;
}


Image * applyThin(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      safeSetPixel(rVal,x,y,safeGetPixel(input,x,y));
      if(safeGetPixel(input,x,y)==1){
        //GetNeighbors & transition count
        int neighborCount = 0;
        int transitionCount = 0;
        if(safeGetPixel(input,x-1,y-1)==1){
          neighborCount++;
          if(safeGetPixel(input,x,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-0,y-1)==1){
          neighborCount++;
          if(safeGetPixel(input,x+1,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y-1)==1){
          neighborCount++;
          if(safeGetPixel(input,x+1,y)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-1,y-0)==1){
          neighborCount++;
          if(safeGetPixel(input,x-1,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y-0)==1){
          neighborCount++;
          if(safeGetPixel(input,x+1,y+1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-1,y+1)==1){
          neighborCount++;
          if(safeGetPixel(input,x-1,y)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-0,y+1)==1){
          neighborCount++;
          if(safeGetPixel(input,x-1,y+1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y+1)==1){
          neighborCount++;
          if(safeGetPixel(input,x,y+1)==0){
            transitionCount++;
          }
        }
        int passesTest = 0;
        if(safeGetPixel(input,x,y-1)==0){
          passesTest = 1;
        }
        if(safeGetPixel(input,x+1,y)==0){
          passesTest = 1;
        }
        if(safeGetPixel(input,x-1,y)==0 && safeGetPixel(input,x,y+1)==0){
          passesTest = 1;
        }
        if(x == 8){
          printf("nC %d tC %d\n",neighborCount,transitionCount);
        }
        if(passesTest && transitionCount == 1 && neighborCount >=2 && neighborCount <= 6){
          safeSetPixel(rVal,x,y,0);
        }
      }
    }
  }
  return rVal;
}


Image * deleteRogueBranches(Image * input, Image * counts){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x< rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      safeSetPixel(rVal,x,y,safeGetPixel(input,x,y));
    }
  }
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      if(safeGetPixel(counts,x,y)>2){
        if(safeGetPixel(counts,x-1,y-1)==1){
            safeSetPixel(rVal,x-1,y-1,0);
        }
        if(safeGetPixel(counts,x,y-1)==1){
            safeSetPixel(rVal,x,y-1,0);
        }
        if(safeGetPixel(counts,x+1,y-1)==1){
            safeSetPixel(rVal,x+1,y-1,0);
        }
        if(safeGetPixel(counts,x+1,y)==1){
            safeSetPixel(rVal,x+1,y,0);
        }
        if(safeGetPixel(counts,x+1,y+1)==1){
            safeSetPixel(rVal,x+1,y+1,0);
        }
        if(safeGetPixel(counts,x,y+1)==1){
            safeSetPixel(rVal,x,y+1,0);
        }
        if(safeGetPixel(counts,x-1,y+1)==1){
            safeSetPixel(rVal,x-1,y+1,0);
        }
        if(safeGetPixel(counts,x-1,y)==1){
            safeSetPixel(rVal,x-1,y,0);
        }
      }
    }
  }
  return rVal;
}


Image * getTransitionCount(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      safeSetPixel(rVal,x,y,safeGetPixel(input,x,y));
      if(safeGetPixel(input,x,y)==1){
        //GetNeighbors & transition count
        int transitionCount = 0;
        if(safeGetPixel(input,x-1,y-1)==1){
          if(safeGetPixel(input,x,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-0,y-1)==1){
          if(safeGetPixel(input,x+1,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y-1)==1){
          if(safeGetPixel(input,x+1,y)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-1,y-0)==1){
          if(safeGetPixel(input,x-1,y-1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y-0)==1){
          if(safeGetPixel(input,x+1,y+1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-1,y+1)==1){
          if(safeGetPixel(input,x-1,y)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x-0,y+1)==1){
          if(safeGetPixel(input,x-1,y+1)==0){
            transitionCount++;
          }
        }
        if(safeGetPixel(input,x+1,y+1)==1){
          if(safeGetPixel(input,x,y+1)==0){
            transitionCount++;
          }
        }
        safeSetPixel(rVal,x,y,transitionCount);
      }
    }
  }
  return rVal;
}

int getEndpoints(Image * input){
  int rVal = 0;
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      if(safeGetPixel(input,x,y)==1){
        rVal++;
      }
    }
  }
  return rVal;
}

int getBranchpoints(Image * input){
  int rVal = 0;
  int x = 0;
  int y = 0;
  for(x = 0; x < input->width; x++){
    for(y = 0; y < input->height; y++){
      if(safeGetPixel(input,x,y)>2){
        rVal++;
      }
    }
  }
  return rVal;
}



Image * scaleBinaryImage(Image * input){
  Image * rVal = NULL;
  rVal = (Image *)calloc(1,sizeof(Image));
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->type = input->type;
  rVal->pixels = calloc(rVal->width * rVal->height,sizeof(float));
  int x = 0;
  int y = 0;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      if(safeGetPixel(input,x,y)>0){
        safeSetPixel(rVal,x,y,255);
      }
    }
  }
  return rVal;
}




//need to fine-tune the forces at work still. Very WIP!

active_contour_point * step_active_contour(Image * input, active_contour_point * pt_list){
  active_contour_point * rVal = (active_contour_point *)malloc(sizeof(active_contour_point));
  active_contour_point * current = pt_list;
  active_contour_point * next = current->next;
  active_contour_point * rVal_iterator = rVal;
  //calculate centroid
  int x_sum = 0;
  int y_sum = 0;
  int num = 0;
  while(current != NULL){
    x_sum = x_sum + current->x;
    y_sum = y_sum + current->y;
    num = num + 1;
    current = current->next;
  }
  int centroid_x = x_sum / num;
  int centroid_y = y_sum / num;
  //calculate next points
  int value_field_dim = 3;
  int half_dim = value_field_dim/2;
  int x,y;
  int value_field;
  int min_x, min_y, min_val;
  current = pt_list;
  active_contour_point * prev = pt_list;
  current = current->next;
  while(next != NULL){
    //set and search field
    min_val = -1;
    for(x = -half_dim; x < half_dim+1; x++){
      for(y = -half_dim; y < half_dim+1; y++){
        int centroid_x_relative = centroid_x - (current->x + x);
        int centroid_y_relative = centroid_y - (current->y + y);
        int next_x_relative = next->x - (current->x + x);
        int next_y_relative = next->y - (current->y + y);
        int prev_x_relative = prev->x - (current->x + x);
        int prev_y_relative = prev->y - (current->y + y);
        int dist_to_next = sqrt(pow(next_x_relative,2) + pow(next_y_relative,2));
        int dist_to_prev = sqrt(pow(prev_x_relative,2) + pow(prev_y_relative,2));
        //int dist_to_centroid = pow((centroid_x_relative - next_x_relative)-x,2) +
        //    pow((centroid_y_relative - next_y_relative) - y,2);
        int dist_to_centroid = sqrt(pow(centroid_x_relative,2) + pow(centroid_y_relative,2));
        //int dist_to_current = pow(half_dim-x,2) + pow(half_dim-y,2);
        int value_at_current = safeGetPixel(input,current->x + x,current->y + y);
        value_field = pow(dist_to_next,2) + pow(dist_to_prev,2) + pow(dist_to_centroid,2) + pow(value_at_current,2);
        if(min_val == -1){
          min_val = value_field;
          min_x = x;
          min_y = y;
        } else {
          if(value_field < min_val){
            min_val = value_field;
            min_x = x;
            min_y = y;
          }
        }
      }
    }
    rVal_iterator->x = current->x + min_x;
    rVal_iterator->y = current->y + min_y;
    rVal_iterator->next = (active_contour_point *)malloc(sizeof(active_contour_point));
    prev = prev->next;
    current = next;
    next = current->next;
    if(next != NULL){
      rVal_iterator = rVal_iterator->next;
    } else {
      free(rVal_iterator->next);
    }
  }
  //handle the loop-around
  //
  //Where we're at:
  //      old list: [head]  ----      ----    [2nd to last][last]
  //iterating list: [next]                    [prev]       [current]
  //
  rVal_iterator->next = (active_contour_point *)malloc(sizeof(active_contour_point));
  rVal_iterator = rVal_iterator->next;
  rVal_iterator->next = NULL;
  //current = rVal_iterator;
  next = pt_list;
  min_val = -1;
  for(x = -half_dim; x < half_dim+1; x++){
    for(y = -half_dim; y < half_dim+1; y++){
      int centroid_x_relative = centroid_x - (current->x + x);
      int centroid_y_relative = centroid_y - (current->y + y);
      int next_x_relative = next->x - (current->x + x);
      int next_y_relative = next->y - (current->y + y);
      int prev_x_relative = prev->x - (current->x + x);
      int prev_y_relative = prev->y - (current->y + y);
      int dist_to_next = sqrt(pow(next_x_relative,2) + pow(next_y_relative,2));
      int dist_to_prev = sqrt(pow(prev_x_relative,2) + pow(prev_y_relative,2));
      //int dist_to_centroid = pow((centroid_x_relative - next_x_relative)-x,2) +
      //    pow((centroid_y_relative - next_y_relative) - y,2);
      int dist_to_centroid = sqrt(pow(centroid_x_relative,2) + pow(centroid_y_relative,2));
      int value_at_current = safeGetPixel(input,current->x + x,current->y + y);
      value_field = pow(dist_to_next,2) + pow(dist_to_prev,2) + pow(dist_to_centroid,2) + pow(value_at_current,2);
      if(min_val == -1){
        min_val = value_field;
        min_x = x;
        min_y = y;
      } else {
        if(value_field < min_val){
          min_val = value_field;
          min_x = x;
          min_y = y;
        }
      }
    }
  }
  rVal_iterator->x = current->x + min_x;
  rVal_iterator->y = current->y + min_y;
  return rVal;
}

Image * label_active_contour(Image * input, active_contour_point * pt_list){
  //clone input image
  Image * rVal = (Image *)malloc(sizeof(Image));
  rVal->type = input->type;
  rVal->width = input->width;
  rVal->height = input->height;
  rVal->pixels = calloc(input->width * input->height,sizeof(float));
  int x,y;
  for(x = 0; x < rVal->width; x++){
    for(y = 0; y < rVal->height; y++){
      safeSetPixel(rVal,x,y,safeGetPixel(input,x,y));
    }
  }
  //draw contour points
  int i,j;
  int current_x,current_y;
  active_contour_point * current = pt_list;
  while(current->next != NULL){
    current_x = current->x;
    current_y = current->y;
    for(j = -3; j < 4; j++){
      safeSetPixel(rVal,current_x+j,current_y+j,0);
      safeSetPixel(rVal,current_x+j,current_y-j,0);
    }
    current = (active_contour_point *)(current->next);
  }
  return rVal;
}

