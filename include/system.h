#ifndef SYSTEM_H
#define SYSTEM_H

#include <3ds.h>

const char * getModel(void);
const char * getRegion(void);
const char getFirmRegion(void);
const char * getLang(void);
char * getMacAddress(void);
char * getScreenType(void);
u64 getLocalFriendCodeSeed(void);
char * getSerialNum(void);
u64 getSoapId(void);

#endif