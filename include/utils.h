#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>
#include <sf2d.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void setBilinearFilter(sf2d_texture *texture);
void getSizeString(char *string, uint64_t size);
char * base64Encode(u8 const * input);
bool isN3DS();
void endDrawing();
char * base64encode(const char * input);

#endif