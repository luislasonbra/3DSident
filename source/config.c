#include "config.h"

const char * getUsername(void) 
{
	int i;
	size_t size = 0x16;
	u8 * temp = (u8*)malloc(size);
	char * username = (char*)malloc(size / 2);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = 0;
	
	CFGU_GetConfigInfoBlk2(0x1C, 0xA0000, temp);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = (char)((u16*)temp)[i];
	
	return username;
}

char * getBirthday(void) 
{
	u16 date = 0;
	static char birthday[8];
	
	CFGU_GetConfigInfoBlk2(0x2, 0xA0001, (u8*)&date);
	
	u8 month = date / 256;
	u8 day = date % 256;
	
	snprintf(birthday, 8, "%u/%u", day, month);
	
	return birthday;
}

char * getEulaVersion(void)
{
	u8 eulaData[4];
	static char version[6];
    
	CFGU_GetConfigInfoBlk2(4, 0xD0000, eulaData);
	
	snprintf(version, 6, "%02X.%02X", eulaData[1], eulaData[0]);
	
	return version;
}