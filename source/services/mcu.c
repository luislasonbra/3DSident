#include "mcu.h"

static Handle mcuHandle;

Result mcuInit(void)
{
    return srvGetServiceHandle(&mcuHandle, "mcu::HWC");
}

Result mcuExit(void)
{
    return svcCloseHandle(mcuHandle);
}

Result MCU_ReadRegister(u8 reg, u32 size, void * data)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();
	
	cmdbuf[0] = IPC_MakeHeader(0x01, 2, 2); // 0x00010082
	cmdbuf[0] = 0x10082;
	cmdbuf[1] = reg;
	cmdbuf[2] = size;
	cmdbuf[3] = size << 4 | 0xC;
	cmdbuf[4] = (u32)data;
	
	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;
	
	return cmdbuf[1];
}

Result MCU_GetInfoRegisters(u32 size, void * data)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();
	
	cmdbuf[0] = IPC_MakeHeader(0x03, 1, 2); // 0x00030042
	cmdbuf[0] = 0x10082;
	cmdbuf[1] = size;
	cmdbuf[2] = size << 4 | 0xC;
	cmdbuf[3] = (u32)data;
	
	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;
	
	return cmdbuf[1];
}

Result MCU_GetBatteryVoltage(u8 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x04, 0, 0); // 0x00040000

	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result MCU_GetBatteryLevel(u8 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x05, 0, 0); // 0x00050000

	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result MCU_GetSoundVolume(u8 * out) // Same as HIDUSER_GetSoundVolume
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x0B, 0, 0); // 0x000B0000

	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result MCU_GetFwVerHigh(u8 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x10, 0, 0); // 0x00100000

	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result MCU_GetFwVerLow(u8 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x11, 0, 0); // 0x00110000

	if (R_FAILED(ret = svcSendSyncRequest(mcuHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}