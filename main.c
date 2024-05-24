/*
This program reads .rid file and reverts color channels of the image or darkens/lightens it.
The result is also saved in .rid file.

You can use supplied python script in order to generate .rid file from an image or 
to generate an image based on result writting .rid file.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/image.h"
#include "include/ridio.h"


// Different modes in which image data is saved
enum Mode {
    RGB, // 0 - nothing is reverted
    RBG, // 1 - reverts blue channel with green channel
    GRB, // 2 - reverts red with green channel
    GBR, // 3 - reverts to Green Blue Red format
    BRG, // 4 - reverts to Blue Red Green format
    BGR, // 5 - reverts to Blue Green Red foramt
    LIGHTEN, // 6 - lightens image by  the given ratio
    DARKEN // 7 - darkens image by the given ratio
};


/*
Main funciton of the program.
*/
int main(int argc, char* argv[])
{
    // Check if arguments have been supplied properly
    // *Note - when not using 6 or 7 mode u can set ratio to anything as it wont be used
    if(argc < 5) {
        printf("[ERROR:main] Supplied too few arguments! The correct syntax is: \n"
        "On linux: ./main <str:input file path> <str:output file path> <int: mode(1-7)>\n"
        "On windows: start .\\main.exe <str:input file path> <str:output file path> <int: mode(1-7)> <float: ratio(for mode 6 and 7)");
    }

    // Open file and load Rid object(RID = RAW IMAGE DATA)
    FILE* fp = open_rid(argv[1]);
    RID* rid_obj = load_rid(fp);
    IMAGESIZE* image_size = rid_obj->size; // size of the image
    RGBPIXEL*** pixels = rid_obj->pixels; // 2d array of ptrs to the pixels' rgb representations

    enum Mode mode = atoi(argv[3]);

    switch(mode) {
        case RGB:
            break;
        case RBG:
            convert_rbg(pixels, image_size);
            break;
        case GRB:
            convert_grb(pixels, image_size);
            break;
        case GBR:
            convert_gbr(pixels, image_size);
            break;
        case BRG:
            convert_brg(pixels, image_size);
            break;
        case BGR:
            convert_bgr(pixels, image_size);
            break;
        case LIGHTEN:
            lighten_img(pixels, image_size, atof(argv[4]));
            break;
        case DARKEN:
            darken_img(pixels, image_size, atof(argv[4]));
            break;
        default:
            printf("[ERROR:main] Specified uknown mode! Please specifiy mode using num between 1 and 6");
            exit(-1);
    }
    // close file before exitting
    fclose(fp);
    save_rid(argv[2], rid_obj);
    return 0;
}