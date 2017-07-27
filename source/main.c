#include <3ds.h>
#include <citro3d.h>

#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "config.h"
#include "fs.h"
#include "kernel.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screen.h"
#include "screenshot.h"
#include "system.h"
#include "utils.h"
#include "wifi.h"

#define selector_xDistance 0
#define selector_yDistance 18
#define MAX_ITEMS 10

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

char kernerlVersion[100], systemVersion[100], firmVersion[100], sdmcCID[33], nandCID[33], username[15], birthday[6], eulaVer[6], pin[6], email[512];
u32 sdTitiles;
u32 nandTitles;

void kernelMenu(void)
{	
	float width = 0;
	
	width = screen_get_string_width("Kernel Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "Kernel Menu");

	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Kernel version:");
	width = screen_get_string_width("Kernel version:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%s", kernerlVersion);
	
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "FIRM version:");
	width = screen_get_string_width("FIRM version:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", firmVersion);
	
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "System version:");
	width = screen_get_string_width("System version:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%s", systemVersion);
	
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "SDMC CID:");
	width = screen_get_string_width("SDMC CID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%s", sdmcCID);

	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "NAND CID:");
	width = screen_get_string_width("NAND CID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%s", nandCID);
	
	screen_draw_string(20, 200, 0.41f, 0.41f, COLOUR_SUBJECT, "Device ID:");
	width = screen_get_string_width("Device ID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "%lu", getDeviceId());
}

void systemMenu(void)
{
	float width = 0;
	
	width = screen_get_string_width("System Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "System Menu");
	
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Model:");
	width = screen_get_string_width("Model:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%s (%s)", getModel(), getRegion());
	
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Language:");
	width = screen_get_string_width("Language:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", getLang());
	
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "ECS Device ID:");
	width = screen_get_string_width("ECS Device ID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%llu", getSoapId());
	
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "Local friend code seed:");
	width = screen_get_string_width("Local friend code seed:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%010llX", getLocalFriendCodeSeed());
	
	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "MAC Address:");
	width = screen_get_string_width("MAC Address:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%s", getMacAddress());
	
	screen_draw_string(20, 200, 0.41f, 0.41f, COLOUR_SUBJECT, "Serial number:");
	width = screen_get_string_width("Serial number:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "%s", getSerialNum());
	
	screen_draw_string(20, 216, 0.41f, 0.41f, COLOUR_SUBJECT, "Screen type:");
	width = screen_get_string_width("Screen type:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 216, 0.41f, 0.41f, COLOUR_VALUE, "%s", getScreenType());
}

void batteryMenu(void)
{
	float width = 0;
	
	u8 batteryPercent = 0, batteryVolt = 0, mcuFwMajor = 0, mcuFwMinor = 0;
	bool isConnected = false;
	
	width = screen_get_string_width("Battery Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "Battery Menu");
	
	mcuGetBatteryLevel(&batteryPercent);
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Battery percentage:");
	width = screen_get_string_width("Battery percentage:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%3d%%", batteryPercent);
	
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Battery status:");
	width = screen_get_string_width("Battery status:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", batteryStatus());
	
	mcuGetBatteryVoltage(&batteryVolt);
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "Battery voltage:");
	width = screen_get_string_width("Battery voltage:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%d (%.1f V)", batteryVolt, 5.0 * ((double)batteryVolt / 256.0));
	
	PTMU_GetAdapterState(&isConnected);
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "Adapter state:");
	width = screen_get_string_width("Adapter state:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%s", isConnected? "connected" : "disconnected");
	
	GetMcuFwVerHigh(&mcuFwMajor);
	GetMcuFwVerLow(&mcuFwMinor);
	//if (CFG_UNITINFO == 0)
	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "MCU firmware:");
	width = screen_get_string_width("MCU firmware:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%u.%u", (mcuFwMajor - 16), mcuFwMinor);
}

