/** @file jmemsrc.c 
 *
 *
 * Source a jpeg from memory
 *
 **************************************************************************
 *
 * Source code for Real Time Image Library (libRTImage)
 *
 * Leeds Vision Group give permission for this code to be copied, modified
 * and distributed within the University of Leeds subject to the following
 * conditions:-
 *
 * - The code is not to be used for commercial gain.
 * - The code and use thereof will be attributed to the authors where
 *   appropriate (including demonstrations which rely on it's use).
 * - All modified, distributions of the source files will retain this header.
 *
 *************************************************************************
 * Aphrodite Galata, March 2001
 *
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jinclude.h"
#include "jpeglib.h"
#include "jerror.h"


/* Expanded data source object for stdio input */
typedef struct {
  struct jpeg_source_mgr pub;	/* public fields */

  unsigned char* membuff;	/* The input buffer */
  int location;			/* Current location in buffer */ 
  int membufflength;            /* The length of the input buffer */
  JOCTET * buffer;		/* start of buffer */
  boolean start_of_buff;	/* have we gotten any data yet? */
} mem_source_mgr;

typedef mem_source_mgr* mem_src_ptr;

#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */


METHODDEF(void)
mem_init_source (j_decompress_ptr cinfo)
{
  mem_src_ptr src;

  src = (mem_src_ptr) cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->location = 0;
  src->start_of_buff = TRUE;
}


METHODDEF(boolean)
mem_fill_input_buffer (j_decompress_ptr cinfo)
{
  mem_src_ptr src;
  size_t bytes_to_read;
  size_t nbytes;

  src = (mem_src_ptr) cinfo->src;

  if((src->location)+INPUT_BUF_SIZE >= src->membufflength)
    bytes_to_read = src->membufflength - src->location;
  else
    bytes_to_read = INPUT_BUF_SIZE;

  memcpy(src->buffer, (src->membuff)+(src->location), bytes_to_read);
  nbytes = bytes_to_read;
  src->location += bytes_to_read;

  if (nbytes <= 0) {
    if (src->start_of_buff)	/* Treat empty input file as fatal error */
      ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_buff = FALSE;

  return TRUE;
}


METHODDEF(void)
mem_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  mem_src_ptr src;

  src = (mem_src_ptr) cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) mem_fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


METHODDEF(void)
mem_term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}


GLOBAL(void)
jpeg_mem_src (j_decompress_ptr cinfo, unsigned char *mbuff, int mbufflen)
{
  mem_src_ptr src;

  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  SIZEOF(mem_source_mgr));
    src = (mem_src_ptr) cinfo->src;
    src->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  INPUT_BUF_SIZE * SIZEOF(JOCTET));
  }

  src = (mem_src_ptr) cinfo->src;
  src->pub.init_source = mem_init_source;
  src->pub.fill_input_buffer = mem_fill_input_buffer;
  src->pub.skip_input_data = mem_skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart;
  src->pub.term_source = mem_term_source;
  src->membuff = mbuff;
  src->membufflength = mbufflen;
  src->pub.bytes_in_buffer = 0;    /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}
