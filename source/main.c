#include <3ds.h>
#include <citro3d.h>

#include "ac.h"
#include "actu.h"
#include "am.h"
#include "config.h"
#include "fs.h"
#include "hardware.h"
#include "kernel.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screen.h"
#include "screenshot.h"
#include "storage.h"
#include "system.h"
#include "utils.h"

#define selector_xDistance 0
#define selector_yDistance 18
#define MAX_ITEMS          10

#define TEXTURE_BOTTOM_SCREEN_BG 0
#define TEXTURE_TOP_SCREEN_BG    1
#define TEXTURE_ICON             2
#define TEXTURE_DRIVE_ICON       3

#define COLOUR_MAINMENU           RGBA8(78, 74, 67, 255)
#define COLOUR_MAINMENU_HIGHLIGHT RGBA8(250, 237, 227, 255)
#define COLOUR_MENU               RGBA8(0, 0, 0, 255)
#define COLOUR_SUBJECT            RGBA8(120, 118, 115, 255)
#define COLOUR_VALUE              RGBA8(67, 72, 66, 255)

char kernerlVersion[100], systemVersion[100], firmVersion[100], initialVersion[0xB];
static u32 sdTitiles = 0, nandTitles = 0, tickets = 0;
static bool isHomebrew = false, displayInfo = true;

void kernelMenu(void)
{	
	float width = 0;
	
	width = screen_get_string_width("Kernel Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "Kernel Menu");

	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Kernel version:");
	width = screen_get_string_width("Kernel version:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "%s", kernerlVersion);
	
	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "FIRM version:");
	width = screen_get_string_width("FIRM version:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s", firmVersion);
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "System version:");
	width = screen_get_string_width("System version:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%s", systemVersion);
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Initial system version:");
	width = screen_get_string_width("Initial system version:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%s", initialVersion);
	
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "SDMC CID:");
	width = screen_get_string_width("SDMC CID:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getSdmcCid() : NULL);

	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "NAND CID:");
	width = screen_get_string_width("NAND CID:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getNandCid() : NULL);
	
	screen_draw_string(15, 220, 0.44f, 0.44f, COLOUR_SUBJECT, "Device ID:");
	width = screen_get_string_width("Device ID:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 220, 0.44f, 0.44f, COLOUR_VALUE, "%lu", displayInfo? getDeviceId() : 0);
}

void systemMenu(void)
{
	float width = 0;
	
	width = screen_get_string_width("System Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "System Menu");
	
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Model:");
	width = screen_get_string_width("Model:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "%s (%s - %s)", getModel(), getRunningHW(), getRegion());
	
	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Language:");
	width = screen_get_string_width("Language:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s", getLang());
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "ECS Device ID:");
	width = screen_get_string_width("ECS Device ID:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%llu", displayInfo? getSoapId() : 0);
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Local friend code seed:");
	width = screen_get_string_width("Local friend code seed:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%010llX", displayInfo? getLocalFriendCodeSeed() : 0);
	
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "MAC Address:");
	width = screen_get_string_width("MAC Address:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getMacAddress() : NULL);
	
	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "Serial number:");
	width = screen_get_string_width("Serial number:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getSerialNumber() : NULL);
	
	screen_draw_string(15, 220, 0.44f, 0.44f, COLOUR_SUBJECT, "Screen type:");
	width = screen_get_string_width("Screen type:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 220, 0.44f, 0.44f, COLOUR_VALUE, "%s", getScreenType());
}

void batteryMenu(void)
{
	float width = 0;
	
	u8 batteryPercent = 0, batteryVolt = 0, fwVerHigh = 0, fwVerLow = 0;
	bool isConnected = false;
	
	width = screen_get_string_width("Battery Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "Battery Menu");
	
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Battery percentage:");
	width = screen_get_string_width("Battery percentage:", 0.44f, 0.44f);
	if (R_SUCCEEDED(MCU_GetBatteryLevel(&batteryPercent)))
		screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "%3d%%", batteryPercent);
	
	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Battery status:");
	width = screen_get_string_width("Battery status:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s", batteryStatus());
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "Battery voltage:");
	width = screen_get_string_width("Battery voltage:", 0.44f, 0.44f);
	if (R_SUCCEEDED(MCU_GetBatteryVoltage(&batteryVolt)))
		screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%d (%.1f V)", batteryVolt, 5.0 * ((double)batteryVolt / 256.0));
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Adapter state:");
	width = screen_get_string_width("Adapter state:", 0.44f, 0.44f);
	if (R_SUCCEEDED(PTMU_GetAdapterState(&isConnected)))
		screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%s", isConnected? "connected" : "disconnected");
	
	//if (CFG_UNITINFO == 0)
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "MCU firmware:");
	width = screen_get_string_width("MCU firmware:", 0.44f, 0.44f);
	if ((R_SUCCEEDED(MCU_GetFwVerHigh(&fwVerHigh))) && (R_SUCCEEDED(MCU_GetFwVerLow(&fwVerLow))))
		screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%u.%u", (fwVerHigh - 0x10), fwVerLow);
	
	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "Power-saving mode:");
	width = screen_get_string_width("Power-saving mode:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", isPowerSaveEnabled()? "enabled" : "disabled");
}