void NNIDInfoMenu(void)
{	
	float width = 0;
	u32 principalID = 0;
	char name[0x16];
	
	AccountDataBlock accountDataBlock;
	ACTU_GetAccountDataBlock((u8*)&accountDataBlock, 0xA0, 0x11);

	width = screen_get_string_width("NNID Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "NNID Menu");
	
	// getNNIDInfo(0x11, 0x8), getNNIDInfo(0x11, 0x1C), getNNIDInfo(0x11, 0x15) are all the same.
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "NNID:");
	width = screen_get_string_width("NNID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%s", getNNIDInfo(0x11, 0x8));

	utf2ascii(name, accountDataBlock.miiName);
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Mii name:");
	width = screen_get_string_width("Mii name:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", name);
	
	ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC);
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "Principal ID:");
	width = screen_get_string_width("Principal ID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%u", principalID);
	
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "Persistent ID:");
	width = screen_get_string_width("Persistent ID:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%u", accountDataBlock.persistentID);
	
	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "Transferable ID Base:");
	width = screen_get_string_width("Transferable ID Base:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%llu", accountDataBlock.transferableID);
	
	screen_draw_string(20, 200, 0.41f, 0.41f, COLOUR_SUBJECT, "Country:");
	width = screen_get_string_width("Country:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "%s", getNNIDInfo(0x3, 0xB));
	
	screen_draw_string(20, 216, 0.41f, 0.41f, COLOUR_SUBJECT, "Time Zone:");
	width = screen_get_string_width("Time Zone:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 216, 0.41f, 0.41f, COLOUR_VALUE, "%s", getNNIDInfo(0x41, 0x1E));
}

void configInfoMenu(void)
{
	float width = 0;
	u8 answer[0x21] = {0};
	
	width = screen_get_string_width("Config Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "Config Menu");
	
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Username:");
	width = screen_get_string_width("Username:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%s", username);
	
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Birthday:");
	width = screen_get_string_width("Birthday:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", birthday);
	
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "EULA version:");
	width = screen_get_string_width("EULA version:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%s", eulaVer);
	
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "Parental control pin:");
	width = screen_get_string_width("Parental control pin:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%s", pin);
	
	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "Parental control email:");
	width = screen_get_string_width("Parental control email:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%s", email);
	
	getParentalSecretAnswer(answer);
	screen_draw_string(20, 200, 0.41f, 0.41f, COLOUR_SUBJECT, "Parental control answer:");
	width = screen_get_string_width("Parental control answer:", 0.41f, 0.41f);
	
	if (!answer[0])
		screen_draw_string((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "(null)");
	else
		screen_draw_stringf((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "%s\n", answer + 1);
	
	/*screen_draw_string(20, 216, 0.41f, 0.41f, COLOUR_SUBJECT, "Debug mode:");
	width = screen_get_string_width("Country:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 216, 0.41f, 0.41f, COLOUR_VALUE, "%s", isDebugModeEnabled()? "enabled" : "disabled");*/
}

void hardwareMenu(void)
{
	float width = 0;
	bool hpInserted = false;
	u8 volume = 0;
	
	width = screen_get_string_width("Hardware Menu", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "Hardware Menu");
	
	DSP_GetHeadphoneStatus(&hpInserted);
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Headphone status:");
	width = screen_get_string_width("Headphone status:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "%s", hpInserted? "inserted" : "not inserted");
	
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Card slot status:");
	width = screen_get_string_width("Card slot status:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%s", getCardSlotStatus());
	
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "SDMC status:");
	width = screen_get_string_width("SDMC status:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%s", detectSD()? "detected" : "not detected");
	
	HIDUSER_GetSoundVolume(&volume);
	double volPercent = (volume * 1.5873015873);
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "Volume slider state:");
	width = screen_get_string_width("Volume slider state:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%d (%.0lf%%)", volume, volPercent);
	
	double _3dSliderPercent = (osGet3DSliderState() * 100.0);
	screen_draw_string(20, 184, 0.41f, 0.41f, COLOUR_SUBJECT, "3D slider state:");
	width = screen_get_string_width("3D slider state:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 184, 0.41f, 0.41f, COLOUR_VALUE, "%.1lf (%.0lf%%)", osGet3DSliderState(), _3dSliderPercent);

	screen_draw_string(20, 200, 0.41f, 0.41f, COLOUR_SUBJECT, "Brightness:");
	width = screen_get_string_width("Brightness:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 200, 0.41f, 0.41f, COLOUR_VALUE, "%s", getBrightness(1));
}

void wifiMenu(void)
{
	screen_draw_rect(0, 19, 400, 221, RGBA8(242, 241, 239, 255));
	
	wifiSlotStructure slotData;
	
	Result wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 0, (u8*)&slotData);
	if ((!wifiRet) && (slotData.set))
	{
		screen_draw_rect(15, 27, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 28, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 30, 0.41f, 0.41f, COLOUR_SUBJECT, "WiFi Slot 1:");
		if (slotData.network.use) 
		{
			screen_draw_stringf(20, 46, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network.SSID);
			screen_draw_stringf(20, 62, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network.passphrase);
		}
		else if (slotData.network_WPS.use) 
		{
			screen_draw_stringf(20, 46, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network_WPS.SSID);
			screen_draw_stringf(20, 62, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network_WPS.passphrase);
		}
		screen_draw_stringf(20, 78, 0.41f, 0.41f, COLOUR_VALUE, "Mac address: %02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
	}
	
	wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 1, (u8*)&slotData);
	if ((!wifiRet) && (slotData.set))
	{
		screen_draw_rect(15, 95, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 96, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 98, 0.41f, 0.41f, COLOUR_SUBJECT, "WiFi Slot 2:");
		if (slotData.network.use) 
		{
			screen_draw_stringf(20, 114, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network.SSID);
			screen_draw_stringf(20, 130, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network.passphrase);
		}
		else if (slotData.network_WPS.use) 
		{
			screen_draw_stringf(20, 114, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network_WPS.SSID);
			screen_draw_stringf(20, 130, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network_WPS.passphrase);
		}
		screen_draw_stringf(20, 146, 0.41f, 0.41f, COLOUR_VALUE, "Mac address: %02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
	}
	
	wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 2, (u8*)&slotData);
	if ((!wifiRet) && (slotData.set))
	{
		screen_draw_rect(15, 163, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 164, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 166, 0.41f, 0.41f, COLOUR_SUBJECT, "WiFi Slot 3:");
		if (slotData.network.use) 
		{
			screen_draw_stringf(20, 182, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network.SSID);
			screen_draw_stringf(20, 198, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network.passphrase);
		}
		else if (slotData.network_WPS.use) 
		{
			screen_draw_stringf(20, 182, 0.41f, 0.41f, COLOUR_VALUE, "SSID: %s", slotData.network_WPS.SSID);
			screen_draw_stringf(20, 198, 0.41f, 0.41f, COLOUR_VALUE, "Pass: %s", slotData.network_WPS.passphrase);
		}
		screen_draw_stringf(20, 214, 0.41f, 0.41f, COLOUR_VALUE, "Mac address: %02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
	}
}

void storageMenu(void)
{
	u64 sdUsed = 0, sdTotal = 0, ctrUsed = 0, ctrTotal = 0, twlUsed = 0, twlTotal = 0, twlpUsed = 0, twlpTotal = 0; 
	
	char sdFreeSize[16], sdUsedSize[16], sdTotalSize[16];
	char ctrFreeSize[16], ctrUsedSize[16], ctrTotalSize[16];
	char twlFreeSize[16], twlUsedSize[16], twlTotalSize[16];
	char twlpFreeSize[16], twlpUsedSize[16], twlpTotalSize[16];
	
	screen_draw_rect(0, 19, 400, 221, RGBA8(242, 241, 239, 255));
	
	getSizeString(sdFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_SD));
	getSizeString(sdUsedSize, getUsedStorage(SYSTEM_MEDIATYPE_SD));
	getSizeString(sdTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_SD));
	getSizeString(ctrFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	getSizeString(ctrUsedSize, getUsedStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	getSizeString(ctrTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	getSizeString(twlFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	getSizeString(twlUsedSize, getUsedStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	getSizeString(twlTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	getSizeString(twlpFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));
	getSizeString(twlpUsedSize, getUsedStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));
	getSizeString(twlpTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));
	
	sdUsed = getUsedStorage(SYSTEM_MEDIATYPE_SD);
	sdTotal = getTotalStorage(SYSTEM_MEDIATYPE_SD);
	screen_draw_rect(20, 105, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(21, 106, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(21, 106, (((double)sdUsed / (double)sdTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(85, 50, 0.41f, 0.41f, COLOUR_VALUE, "SD:");
	screen_draw_string(85, 71, 0.41f, 0.41f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(85, 87, 0.41f, 0.41f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(85, 103, 0.41f, 0.41f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((85 + screen_get_string_width("Free:", 0.41f, 0.41f) + 3), 71, 0.41f, 0.41f, COLOUR_VALUE, "%s", sdFreeSize);
	screen_draw_stringf((85 + screen_get_string_width("Used:", 0.41f, 0.41f) + 3), 87, 0.41f, 0.41f, COLOUR_VALUE, "%s", sdUsedSize);
	screen_draw_stringf((85 + screen_get_string_width("Total:", 0.41f, 0.41f) + 3), 103, 0.41f, 0.41f, COLOUR_VALUE, "%s", sdTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 20, 40);
	
	ctrUsed = getUsedStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	ctrTotal = getTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	screen_draw_rect(220, 105, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(221, 106, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(221, 106, (((double)ctrUsed / (double)ctrTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(285, 50, 0.41f, 0.41f, COLOUR_VALUE, "CTR Nand:");
	screen_draw_string(285, 71, 0.41f, 0.41f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(285, 87, 0.41f, 0.41f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(285, 103, 0.41f, 0.41f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((285 + screen_get_string_width("Free:", 0.41f, 0.41f) + 3), 71, 0.41f, 0.41f, COLOUR_VALUE, "%s", ctrFreeSize);
	screen_draw_stringf((285 + screen_get_string_width("Used:", 0.41f, 0.41f) + 3), 87, 0.41f, 0.41f, COLOUR_VALUE, "%s", ctrUsedSize);
	screen_draw_stringf((285 + screen_get_string_width("Total:", 0.41f, 0.41f) + 3), 103, 0.41f, 0.41f, COLOUR_VALUE, "%s", ctrTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 220, 40);
	
	twlUsed = getUsedStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	twlTotal = getTotalStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	screen_draw_rect(20, 200, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(21, 201, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(21, 201, (((double)twlUsed / (double)twlTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(85, 145, 0.41f, 0.41f, COLOUR_VALUE, "TWL Nand:");
	screen_draw_string(85, 166, 0.41f, 0.41f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(85, 182, 0.41f, 0.41f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(85, 198, 0.41f, 0.41f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((85 + screen_get_string_width("Free:", 0.41f, 0.41f) + 3), 166, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlFreeSize);
	screen_draw_stringf((85 + screen_get_string_width("Used:", 0.41f, 0.41f) + 3), 182, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlUsedSize);
	screen_draw_stringf((85 + screen_get_string_width("Total:", 0.41f, 0.41f) + 3), 198, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 20, 135);
	
	twlpUsed = getUsedStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	twlpTotal = getTotalStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	screen_draw_rect(220, 200, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(221, 201, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(221, 201, (((double)twlpUsed / (double)twlpTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(285, 145, 0.41f, 0.41f, COLOUR_VALUE, "TWL Photo:");
	screen_draw_string(285, 166, 0.41f, 0.41f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(285, 182, 0.41f, 0.41f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(285, 198, 0.41f, 0.41f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((285 + screen_get_string_width("Free:", 0.41f, 0.41f) + 3), 166, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlpFreeSize);
	screen_draw_stringf((285 + screen_get_string_width("Used:", 0.41f, 0.41f) + 3), 182, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlpUsedSize);
	screen_draw_stringf((285 + screen_get_string_width("Total:", 0.41f, 0.41f) + 3), 198, 0.41f, 0.41f, COLOUR_VALUE, "%s", twlpTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 220, 135);
}

void miscMenu(void)
{	
	float width = 0;
	double wifiPercent = (osGetWifiStrength() * 33.3333333333);
	
	width = screen_get_string_width("Miscellaneous", 0.41f, 0.41f);
	screen_draw_string(((400 - width) / 2), 90, 0.41f, 0.41f, COLOUR_MENU, "Miscellaneous");
	
	screen_draw_string(20, 120, 0.41f, 0.41f, COLOUR_SUBJECT, "Installed titles:");
	width = screen_get_string_width("Installed titles:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 120, 0.41f, 0.41f, COLOUR_VALUE, "SD: %lu (NAND: %lu)", sdTitiles, nandTitles);
	
	u64 homemenuID = 0;
	Result ret = APT_GetAppletInfo(APPID_HOMEMENU, &homemenuID, NULL, NULL, NULL, NULL);
	screen_draw_string(20, 136, 0.41f, 0.41f, COLOUR_SUBJECT, "Homemenu ID:");
	width = screen_get_string_width("Homemenu ID:", 0.41f, 0.41f);
	if(ret == 0)
		screen_draw_stringf((20 + width + 3), 136, 0.41f, 0.41f, COLOUR_VALUE, "%016llX", homemenuID);
	
	screen_draw_string(20, 152, 0.41f, 0.41f, COLOUR_SUBJECT, "WiFi signal strength:");
	width = screen_get_string_width("WiFi signal strength:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 152, 0.41f, 0.41f, COLOUR_VALUE, "%d (%.0lf%%)", osGetWifiStrength(), wifiPercent);
	
	u32 ip = gethostid();
	screen_draw_string(20, 168, 0.41f, 0.41f, COLOUR_SUBJECT, "IP:");
	width = screen_get_string_width("IP:", 0.41f, 0.41f);
	screen_draw_stringf((20 + width + 3), 168, 0.41f, 0.41f, COLOUR_VALUE, "%lu.%lu.%lu.%lu", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
}

void initServices(void)
{
	dspInit();
	cfguInit();
	cfgsInit();
	fsInit();
	sdmcInit();
	openArchive(ARCHIVE_SDMC);
	ptmuInit();
	mcuInit();
	amInit(); 
	amAppInit();
	psInit();
	aptInit();
	hidInit();
	actInit(SDK(11, 2, 0, 200), 0x20000);
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);
	
	gfxInitDefault();
	gfxSet3D(false);
	romfsInit();
	screen_init();
	
	screen_load_texture_file(TEXTURE_BOTTOM_SCREEN_BG, "romfs:/bottomScreen.png", true);
	screen_load_texture_file(TEXTURE_TOP_SCREEN_BG, "romfs:/topScreen.png", true);
	screen_load_texture_file(TEXTURE_ICON, "romfs:/icon.png", true);
	screen_load_texture_file(TEXTURE_DRIVE_ICON, "romfs:/drive.png", true);
	
	if (isN3DS())
		osSetSpeedupEnable(true);
	
	// Simply because I don't want the following constantly being checked for in a while loop.
	strcpy(kernerlVersion, getVersion(0));
	strcpy(firmVersion, getVersion(1));
	strcpy(systemVersion, getVersion(2));
	strncpy(sdmcCID, getCID(0), 33);
	strncpy(nandCID, getCID(1), 33);
	strncpy(username, getUsername(), 15);
	strncpy(birthday, getBirthday(), 6);
	strncpy(eulaVer, getEulaVersion(), 6);
	strncpy(pin, getParentalPin(), 6);
	strncpy(email, getParentalEmail(), 512);
	
	sdTitiles = titleCount(MEDIATYPE_SD);
	nandTitles = titleCount(MEDIATYPE_NAND);
}

void termServices(void)
{
	osSetSpeedupEnable(0);
	
	screen_unload_texture(TEXTURE_DRIVE_ICON);
	screen_unload_texture(TEXTURE_ICON);
	screen_unload_texture(TEXTURE_TOP_SCREEN_BG);
	screen_unload_texture(TEXTURE_BOTTOM_SCREEN_BG);
	
	screen_exit();
	romfsExit();
	gfxExit();
	
    socExit();
	actExit();
	hidExit();
	aptExit();
	psExit();
	acExit();
	amExit();
	httpcExit();
	mcuExit();
	ptmuExit();
	closeArchive();
	sdmcExit();
	fsExit();
	cfgsExit();
	cfguExit();
	dspExit();
}

int	touchButton(touchPosition *touch, int selection)
{
	if (touch->px >= 15 && touch->px <= 300 && touch->py >= 37 && touch->py <= 56)
		selection = 1;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 56 && touch->py <= 73)
		selection = 2;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 73 && touch->py <= 92)
		selection = 3;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 92 && touch->py <= 110)
		selection = 4;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 110 && touch->py <= 127)
		selection = 5;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 127 && touch->py <= 144)
		selection = 6;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 144 && touch->py <= 161)
		selection = 7;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 161 && touch->py <= 178)
		selection = 8;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 178 && touch->py <= 195)	
		selection = 9;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 195 && touch->py <= 212)	
		selection = 10;
	
	return selection;
}

int main(int argc, char **argv) 
{
	initServices();
	
	int selection = 1;
	int selector_y = 17; 
	int selector_image_y = 0;
	touchPosition touch;

	while (aptMainLoop()) 
	{
		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		selection = touchButton(&touch, selection);
		
		selector_image_y = selector_y + (selector_yDistance * selection);
		
		screen_begin_frame();
		
		screen_select(GFX_TOP);
		screen_draw_texture(TEXTURE_TOP_SCREEN_BG, 0, 0);
		screen_draw_texture(TEXTURE_ICON, ((400.0 - screen_get_texture_width(TEXTURE_ICON)) / 2.0), 36);
		screen_draw_string(5, 2, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "3DSident v0.7.6");
		
		if (selection == 1)
			kernelMenu();
		else if (selection == 2)
			systemMenu();
		else if (selection == 3)
			batteryMenu();
		else if (selection == 4)
			NNIDInfoMenu();
		else if (selection == 5)
			configInfoMenu();
		else if (selection == 6)
			hardwareMenu();
		else if (selection == 7)
			wifiMenu();
		else if (selection == 8)
			storageMenu();
		else if (selection == 9)
			miscMenu();
		
		screen_select(GFX_BOTTOM);
		screen_draw_texture(TEXTURE_BOTTOM_SCREEN_BG, 0, 0);
		screen_draw_rect(16, selector_image_y, 286, 18, RGBA8(242, 119, 62, 255));
		
		if (selection == 1)
			screen_draw_string(22, 38, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Kernel Information");
		else 
			screen_draw_string(22, 38, 0.41f, 0.41f, COLOUR_MAINMENU, "Kernel Information");
		
		if (selection == 2)
			screen_draw_string(22, 56, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "System Information");
		else
			screen_draw_string(22, 56, 0.41f, 0.41f, COLOUR_MAINMENU, "System Information");
		
		if (selection == 3)
			screen_draw_string(22, 74, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Battery Information");
		else
			screen_draw_string(22, 74, 0.41f, 0.41f, COLOUR_MAINMENU, "Battery Information");
		
		if (selection == 4)
			screen_draw_string(22, 92, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "NNID Information");
		else
			screen_draw_string(22, 92, 0.41f, 0.41f, COLOUR_MAINMENU, "NNID Information");
		
		if (selection == 5)
			screen_draw_string(22, 110, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Config Information");
		else
			screen_draw_string(22, 110, 0.41f, 0.41f, COLOUR_MAINMENU, "Config Information");
		
		if (selection == 6)
			screen_draw_string(22, 128, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Hardware Information");
		else
			screen_draw_string(22, 128, 0.41f, 0.41f, COLOUR_MAINMENU, "Hardware Information");
		
		if (selection == 7)
			screen_draw_string(22, 146, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "WiFi Information");
		else
			screen_draw_string(22, 146, 0.41f, 0.41f, COLOUR_MAINMENU, "WiFi Information");
		
		if (selection == 8)
			screen_draw_string(22, 164, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Storage Information");
		else
			screen_draw_string(22, 164, 0.41f, 0.41f, COLOUR_MAINMENU, "Storage Information");
		
		if (selection == 9)
			screen_draw_string(22, 182, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Miscellaneous");
		else
			screen_draw_string(22, 182, 0.41f, 0.41f, COLOUR_MAINMENU, "Miscellaneous");
		
		if (selection == 10)
			screen_draw_string(22, 200, 0.41f, 0.41f, COLOUR_MAINMENU_HIGHLIGHT, "Exit");
		else
			screen_draw_string(22, 200, 0.41f, 0.41f, COLOUR_MAINMENU, "Exit");
		
		screen_end_frame();
		
		if (kDown & KEY_DDOWN)
			selection++;
		else if (kDown & KEY_DUP)
			selection--;
		
		if (kHeld & KEY_CPAD_DOWN)
		{
			svcSleepThread(50000000);
			selection++;
		}
		else if (kHeld & KEY_CPAD_UP)
		{
			svcSleepThread(50000000);
			selection--;
		}
		
		if (selection > MAX_ITEMS) 
			selection = 1;
		if (selection < 1) 
			selection = MAX_ITEMS;
		
		if (((selection == 10) && ((kDown & KEY_A) || (kDown & KEY_TOUCH))) || (kDown & KEY_START))
			break;
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
	}

	termServices();
	return 0;
}