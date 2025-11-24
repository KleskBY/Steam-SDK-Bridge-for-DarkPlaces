#pragma once
#include "kleskby/steam.h"

void VM_steam_achivment_unlock(prvm_prog_t* prog)
{
	const char* szString;
	int nCnt;
	VM_SAFEPARMCOUNT(1, VM_steam_achivment_unlock);
	szString = PRVM_G_STRING(OFS_PARM0);
	nCnt = Steam_Achivment_Unlock(szString);
	PRVM_G_FLOAT(OFS_RETURN) = nCnt;
}


void VM_steam_achivment_lock(prvm_prog_t* prog)
{
	const char* szString;
	int nCnt;
	VM_SAFEPARMCOUNT(1, VM_steam_achivment_lock);
	szString = PRVM_G_STRING(OFS_PARM0);
	nCnt = Steam_Achivment_Lock(szString);
	PRVM_G_FLOAT(OFS_RETURN) = nCnt;
}

void VM_steam_open_url(prvm_prog_t* prog)
{
	const char* szString;
	int nCnt;
	VM_SAFEPARMCOUNT(1, VM_steam_achivment_lock);
	szString = PRVM_G_STRING(OFS_PARM0);
	Steam_OpenUrl(szString);
	//PRVM_G_FLOAT(OFS_RETURN) = nCnt;
}