#include "imageio.h"
#include <malloc.h>

#if defined(__GNUC__)
  #include <alloca.h>
#endif

int DecodePNGImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength);
int DecodeJPGImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength);

bool DecodeImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength)
{
  if( DecodePNGImage(pctor, pctorPrm, src, srclength) )
    return 1;
  if( DecodeJPGImage(pctor, pctorPrm, src, srclength) )
    return 1;
  return 0;
}

typedef struct _imginput
{
  unsigned char* pos;
  unsigned char* end;
} imginput;

#include "png.h"

static void PNGAPI png_read_data(png_structp png_ptr,png_bytep data, png_size_t length)
{
  imginput *pio = (imginput *)png_get_io_ptr(png_ptr);
  if(pio->pos + length > pio->end)
    png_error(png_ptr,"PNG read error");
  memcpy(data,pio->pos,length);
  pio->pos += length;
}

int DecodePNGImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength)
{
    if(!pctor)
      return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if (!png_ptr) // out of memory?
      return 0;
    png_infop	info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) { // out of memory?
      png_destroy_read_struct(&png_ptr,NULL,NULL);
      return 0;
    }
    // setup i/o routine
    imginput ioc;
    ioc.pos = src;
    ioc.end = src + srclength;

    png_set_read_fn(png_ptr,&ioc,png_read_data);

    // init jmpbuf
    if (setjmp(png_jmpbuf(png_ptr))) { // error occurred
      png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
      return 0;
    }
    png_read_info(png_ptr,info_ptr);
    png_set_bgr(png_ptr);
    png_uint_32	width,height;
    int		bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
		 &interlace_type,NULL,NULL);

    // configure transformations, we always want RGB data in the end
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr,PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png_ptr);
    if (bit_depth == 16)
      png_set_strip_16(png_ptr);
    if (bit_depth < 8)
      png_set_packing(png_ptr);

//#ifdef PNG_FLOATING_POINT_SUPPORTED
    //png_color_16  my_background={0,0,0,0,0};
    png_color_16p dib_background;
    if (png_get_bKGD(png_ptr, info_ptr, &dib_background))
      png_set_background(png_ptr, dib_background,
			 PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
    //else
    //  png_set_background(png_ptr, &my_background,
		//	 PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

    double screen_gamma=2.2; // typical value
    double gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &gamma))
      png_set_gamma(png_ptr, screen_gamma, gamma);
    else
      png_set_gamma(png_ptr, screen_gamma, 0.45455);
//#endif


    // update info after applying transformations
    png_read_update_info(png_ptr,info_ptr);

    int		rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    int   bpp = rowBytes / width;

    if( bpp == 3 )
      png_set_add_alpha(png_ptr,0xff,1);
    else
      png_set_invert_alpha(png_ptr);

    png_byte** rowPtrs = (png_byte**)malloc(sizeof(png_byte*)*height);
    rowPtrs[0] = 0;
    //void* retval = 0;
    if(rowPtrs && height && width)
    {
      if(pctor(pctorPrm, width,height,rowPtrs))
         png_read_image(png_ptr, rowPtrs);
    }
    png_read_end(png_ptr,info_ptr);
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);

    if( rowPtrs )
      free(rowPtrs);

    return 1;

}

// JPEG

