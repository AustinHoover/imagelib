## imagelib
An image processing library written in C

# Compiling
To compile into a shared library, run:
```
make
```
To compile documentation, run:
```
doxygen
```

#Features
* Read/Write PPM P5 images
* Apply symmetric kernels
* MSF kernels
* Skeletonization
* Branch and endpoint labeling

#Usage

Loading an image
```
Image * myImage = readPPM("path/to/file.ppm");
```

Writing an image
```
writePPM("path/to/write/to.ppm",myImage);
```

Applying kernels
```
Image * result = applySlidingKernelToImage(inputImage, VERTICAL_KERNEL);
```

Applying a MSF kernel
```
Image * template = zeroMeanCenterImage(myImage);
Image * result = applyMSF(myOtherImage,template);
Image * output = normalize(result);
```

Skeletonization
```
while(diffImages(myImage,myThinnedImage)){
  myThinnedImage = applyThin(myThinnedImage);
}
```

#To-Do
* Abstract backing datatypes
* Implement reading other file formats
* Add support for non-grayscale images
* Break Image type into BinaryImage, RGBImage, etc
* Thoroughly comment implementing code
* Convert to more c-like syntax

#Analysis of major design tradeoffs
* The prefered functions for working with single pixels, `safeGetPixel()` and `safeSetPixel()`, have more overhead compared to their unsafe counterparts. While they are less performant do to their design, they are significantly easier to work with.
* Functions that return image objects return a new instance instead of editing the input object directly. This makes working in memory-restrictive scenarios much more difficult, but allows for developers in less strict settings to not have to worry about simple error checking as much.
