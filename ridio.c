/*
Contains all functions related to opening, reading and saving in rid format.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/image.h"

#define MAX_FILENAME_LEN 260 // Windows Api max file name length


/*
* Checks if given filename ends with `.rid`. 
* If it does not end with `.rid` it prints error to the console
* and exits with code -1.
*/
static void check_file_ext(char* filename) {
    char* filename_cp = (char*)malloc(MAX_FILENAME_LEN);
    strcpy(filename_cp, filename);
    char delimiters[] = ".";
    char* last_substring;
    last_substring = strtok(filename_cp, delimiters);
    while(last_substring != NULL) {
        if(strtok(NULL, delimiters) == NULL && !strcmp(last_substring, "rid")) {
            printf("[ERROR:check_file_ext] Specified file must have '.rid' extension!\n");
            getchar();
            exit(-1);
        }
        last_substring = strtok(NULL, delimiters);  
    }
}

/* Opens `.rid` and returns ptr to it.
* If any error occured during opening the file(ptr is set to NULL)
* shows error in the console and exits with -1.
*/ 
FILE* open_rid(char filepath[MAX_FILENAME_LEN]) {
    check_file_ext(filepath);
    FILE* fp = fopen(filepath, "rb");
    if (fp == NULL) {
        printf("[ERROR:open_rid] Error occured while opening '%s' file!", filepath);
        getchar();
        exit(-1);
    }
    return fp;
};

/*
* Loads every rgb pixel value directly from the file ptr and saves it as RGBPIXEL struct in the 2d array.
*/
static RGBPIXEL*** load_pixels(FILE* fp, IMAGESIZE* size) {
    
    RGBPIXEL*** pixels = (RGBPIXEL***)malloc(sizeof(RGBPIXEL**)*size->height);
    for(int y = 0; y < size->height; y++) {
        pixels[y] = (RGBPIXEL**)malloc(sizeof(RGBPIXEL*)*size->width);
        for(int x = 0; x < size->width; x++) {
            RGBPIXEL* pixel = (RGBPIXEL*)malloc(sizeof(RGBPIXEL));
            int offset = sizeof(IMAGESIZE) + y*size->width*sizeof(RGBPIXEL) + x*sizeof(RGBPIXEL); // Count offset from 8th byte as image size is stored there
            fseek(fp, offset, SEEK_SET);
            fread(pixel, sizeof(RGBPIXEL), 1, fp);
            pixels[y][x] = pixel;
        }
    }
    return pixels;
}

/*
* Loads rid object from opened `.rid` file. 
* Rid object stores image size and 2d array with loaded properly loaded pixels.
*/
RID* load_rid(FILE* fp) {
    RID* rid_obj = (RID*)malloc(sizeof(RID));
    IMAGESIZE* image_size = (IMAGESIZE*)malloc(sizeof(IMAGESIZE));
    fread(image_size, sizeof(IMAGESIZE), 1, fp);
    rid_obj->size = image_size;
    RGBPIXEL*** pixels = load_pixels(fp, image_size);
    rid_obj->pixels = pixels;
    return rid_obj;
}

/*
* Saves rid object into the given file with `.rid` extension and frees memory used for that rid object.
*/
void save_rid(char filepath[MAX_FILENAME_LEN], RID* rid_obj) {
    check_file_ext(filepath);
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL) {
        printf("[ERROR:save_rid] Error occured while writting to the '%s' file!", filepath);
        getchar();
        exit(-1);
    }
    fwrite(&(rid_obj->size->width), sizeof(unsigned int), 1, fp);
    fwrite(&(rid_obj->size->height), sizeof(unsigned int), 1, fp);
    fseek(fp, sizeof(IMAGESIZE), SEEK_SET);
    for(int y = 0; y < rid_obj->size->height; y++) {
        for(int x = 0; x < rid_obj->size->width; x++) {
            long int offset = 8 + y*rid_obj->size->width*sizeof(RGBPIXEL) + x*sizeof(RGBPIXEL);
            fseek(fp, offset, SEEK_SET);
            fwrite(rid_obj->pixels[y][x], sizeof(RGBPIXEL), 1, fp);
        }
    }
    free(rid_obj); // free alocated memory of the rid object
    fclose(fp);
}