extern "C"
{
  #include "jpeglib.h"
}

  struct my_error_mgr {
    struct jpeg_error_mgr pub;	/* "public" fields */
    jmp_buf setjmp_buffer;	/* for return to caller */
  };

  typedef struct my_error_mgr * my_error_ptr;

  METHODDEF(void)  __my_error_exit (j_common_ptr cinfo)
  {
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
  }

  int DecodeJPGImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength)
  {

	// load a JPEG compressed image
	// gray scale image will be keeped.
	// return TRUE if successful

    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr __jerr;

    JSAMPROW row_pointer[1];
    int isgray;


    /* Step 1: allocate and initialize JPEG decompression object */
    cinfo.err = jpeg_std_error(&__jerr.pub);
    __jerr.pub.error_exit = __my_error_exit;

    /* Establish the setjmp return context for my_error_exit to use. */

    if (setjmp(__jerr.setjmp_buffer)) {
      /* If we get here, the JPEG code has signaled an error.
       * We need to clean up the JPEG object, close the input file, and return.
       */
      jpeg_destroy_decompress(&cinfo);
      return 0;
    }

    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_mem_src(&cinfo, src, srclength);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void) jpeg_read_header(&cinfo, (unsigned char)TRUE);

    if(cinfo.num_components == 3 && cinfo.out_color_space == JCS_RGB)
    {
       isgray = 0;
    }
    else if(cinfo.num_components == 1 && cinfo.out_color_space == JCS_GRAYSCALE)
    {
       isgray = 1;
    }
    else {
      jpeg_destroy_decompress(&cinfo);
      return 0;
    }

    /* Step 4: set parameters for decompression */

    /* Step 5: Start decompressor */

    (void) jpeg_start_decompress(&cinfo);

	  unsigned char** rowPtrs = (unsigned char**)malloc( sizeof(unsigned char*) * cinfo.image_height);
    rowPtrs[0] = 0;
    //void* retval = 0;
    if(rowPtrs && cinfo.image_height && cinfo.image_width)
    {
      if(!pctor(pctorPrm, cinfo.image_width ,cinfo.image_height,rowPtrs))
         goto FAIL;
    }

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */
    if(isgray) {
       unsigned char* row = (unsigned char*) alloca (cinfo.image_width);
       row_pointer[0] = &row[0];
       while (cinfo.output_scanline < cinfo.output_height)
       {
	     int scan = cinfo.output_scanline;
         (void) jpeg_read_scanlines(&cinfo, row_pointer , 1);
         unsigned char* rgba = rowPtrs[scan];
         for(size_t i = 0; i < cinfo.image_width; i++, rgba += 4)
         {
           rgba[0] = rgba[1] = rgba[2] = row[i];
           rgba[3] = 0xff;
         }
       }
    }
    else {
       unsigned char* row = (unsigned char*) alloca (3 * cinfo.image_width);
       row_pointer[0] = &row[0];
       while (cinfo.output_scanline < cinfo.output_height)
       {
	       int scan = cinfo.output_scanline;
         (void) jpeg_read_scanlines(&cinfo, row_pointer , 1);
         unsigned char* rgba = rowPtrs[scan];
		     unsigned char* src = row;
         for(size_t i = 0; i < cinfo.image_width; i++, src += 3, rgba += 4)
		     {
           rgba[0] = src[2];
		       rgba[1] = src[1];
		       rgba[2] = src[0];
           rgba[3] = 0xff;
		     }
       }
    }

FAIL:
    if( rowPtrs )
      free(rowPtrs);

    /* Step 7: Finish decompression */
    (void) jpeg_finish_decompress(&cinfo);

    /* Step 8: Release JPEG decompression object */
    jpeg_destroy_decompress(&cinfo);

    return 1;
  }


bool EncodePNGImage(image_write_function* pstreamFn, void* streamPrm, unsigned width, unsigned height, unsigned char** rows, unsigned bpp, unsigned compression);
bool EncodeJPGImage(image_write_function* pstreamFn, void* streamPrm, unsigned width, unsigned height, unsigned char** rows, unsigned bpp, unsigned compression);

bool EncodeImage(image_write_function* pfn, void* fnPrm,
                 unsigned width, unsigned height,
                 unsigned char** rows, unsigned bpp, unsigned compression,
                 unsigned type) // 0 - png, 1, - jpg
{
  if( type == 0 )
    return EncodePNGImage(pfn, fnPrm, width, height, rows, bpp, compression);
  else if( type == 1 )
    return EncodeJPGImage(pfn, fnPrm, width, height, rows, bpp, compression);
  return 0;
}

struct write_io
{
  image_write_function* pctl;
  void*           streamPrm;
};

static void PNGAPI png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
   write_io *pio = (write_io *)png_get_io_ptr(png_ptr);
   pio->pctl( pio->streamPrm, data, length );
}

