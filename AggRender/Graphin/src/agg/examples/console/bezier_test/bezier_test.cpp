#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "agg_rendering_buffer.h"
#include "agg_curves.h"
#include "agg_conv_stroke.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"



void write_bmp_24(const char* filename, 
                  unsigned char* frame_buffer, 
                  unsigned w, 
                  unsigned h,
                  unsigned stride)
{
#pragma pack(1)
    struct
    {
        agg::int16u  bfType; 
        agg::int32u  bfSize; 
        agg::int16u  bfReserved1; 
        agg::int16u  bfReserved2; 
        agg::int32u  bfOffBits;
    } bmf;


    struct
    {
        agg::int32u biSize; 
        agg::int32  biWidth; 
        agg::int32  biHeight; 
        agg::int16u biPlanes; 
        agg::int16u biBitCount;
        agg::int32u biCompression; 
        agg::int32u biSizeImage; 
        agg::int32  biXPelsPerMeter; 
        agg::int32  biYPelsPerMeter; 
        agg::int32u biClrUsed; 
        agg::int32u biClrImportant; 
    } bmh; 
#pragma pack()


    bmf.bfType      = 0x4D42;
    bmf.bfOffBits   = sizeof(bmf) + sizeof(bmh);
    bmf.bfSize      = bmf.bfOffBits + h * stride;
    bmf.bfReserved1 = 0;
    bmf.bfReserved2 = 0;

    bmh.biSize   = sizeof(bmh);
    bmh.biWidth  = w;
    bmh.biHeight = h;
    bmh.biPlanes = 1;
    bmh.biBitCount = 24;
    bmh.biCompression = 0;
    bmh.biSizeImage = h * stride;
    bmh.biXPelsPerMeter = 0;
    bmh.biYPelsPerMeter = 0;
    bmh.biClrUsed = 0;
    bmh.biClrImportant = 0;


    FILE* fd = fopen(filename, "wb");
    if(fd)
    {
        fwrite(&bmf, sizeof(bmf), 1, fd);
        fwrite(&bmh, sizeof(bmh), 1, fd);
        fwrite(frame_buffer, stride, h, fd);
        fclose(fd);
    }
}



int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        printf("usage: bezier_test <width> <height> <number_of_curves>\n");
        return 1;
    }

    unsigned w = atoi(argv[1]);
    unsigned h = atoi(argv[2]);
    unsigned n = atoi(argv[3]);

    if(w < 20 || h < 20)
    {
        printf("Width and hight must be at least 20\n");
        return 1;
    }

    if(w > 4096 || h > 4096)
    {
        printf("Width and hight mustn't exceed 4096\n");
        return 1;
    }

    unsigned stride = (w * 3 + 3) / 4 * 4;

    unsigned char* frame_buffer = new unsigned char[stride * h];

    agg::rendering_buffer rbuf;
    rbuf.attach((unsigned char*)frame_buffer, w, h, stride);

    agg::pixfmt_bgr24 pixf(rbuf);
    agg::renderer_base<agg::pixfmt_bgr24> renb(pixf);
    agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgr24> > ren(renb);

    renb.clear(agg::rgba(1.0, 1.0, 1.0));

    clock_t t1 = clock();
    agg::curve4 curve;
    agg::conv_stroke<agg::curve4> poly(curve);
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 sl;
    unsigned i;
    for(i = 0; i < n; i++)
    {
        poly.width(double(rand() % 3500 + 500) / 500.0);

        curve.init(rand() % w, rand() % h, 
                   rand() % w, rand() % h, 
                   rand() % w, rand() % h, 
                   rand() % w, rand() % h);

        ren.color(agg::rgba8(rand() & 0xFF, 
                             rand() & 0xFF, 
                             rand() & 0xFF, 
                             rand() & 0xFF));

        ras.add_path(poly, 0);
        agg::render_scanlines(ras, sl, ren);
    }
    clock_t t2 = clock();

    double sec = double(t2 - t1) / CLOCKS_PER_SEC;
    printf("%10.3f sec, %10.3f curves per sec\n", sec, double(n) / sec);

    write_bmp_24("output.bmp", (unsigned char*)frame_buffer, w, h, stride);

    delete [] frame_buffer;
    return 0;
}