void NNIDInfoMenu(void)
{	
	float width = 0;
	u32 principalID = 0;
	char country[0x3], name[0x16], nnid[0x11], timeZone[0x41];
	
	AccountDataBlock accountDataBlock;
	Result accountDataBlockRet = ACTU_GetAccountDataBlock((u8*)&accountDataBlock, 0xA0, 0x11);
	
	MiiData miiData;
	Result miiDataRet = ACTU_GetAccountDataBlock((u8*)&miiData, 0x60, 0x7);

	width = screen_get_string_width("NNID Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "NNID Menu");
	
	// getNNIDInfo(0x11, 0x8), getNNIDInfo(0x11, 0x1C), getNNIDInfo(0x11, 0x15) are all the same.
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "NNID:");
	width = screen_get_string_width("NNID:", 0.44f, 0.44f);
	if (R_SUCCEEDED(ACTU_GetAccountDataBlock(nnid, 0x11, 0x8)))
		screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? nnid : NULL);

	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Mii name:");
	width = screen_get_string_width("Mii name:", 0.44f, 0.44f);
	if ((R_SUCCEEDED(accountDataBlockRet)) && (R_SUCCEEDED(miiDataRet)))
	{
		u16_to_u8(name, accountDataBlock.miiName, 0x16);
		screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s (%u)", displayInfo? name : NULL, 
			displayInfo? miiData.miiID : 0);
	}
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "Principal ID:");
	width = screen_get_string_width("Principal ID:", 0.44f, 0.44f);
	if (R_SUCCEEDED(ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC)))
		screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%u", displayInfo? principalID : 0);
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Persistent ID:");
	width = screen_get_string_width("Persistent ID:", 0.44f, 0.44f);
	if (R_SUCCEEDED(accountDataBlockRet))
		screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%u", displayInfo? accountDataBlock.persistentID : 0);
	
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "Transferable ID Base:");
	width = screen_get_string_width("Transferable ID Base:", 0.44f, 0.44f);
	if (R_SUCCEEDED(accountDataBlockRet))
		screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%llu", displayInfo? accountDataBlock.transferableID : 0);
	
	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "Country:");
	width = screen_get_string_width("Country:", 0.44f, 0.44f);
	if (R_SUCCEEDED(ACTU_GetAccountDataBlock(country, 0x3, 0xB)))
		screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? country : NULL);
	
	screen_draw_string(15, 220, 0.44f, 0.44f, COLOUR_SUBJECT, "Time Zone:");
	width = screen_get_string_width("Time Zone:", 0.44f, 0.44f);
	if (R_SUCCEEDED(ACTU_GetAccountDataBlock(timeZone, 0x41, 0x1E)))
		screen_draw_stringf((15 + width + 3), 220, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? timeZone : NULL);
}

