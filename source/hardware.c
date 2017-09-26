#include <stdio.h>

#include "hardware.h"

char * getCardSlotStatus(void)
{
	bool isInserted = false;
	FS_CardType cardType = 0;
	
	static char card[0x14];
	
	if (R_SUCCEEDED(FSUSER_CardSlotIsInserted(&isInserted)))
	{
		if (isInserted)
		{
			FSUSER_GetCardType(&cardType);
			snprintf(card, 0x14, "inserted %s", cardType? "(TWL)" : "(CTR)");
			return card;
		}
	}
	
	snprintf(card, 0x14, "not inserted");
	return card;
}

bool detectSD(void)
{
	bool isSD = false;
	
	if (R_SUCCEEDED(FSUSER_IsSdmcDetected(&isSD)))
		return isSD;
	
	return false;
}

char * getBrightness(u32 screen)
{
	u32 brightness = 0;
	static char level[0xD];
	
	if(R_SUCCEEDED(gspLcdInit()))
	{
		if (R_SUCCEEDED(GSPLCD_GetBrightness(screen, &brightness)))
			gspLcdExit();
	}
	
	snprintf(level, 0xD, "%.0lf%%", (((double)brightness / (double)142.0) * (double)100.0));
	
	return level;
}