#pragma once
#ifndef IMAGE_H
#define IMAGE_H

/* Stores image size(width and height in pixels) */
typedef struct {
    unsigned int width;
    unsigned int height;
} IMAGESIZE; 

/* Stores pixel rgb value in a single struct */
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} RGBPIXEL;

/* Stores pixel rgb value in a single struct but larger than 255(as typical rgb value does)*/
typedef struct {
    double red;
    double green;
    double blue;
} LRGBPIXEL;

/* Stores rid format object */
typedef struct {
    IMAGESIZE* size;
    RGBPIXEL*** pixels;
} RID;

void convert_rbg(RGBPIXEL*** pixels, IMAGESIZE* size);
void convert_grb(RGBPIXEL*** pixels, IMAGESIZE* size);
void convert_gbr(RGBPIXEL*** pixels, IMAGESIZE* size);
void convert_brg(RGBPIXEL*** pixels, IMAGESIZE* size);
void convert_bgr(RGBPIXEL*** pixels, IMAGESIZE* size);
void lighten_img(RGBPIXEL*** pixels,  IMAGESIZE* size, float ratio);
void darken_img(RGBPIXEL*** pixels,  IMAGESIZE* size, float ratio);
#endif