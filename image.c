/*
Contains all functions used to revert an image.
*/
#include <stdlib.h>
#include <math.h>
#include "include/image.h"
#include "include/ridio.h"


void convert_rbg(RGBPIXEL*** pixels, IMAGESIZE* size) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            // Another method of swapping color modes by swapping their pointer reference and using temporary variable
            char placeholder = pixels[y][x]->blue;
            pixels[y][x]->blue = pixels[y][x]->green;
            pixels[y][x]->green = pixels[y][x]->blue;
        }
    }
}

void convert_grb(RGBPIXEL*** pixels, IMAGESIZE* size) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            // Simple trick to swap two variable values by using xor
            pixels[y][x]->red ^= pixels[y][x]->green ^= pixels[y][x]->red ^= pixels[y][x]->green;
        }
    }
}

void convert_gbr(RGBPIXEL*** pixels,  IMAGESIZE* size) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            pixels[y][x]->red ^= pixels[y][x]->green ^= pixels[y][x]->red ^= pixels[y][x]->green;
            pixels[y][x]->green ^= pixels[y][x]->blue ^= pixels[y][x]->green ^= pixels[y][x]->blue;
        }
    }
}

void convert_brg(RGBPIXEL*** pixels,  IMAGESIZE* size) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            pixels[y][x]->red ^= pixels[y][x]->blue ^= pixels[y][x]->red ^= pixels[y][x]->blue;
            pixels[y][x]->green ^= pixels[y][x]->blue ^= pixels[y][x]->green ^= pixels[y][x]->blue;
        }
    }
}

void convert_bgr(RGBPIXEL*** pixels,  IMAGESIZE* size) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            pixels[y][x]->red ^= pixels[y][x]->blue ^= pixels[y][x]->red ^= pixels[y][x]->blue;
        }
    }
}

void lighten_img(RGBPIXEL*** pixels,  IMAGESIZE* size, float ratio) {
    // *Note - values that are bigger than 255 are set to 255 for RGB standard purposes
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            if(pixels[y][x]->red*ratio > 255) {
                pixels[y][x]->red = 255;
            } else {
                pixels[y][x]->red = round(pixels[y][x]->red*ratio);
            }
            if(pixels[y][x]->green*ratio > 255) {
                pixels[y][x]->green = 255;
            } else {
                pixels[y][x]->green = round(pixels[y][x]->green*ratio);
            }
            if(pixels[y][x]->blue*ratio > 255) {
                pixels[y][x]->blue = 255;
            } else {
                pixels[y][x]->blue = round(pixels[y][x]->blue*ratio);
            }
        }
    }
}

void darken_img(RGBPIXEL*** pixels,  IMAGESIZE* size, float ratio) {
    for(int y = 0; y < size->height; y++) {
        for(int x = 0; x < size->width; x++) {
            pixels[y][x]->red = round(pixels[y][x]->red/ratio);
            pixels[y][x]->green = round(pixels[y][x]->green/ratio);
            pixels[y][x]->blue = round(pixels[y][x]->blue/ratio);
        }
    }
}