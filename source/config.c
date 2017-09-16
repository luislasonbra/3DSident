#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"

struct Birthday
{
    s8  month;  // birthday month (1 - 12)
    s8  day;    // birthday day (1 - 31)
};

const wchar_t * getUsername(void) 
{
	u8 data[0x1C];
	static wchar_t userName[0x13];
    
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, data)))
	{
		for (int i = 0; i < 0x13; i++)
			userName[i] = (wchar_t)((u16 *)data)[i];
	}
	
	return userName;
}

char * getBirthday(void) 
{
	u8 data[0x2];
	static char date[0xA];

	struct Birthday birthday;
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x2, 0x000A0001, data)))
	{
		birthday.month = data[0x01];
		birthday.day = data[0x00];
	
		snprintf(date, 0xA, "%02d/%02d", birthday.day, birthday.month);
	}
	
	return date;
}

char * getEulaVersion(void)
{
	u8 data[0x4];
	static char version[0x6];
    
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x4, 0x000D0000, data)))
		snprintf(version, 0x6, "%1X.%02X", data[0x1], data[0x0]);
	
	return version;
}

char * getParentalPin(void)
{	
	u8 data[0x94];
	static char parentalPin[0x5];
	
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x94, 0x00100001, data)))
		snprintf(parentalPin, 0x5, "%u%u%u%u", (data[0xD] - 0x30), (data[0xE] - 0x30), (data[0xF] - 0x30), (data[0x10] - 0x30));
	
	if (strncmp(parentalPin, "4294", 0x4) == 0) // Null parental controls pin
		return "(null)";
		
	return parentalPin;
}

char * getParentalEmail(void)
{	
	u8 data[0x200];
	static char emailAddr[0x200];
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x200, 0x000C0002, data)))
		snprintf(emailAddr, 0x200, "%s", (data + 1)); 
	
	if (strncmp(emailAddr, "", 0x1) == 0) // Null parental controls email
		return "(null)";
		
	return emailAddr;
}

char * getParentalSecretAnswer(void)
{	
	u8 data[0x94]; // block 0x00100001 is of size 0x94
	static char out[0x21];
	
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x94, 0x00100001, data)))
		u16_to_u8(out, (u16 *)(data + 0x10), 0x21); // 0x21 cause the secret answer can only be 32 characters long.
	
	return out;
}

bool isDebugModeEnabled(void)
{
	u8 data[0x4];
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x4, 0x00130000, data)))
	{
		if ((data[0x0] + data[0x1] + data[0x2] + data[0x3]) == 0x100)
			return true;
	}
	
	return false;
}

bool isUpdatesEnabled(void)
{
	u8 data[0x4];
	bool isEnabled = false;
    
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x4, 0x000F0005, data)))
		isEnabled = data[0] & 0xFF;
	
	return isEnabled;
}