static void PNGAPI png_flush_data(png_structp png_ptr)
{
   ;// does nothing
}

bool EncodePNGImage(image_write_function* pctl, void* streamPrm, unsigned width, unsigned height, unsigned char** rows, unsigned bpp, unsigned compression)
{

  png_structp png = 0;
  png_infop pngInfo = 0;

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (!png)
  {
    return false;
  }

  pngInfo = png_create_info_struct(png);
  if (!pngInfo)
  {
    png_destroy_write_struct(&png, &pngInfo);
  }

	if (setjmp(png->jmpbuf))
	{
    png_destroy_write_struct(&png, &pngInfo);
    return false;
  }

  // setup i/o routine

  write_io pio; pio.pctl = pctl; pio.streamPrm = streamPrm;

  png_set_write_fn(png,&pio,png_write_data,png_flush_data);

  unsigned fmt = bpp == 32? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;

  png_set_IHDR(png, pngInfo, width, height, 8, fmt,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png, pngInfo);
  png_set_bgr(png);
  //png_write_png (png, pngInfo, PNG_TRANSFORM_BGR, NULL);

  png_write_image(png, rows);

  png_write_end(png, pngInfo);
  //png_write_flush(png);

  png_destroy_write_struct(&png, &pngInfo);

  return true;
}

// JPG store

typedef struct my_jpeg_out_mgr {
  struct jpeg_destination_mgr pub;
  unsigned char buffer[256];
  image_write_function* pctl;
  void*           streamPrm;
} my_jpeg_out_mgr;

inline void rgba2bgr(unsigned char* rgba, unsigned char* bgr, unsigned int num_pixels)
{
  for(unsigned n = 0; n < num_pixels; ++n)
  {
    bgr[0] = rgba[2];
    bgr[1] = rgba[1];
    bgr[2] = rgba[0];
    rgba += 4;
    bgr += 3;
  }
}

/* this routine is called to initiate the jpeg writing process */

  void my_init_destination(
    j_compress_ptr cinfo
  )
  {
    struct my_jpeg_out_mgr *dest = (struct my_jpeg_out_mgr *) cinfo->dest;
    dest->pub.next_output_byte = dest->buffer;
    memset(dest->buffer, 0, 256);
    dest->pub.free_in_buffer = 256;
  }

  /* this routine is called each time the buffer gets full */

  boolean my_empty_output_buffer(
    j_compress_ptr cinfo
  )
  {
    struct my_jpeg_out_mgr *dest = (struct my_jpeg_out_mgr *) cinfo->dest;
    dest->pctl(dest->streamPrm,dest->buffer,256);
    memset(dest->buffer, 0, 256);
    dest->pub.next_output_byte = dest->buffer;
    dest->pub.free_in_buffer = 256;
    return TRUE;
  }

  /* and this routine gets called when the writing process is finished */

  void my_term_destination(
    j_compress_ptr cinfo
  )
  {
    struct my_jpeg_out_mgr *dest = (struct my_jpeg_out_mgr *) cinfo->dest;
    int n = 256 - int(dest->pub.free_in_buffer);
    dest->pctl(dest->streamPrm,dest->buffer,n);
  }

  bool EncodeJPGImage(image_write_function* pctl, void* streamPrm, unsigned width, unsigned height, unsigned char** rows, unsigned bpp, unsigned quality)
  {
    unsigned char* rowbuf = (unsigned char*)alloca(  width * 3 );
    if(!rowbuf) return false;

    struct my_jpeg_out_mgr dest;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    cinfo.dest = (struct jpeg_destination_mgr *) &dest;

    dest.pctl = pctl;
    dest.streamPrm = streamPrm;

    dest.pub.init_destination = my_init_destination;
    dest.pub.empty_output_buffer = my_empty_output_buffer;
    dest.pub.term_destination = my_term_destination;

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    while(cinfo.next_scanline < cinfo.image_height)
    {
      rgba2bgr(rows[cinfo.next_scanline], rowbuf, cinfo.image_width);
      row_pointer[0] = rowbuf;
      jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return true;

  }


