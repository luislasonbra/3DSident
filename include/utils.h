#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>
#include <sf2d.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void setBilinearFilter(sf2d_texture *texture);
void getSizeString(char *string, uint64_t size);
bool isN3DS();
void endDrawing();
void utf2ascii(char* dst, u16* src);
char * base64Encode(u8 const * input);

#endif