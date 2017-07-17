#include "config.h"
#include "utils.h"

const char * getUsername(void) 
{
	int i = 0;
	size_t size = 0x16;
	u8 * data = (u8*)malloc(size);
	char * username = (char*)malloc(size / 2);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = 0;
	
	CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, data);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = (char)((u16*)data)[i];
	
	return username;
}

char * getBirthday(void) 
{
	u16 data = 0;
	static char birthday[0x8];
	
	CFGU_GetConfigInfoBlk2(0x2, 0x000A0001, (u8*)&data);
	
	u8 month = data / 256;
	u8 day = data % 256;
	
	snprintf(birthday, 0x8, "%u/%u", day, month);
	
	return birthday;
}

char * getEulaVersion(void)
{
	u8 data[0x4];
	static char version[0x6];
    
	CFGU_GetConfigInfoBlk2(0x4, 0x000D0000, data);
	
	snprintf(version, 0x6, "%1X.%02X", data[0x1], data[0x0]);
	
	return version;
}

char * getParentalPin(void)
{	
	u8 data[0x94];
	static char parentalPin[0x5];
	
	CFG_GetConfigInfoBlk8(0x94, 0x00100001, data);
	
	snprintf(parentalPin, 0x5, "%u%u%u%u", (data[0xD] - 0x30), (data[0xE] - 0x30), (data[0xF] - 0x30), (data[0x10] - 0x30));
	
	if (strncmp(parentalPin, "4294", 0x4) == 0) // Null parental controls pin
		return "(null)";
		
	return parentalPin;
}

bool isDebugModeEnabled(void)
{
	u8 data[0x4];
	
	CFGU_GetConfigInfoBlk2(0x4, 0x00130000, data);
	
	if ((data[0x0] + data[0x1] + data[0x2] + data[0x3]) == 0x100)
		return true;
	
	return false;
}