void configInfoMenu(void)
{
	float width = 0;
	
	width = screen_get_string_width("Config Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "Config Menu");
	
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Username:");
	width = screen_get_string_width("Username:", 0.44f, 0.44f);
	screen_draw_wstring((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, getUsername());
	
	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Birthday:");
	width = screen_get_string_width("Birthday:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getBirthday() : NULL);
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "EULA version:");
	width = screen_get_string_width("EULA version:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%s", getEulaVersion());
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Parental control pin:");
	width = screen_get_string_width("Parental control pin:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getParentalPin() : NULL);
	
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "Parental control email:");
	width = screen_get_string_width("Parental control email:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? getParentalEmail() : NULL);
	
	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "Parental control answer:");
	width = screen_get_string_width("Parental control answer:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", displayInfo? (getParentalSecretAnswer() + 1) : NULL);

	// This is useless in a retail device.
	/*screen_draw_string(15, 220, 0.44f, 0.44f, COLOUR_SUBJECT, "Network updates:");
	width = screen_get_string_width("Network updates:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 220, 0.44f, 0.44f, COLOUR_VALUE, "%s", isUpdatesEnabled()? "enabled" : "disabled"); */
}

void hardwareMenu(void)
{
	float width = 0;
	bool hpInserted = false;
	u8 volume = 0;
	
	width = screen_get_string_width("Hardware Menu", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "Hardware Menu");
	
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Headphone status:");
	width = screen_get_string_width("Headphone status:", 0.44f, 0.44f);
	if (R_SUCCEEDED(DSP_GetHeadphoneStatus(&hpInserted)))
		screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "%s", hpInserted? "inserted" : "not inserted");
	
	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Card slot status:");
	width = screen_get_string_width("Card slot status:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%s", getCardSlotStatus());
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "SDMC status:");
	width = screen_get_string_width("SDMC status:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%s", detectSD()? "detected" : "not detected");
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Volume slider state:");
	width = screen_get_string_width("Volume slider state:", 0.44f, 0.44f);
	if (R_SUCCEEDED(HIDUSER_GetSoundVolume(&volume)))
	{
		double volPercent = (volume * 1.5873015873);
		screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%d (%.0lf%%)", volume, volPercent);
	}
	
	double _3dSliderPercent = (osGet3DSliderState() * 100.0);
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "3D slider state:");
	width = screen_get_string_width("3D slider state:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%.1lf (%.0lf%%)", osGet3DSliderState(), _3dSliderPercent);
	
	screen_draw_string(15, 202, 0.44f, 0.44f, COLOUR_SUBJECT, "Sound output:");
	width = screen_get_string_width("Sound output:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 202, 0.44f, 0.44f, COLOUR_VALUE, "%s", getSoundOutputMode());
	
	if (isN3DS())
	{
		screen_draw_string(15, 220, 0.44f, 0.44f, COLOUR_SUBJECT, "Brightness level:");
		width = screen_get_string_width("Brightness level:", 0.44f, 0.44f);
		screen_draw_stringf((15 + width + 3), 220, 0.44f, 0.44f, COLOUR_VALUE, "%s (auto-brightness mode %s)", getBrightness(1), isAutoBrightnessEnabled()? "enabled" : "disabled");
	}
}

