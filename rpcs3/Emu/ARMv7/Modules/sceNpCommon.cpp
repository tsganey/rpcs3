#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/ARMv7/ARMv7Module.h"

#include "sceNpCommon.h"

LOG_CHANNEL(sceNpCommon);

s32 sceNpAuthInit()
{
	throw EXCEPTION("");
}

s32 sceNpAuthTerm()
{
	throw EXCEPTION("");
}

s32 sceNpAuthCreateStartRequest(vm::cptr<SceNpAuthRequestParameter> param)
{
	throw EXCEPTION("");
}

s32 sceNpAuthDestroyRequest(s32 id)
{
	throw EXCEPTION("");
}

s32 sceNpAuthAbortRequest(s32 id)
{
	throw EXCEPTION("");
}

s32 sceNpAuthGetTicket(s32 id, vm::ptr<void> buf, u32 len)
{
	throw EXCEPTION("");
}

s32 sceNpAuthGetTicketParam(vm::cptr<u8> ticket, u32 ticketSize, s32 paramId, vm::ptr<SceNpTicketParam> param)
{
	throw EXCEPTION("");
}

s32 sceNpAuthGetEntitlementIdList(vm::cptr<u8> ticket, u32 ticketSize, vm::ptr<SceNpEntitlementId> entIdList, u32 entIdListNum)
{
	throw EXCEPTION("");
}

s32 sceNpAuthGetEntitlementById(vm::cptr<u8> ticket, u32 ticketSize, vm::cptr<char> entId, vm::ptr<SceNpEntitlement> ent)
{
	throw EXCEPTION("");
}

s32 sceNpCmpNpId(vm::cptr<SceNpId> npid1, vm::cptr<SceNpId> npid2)
{
	throw EXCEPTION("");
}

s32 sceNpCmpNpIdInOrder(vm::cptr<SceNpId> npid1, vm::cptr<SceNpId> npid2, vm::ptr<s32> order)
{
	throw EXCEPTION("");
}

#define REG_FUNC(nid, name) REG_FNID(SceNpCommon, nid, name)

DECLARE(arm_module_manager::SceNpCommon)("SceNpCommon", []()
{
	REG_FUNC(0x441D8B4E, sceNpAuthInit);
	REG_FUNC(0x6093B689, sceNpAuthTerm);
	REG_FUNC(0xED42079F, sceNpAuthCreateStartRequest);
	REG_FUNC(0x14FC18AF, sceNpAuthDestroyRequest);
	REG_FUNC(0xE2582575, sceNpAuthAbortRequest);
	REG_FUNC(0x59608D1C, sceNpAuthGetTicket);
	REG_FUNC(0xC1E23E01, sceNpAuthGetTicketParam);
	REG_FUNC(0x3377CD37, sceNpAuthGetEntitlementIdList);
	REG_FUNC(0xF93842F0, sceNpAuthGetEntitlementById);
	REG_FUNC(0xFB8D82E5, sceNpCmpNpId);
	REG_FUNC(0x6BC8150A, sceNpCmpNpIdInOrder);
});
