#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/ARMv7/ARMv7Module.h"

#include "sceDeci4p.h"

LOG_CHANNEL(sceDeci4p);

s32 sceKernelDeci4pOpen(vm::cptr<char> protoname, u32 protonum, u32 bufsize)
{
	throw EXCEPTION("");
}

s32 sceKernelDeci4pClose(s32 socketid)
{
	throw EXCEPTION("");
}

s32 sceKernelDeci4pRead(s32 socketid, vm::ptr<void> buffer, u32 size, u32 reserved)
{
	throw EXCEPTION("");
}

s32 sceKernelDeci4pWrite(s32 socketid, vm::cptr<void> buffer, u32 size, u32 reserved)
{
	throw EXCEPTION("");
}

s32 sceKernelDeci4pRegisterCallback(s32 socketid, s32 cbid)
{
	throw EXCEPTION("");
}


#define REG_FUNC(nid, name) REG_FNID(SceDeci4pUserp, nid, name)

DECLARE(arm_module_manager::SceDeci4p)("SceDeci4pUserp", []()
{
	REG_FUNC(0x28578FE8, sceKernelDeci4pOpen);
	REG_FUNC(0x63B0C50F, sceKernelDeci4pClose);
	REG_FUNC(0x971E1C66, sceKernelDeci4pRead);
	REG_FUNC(0xCDA3AAAC, sceKernelDeci4pWrite);
	REG_FUNC(0x73371F35, sceKernelDeci4pRegisterCallback);
});
