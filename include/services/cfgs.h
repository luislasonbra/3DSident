#ifndef CFGS_H
#define CFGS_H

#include <3ds.h>

Result cfgsInit(void);
Result cfgsExit(void);
Result cfgsSecureInfoGetSerialNo(char *serial);

#endif