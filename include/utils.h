#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>

void getSizeString(char * string, uint64_t size);
bool isN3DS(void);
void u16_to_u8(char * buf, const u16 * input, size_t bufsize);
char * extract_between(const char *string, const char *str1, const char *str2);
char * base64Encode(u8 const * bytesToEnc, size_t bufLen);

#endif