void wifiMenu(void)
{
	char ssid[0x20], passphrase[0x40];

	screen_draw_rect(0, 19, 400, 221, RGBA8(242, 241, 239, 255));
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(0)))
	{
		screen_draw_rect(15, 27, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 28, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 30, 0.44f, 0.44f, COLOUR_SUBJECT, "WiFi Slot 1:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			screen_draw_stringf(20, 46, 0.44f, 0.44f, COLOUR_VALUE, "SSID: %s", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			screen_draw_stringf(20, 62, 0.44f, 0.44f, COLOUR_VALUE, "Pass: %s (%s)", displayInfo? passphrase : NULL, getSecurityMode());
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(1)))
	{
		screen_draw_rect(15, 95, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 96, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 98, 0.44f, 0.44f, COLOUR_SUBJECT, "WiFi Slot 2:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			screen_draw_stringf(20, 114, 0.44f, 0.44f, COLOUR_VALUE, "SSID: %s", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			screen_draw_stringf(20, 130, 0.44f, 0.44f, COLOUR_VALUE, "Pass: %s (%s)", displayInfo? passphrase : NULL, getSecurityMode());
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(2)))
	{
		screen_draw_rect(15, 163, 370, 70, RGBA8(180, 180, 178, 255));
		screen_draw_rect(16, 164, 368, 68, RGBA8(255, 255, 253, 255));
		
		screen_draw_string(20, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "WiFi Slot 3:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			screen_draw_stringf(20, 182, 0.44f, 0.44f, COLOUR_VALUE, "SSID: %s", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			screen_draw_stringf(20, 198, 0.44f, 0.44f, COLOUR_VALUE, "Pass: %s (%s)", displayInfo? passphrase : NULL, getSecurityMode());
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
	screen_draw_string(85, 50, 0.44f, 0.44f, COLOUR_VALUE, "SD:");
	screen_draw_string(85, 71, 0.44f, 0.44f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(85, 87, 0.44f, 0.44f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(85, 103, 0.44f, 0.44f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((85 + screen_get_string_width("Free:", 0.44f, 0.44f) + 3), 71, 0.44f, 0.44f, COLOUR_VALUE, "%s", sdFreeSize);
	screen_draw_stringf((85 + screen_get_string_width("Used:", 0.44f, 0.44f) + 3), 87, 0.44f, 0.44f, COLOUR_VALUE, "%s", sdUsedSize);
	screen_draw_stringf((85 + screen_get_string_width("Total:", 0.44f, 0.44f) + 3), 103, 0.44f, 0.44f, COLOUR_VALUE, "%s", sdTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 20, 40);
	
	ctrUsed = getUsedStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	ctrTotal = getTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	screen_draw_rect(220, 105, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(221, 106, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(221, 106, (((double)ctrUsed / (double)ctrTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(285, 50, 0.44f, 0.44f, COLOUR_VALUE, "CTR Nand:");
	screen_draw_string(285, 71, 0.44f, 0.44f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(285, 87, 0.44f, 0.44f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(285, 103, 0.44f, 0.44f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((285 + screen_get_string_width("Free:", 0.44f, 0.44f) + 3), 71, 0.44f, 0.44f, COLOUR_VALUE, "%s", ctrFreeSize);
	screen_draw_stringf((285 + screen_get_string_width("Used:", 0.44f, 0.44f) + 3), 87, 0.44f, 0.44f, COLOUR_VALUE, "%s", ctrUsedSize);
	screen_draw_stringf((285 + screen_get_string_width("Total:", 0.44f, 0.44f) + 3), 103, 0.44f, 0.44f, COLOUR_VALUE, "%s", ctrTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 220, 40);
	
	twlUsed = getUsedStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	twlTotal = getTotalStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	screen_draw_rect(20, 200, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(21, 201, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(21, 201, (((double)twlUsed / (double)twlTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(85, 145, 0.44f, 0.44f, COLOUR_VALUE, "TWL Nand:");
	screen_draw_string(85, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(85, 182, 0.44f, 0.44f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(85, 198, 0.44f, 0.44f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((85 + screen_get_string_width("Free:", 0.44f, 0.44f) + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlFreeSize);
	screen_draw_stringf((85 + screen_get_string_width("Used:", 0.44f, 0.44f) + 3), 182, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlUsedSize);
	screen_draw_stringf((85 + screen_get_string_width("Total:", 0.44f, 0.44f) + 3), 198, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 20, 135);
	
	twlpUsed = getUsedStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	twlpTotal = getTotalStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	screen_draw_rect(220, 200, 60, 10, RGBA8(120, 118, 115, 255));
	screen_draw_rect(221, 201, 58, 8, RGBA8(242, 241, 239, 255));
	screen_draw_rect(221, 201, (((double)twlpUsed / (double)twlpTotal) * 58.00), 8, RGBA8(242, 119, 62, 255));
	screen_draw_string(285, 145, 0.44f, 0.44f, COLOUR_VALUE, "TWL Photo:");
	screen_draw_string(285, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "Free:");
	screen_draw_string(285, 182, 0.44f, 0.44f, COLOUR_SUBJECT, "Used:");
	screen_draw_string(285, 198, 0.44f, 0.44f, COLOUR_SUBJECT, "Total:");
	screen_draw_stringf((285 + screen_get_string_width("Free:", 0.44f, 0.44f) + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlpFreeSize);
	screen_draw_stringf((285 + screen_get_string_width("Used:", 0.44f, 0.44f) + 3), 182, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlpUsedSize);
	screen_draw_stringf((285 + screen_get_string_width("Total:", 0.44f, 0.44f) + 3), 198, 0.44f, 0.44f, COLOUR_VALUE, "%s", twlpTotalSize);
	screen_draw_texture(TEXTURE_DRIVE_ICON, 220, 135);
}

void miscMenu(void)
{	
	float width = 0;
	double wifiPercent = (osGetWifiStrength() * 33.3333333333);
	u64 homemenuID = 0;
	
	width = screen_get_string_width("Miscellaneous", 0.48f, 0.48f);
	screen_draw_string(((400 - width) / 2), 85, 0.48f, 0.48f, COLOUR_MENU, "Miscellaneous");
	
	screen_draw_string(15, 112, 0.44f, 0.44f, COLOUR_SUBJECT, "Installed titles:");
	width = screen_get_string_width("Installed titles:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 112, 0.44f, 0.44f, COLOUR_VALUE, "SD: %lu (NAND: %lu)", sdTitiles, nandTitles);

	screen_draw_string(15, 130, 0.44f, 0.44f, COLOUR_SUBJECT, "Installed tickets:");
	width = screen_get_string_width("Installed tickets:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 130, 0.44f, 0.44f, COLOUR_VALUE, "%lu", tickets);
	
	screen_draw_string(15, 148, 0.44f, 0.44f, COLOUR_SUBJECT, "Homemenu ID:");
	width = screen_get_string_width("Homemenu ID:", 0.44f, 0.44f);
	if (R_SUCCEEDED(APT_GetAppletInfo(APPID_HOMEMENU, &homemenuID, NULL, NULL, NULL, NULL)))
		screen_draw_stringf((15 + width + 3), 148, 0.44f, 0.44f, COLOUR_VALUE, "%016llX", homemenuID);
	
	screen_draw_string(15, 166, 0.44f, 0.44f, COLOUR_SUBJECT, "WiFi signal strength:");
	width = screen_get_string_width("WiFi signal strength:", 0.44f, 0.44f);
	screen_draw_stringf((15 + width + 3), 166, 0.44f, 0.44f, COLOUR_VALUE, "%d (%.0lf%%)", osGetWifiStrength(), wifiPercent);
	
	u32 ip = gethostid();
	screen_draw_string(15, 184, 0.44f, 0.44f, COLOUR_SUBJECT, "IP:");
	width = screen_get_string_width("IP:", 0.44f, 0.44f);

	if (displayInfo)
		screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%lu.%lu.%lu.%lu", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
	else
		screen_draw_stringf((15 + width + 3), 184, 0.44f, 0.44f, COLOUR_VALUE, "%s", NULL);
}

void initServices(void)
{
	aciInit();
	
	// A crappy way to check if user is running from *hax.
	if (envIsHomebrew() && (R_FAILED(actInit())))
		isHomebrew = envIsHomebrew();
	
	actInit();
	
	amAppInit();
	amInit(); 
	aptInit();
	cfguInit();
	dspInit();
	hidInit();
	mcuInit();
	ptmuInit();
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);
	
	fsInit();
	sdmcInit();
	openArchive(&fsArchive, ARCHIVE_SDMC);
	
	gfxInitDefault();
	gfxSet3D(false);
	romfsInit();
	screen_init();
	
	if (isN3DS())
		osSetSpeedupEnable(true);
	
	screen_load_texture_png(TEXTURE_BOTTOM_SCREEN_BG, "romfs:/bottomScreen.png", true);
	screen_load_texture_png(TEXTURE_TOP_SCREEN_BG, "romfs:/topScreen.png", true);
	screen_load_texture_png(TEXTURE_ICON, "romfs:/icon.png", true);
	screen_load_texture_png(TEXTURE_DRIVE_ICON, "romfs:/drive.png", true);
	
	// Having these in a while loop results in a huge drop in FPS
	
	strcpy(kernerlVersion, getVersion(0));
	strcpy(firmVersion, getVersion(1));
	strcpy(initialVersion, getVersion(2));
	strcpy(systemVersion, getVersion(3));
	
	sdTitiles = titleCount(MEDIATYPE_SD);
	nandTitles = titleCount(MEDIATYPE_NAND);
	tickets = ticketCount();
}

void termServices(void)
{
	screen_unload_texture(TEXTURE_DRIVE_ICON);
	screen_unload_texture(TEXTURE_ICON);
	screen_unload_texture(TEXTURE_TOP_SCREEN_BG);
	screen_unload_texture(TEXTURE_BOTTOM_SCREEN_BG);
	
	if (isN3DS())
		osSetSpeedupEnable(false);
	
	screen_exit();
	romfsExit();
	gfxExit();
	
	closeArchive(fsArchive);
	sdmcExit();
	fsExit();
	
	socExit();
	ptmuExit();
	mcuExit();
	hidExit();
	dspExit();
	cfguExit();
	aptExit();
	amExit();
	actExit();
	acExit();
	aciExit();
}

int touchButton(touchPosition *touch, int selection)
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
		screen_draw_texture(TEXTURE_ICON, ((400.0 - screen_get_texture_width(TEXTURE_ICON)) / 2.0), 31);
		screen_draw_stringf(5, 2, 0.48f, 0.48f, COLOUR_MAINMENU_HIGHLIGHT, "3DSident v0.7.8 %s", isDebugUnit());
		
		switch(selection)
		{
			case 1:
				kernelMenu();
				break;
			
			case 2:
				systemMenu();
				break;
			
			case 3:
				batteryMenu();
				break;
			
			case 4:
				if (!isHomebrew)
					NNIDInfoMenu();
				else
				{
					u32 width = ((400 - (screen_get_string_width("This service cannot be accessed in *hax.", 0.48f, 0.48f))) / 2);
					screen_draw_stringf(width, 120, 0.48f, 0.48f, COLOUR_MAINMENU, "%s", isHomebrew? "This service cannot be accessed in *hax." : "");
				}
				break;
			
			case 5:
				if (!isHomebrew)
					configInfoMenu();
				else
				{
					u32 width = ((400 - (screen_get_string_width("This service cannot be accessed in *hax.", 0.48f, 0.48f))) / 2);
					screen_draw_stringf(width, 120, 0.48f, 0.48f, COLOUR_MAINMENU, "%s", isHomebrew? "This service cannot be accessed in *hax." : "");
				}
				break;
			
			case 6:
				hardwareMenu();
				break;
			
			case 7:
				if (!isHomebrew)
					wifiMenu();
				else
				{
					u32 width = ((400 - (screen_get_string_width("This service cannot be accessed in *hax.", 0.48f, 0.48f))) / 2);
					screen_draw_stringf(width, 120, 0.48f, 0.48f, COLOUR_MAINMENU, "%s", isHomebrew? "This service cannot be accessed in *hax." : "");
				}
				break;
			
			case 8:
				storageMenu();
				break;
			
			case 9:
				miscMenu();
				break;
			case 10:
				screen_draw_string(((400 - screen_get_string_width("Press select to hide user-specific info", 0.48f, 0.48f)) / 2), 112, 
					0.48f, 0.48f, COLOUR_SUBJECT, "Press select to hide user-specific info");
				break;
		}
		
		screen_select(GFX_BOTTOM);
		screen_draw_texture(TEXTURE_BOTTOM_SCREEN_BG, 0, 0);
		screen_draw_rect(16, selector_image_y, 286, 18, RGBA8(242, 119, 62, 255));
		
		screen_draw_string(22, 38, 0.44f, 0.44f, selection == 1? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Kernel Information");
		screen_draw_string(22, 56, 0.44f, 0.44f, selection == 2? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "System Information");
		screen_draw_string(22, 74, 0.44f, 0.44f, selection == 3? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Battery Information");
		screen_draw_string(22, 92, 0.44f, 0.44f, selection == 4? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "NNID Information");
		screen_draw_string(22, 110, 0.44f, 0.44f, selection == 5? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Config Information");
		screen_draw_string(22, 128, 0.44f, 0.44f, selection == 6? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Hardware Information");
		screen_draw_string(22, 146, 0.44f, 0.44f, selection == 7? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "WiFi Information");
		screen_draw_string(22, 164, 0.44f, 0.44f, selection == 8? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Storage Information");
		screen_draw_string(22, 182, 0.44f, 0.44f, selection == 9? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Miscellaneous");
		screen_draw_string(22, 200, 0.44f, 0.44f, selection == 10? COLOUR_MAINMENU_HIGHLIGHT : COLOUR_MAINMENU, "Exit");
		
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

		if (kDown & KEY_SELECT)
		{
			if (displayInfo)
				displayInfo = false;
			else 
				displayInfo = true;
		}
	}

	termServices();
	return 0;
}
