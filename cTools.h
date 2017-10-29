#ifndef _CTOOLS_H
#define _CTOOLS_H

#include <windows.h>
#include <stdio.h>
#include <malloc.h>

enum DetourType
{
	NRTJ,
	RTNJ,
	RNTJ,
	CUSTOM
};

#define HOOK(func,addy) p##func = (func##_)DetourFunction((PBYTE)addy,(PBYTE)n##func) 
#define UNHOOK(func,addy) p##func = (func##_)DetourFunction((PBYTE)addy,(PBYTE)p##func) 
#define StartRoutine(Function) CreateThread(0,0,(LPTHREAD_START_ROUTINE)Function,0,0,0);
#define MB(text,head) MessageBox(NULL, text, head, MB_OK);

#define CHECK_BAD(x) if(IsBadReadPtr(this,sizeof(x))) return
#define CHECK_BAD_NUM(x) if(IsBadReadPtr(this,sizeof(x))) return 0
#define CHECK_BAD_PTR(x) if(IsBadReadPtr(this,sizeof(x))) return nullptr

class cTools
{
public:
	void PrintDbg(const char *fmt, ...);
	DWORD* Dev(DWORD Base, DWORD Len);
	void *DetourFunc(BYTE *src, const BYTE *dst, const int len);
	void *DetourRemove(BYTE *src, BYTE *restore, INT len);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask);
	DWORD GetModuleSize(char * pModuleName);
	void MEMwrite(void *adr, void *ptr, int size);
	void* DetourCreate(BYTE *src, const BYTE *dst, const int len);
private:
	BOOL bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
};

#endif