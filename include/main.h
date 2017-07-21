#ifndef MAIN_H
#define MAIN_H

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "UbuntuMedium_ttf.h"
#include "UbuntuRegular_ttf.h"

#define selector_xDistance 0 //The distance between the X axis of the two selections
#define selector_yDistance 18 //The distance between the Y axis of the two selections

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

char kernerlVersion[100], systemVersion[100], firmVersion[100], sdmcCID[33], nandCID[33], username[15], birthday[6], eulaVer[6], pin[6], email[512];

sftd_font *font_m, *font_r;
sf2d_texture *topScreen, *bottomScreen, *logo, *drive;

#endif