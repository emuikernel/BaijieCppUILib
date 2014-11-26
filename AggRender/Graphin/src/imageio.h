#ifndef __imageio_h__
#define __imageio_h__

#include "graphin.h"


/* image ctor, gets width, height of the image and returning array of pointers to rows */
typedef bool ImageCtor(void* pctorPrm, unsigned int width, unsigned int height, unsigned char** rowPtrs);

extern bool DecodeImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength);

bool EncodeImage(image_write_function* pfn, void* fnPrm, 
                 unsigned width, unsigned height, 
                 unsigned char** rows, unsigned bpp, unsigned compression, 
                 unsigned type); // 0 - png, 1, - jpg
#endif
