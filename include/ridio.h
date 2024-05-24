#pragma once
#ifndef RIDIO_H
#define RIDIO_H

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define MAX_FILENAME_LEN 33000


FILE* open_rid(char filepath[MAX_FILENAME_LEN]);
RID* load_rid(FILE* fp);
void save_rid(char filepath[MAX_FILENAME_LEN], RID* rid_obj);

#endif