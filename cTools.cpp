#include "Headers.h"
cTools Tools;

void cTools::PrintDbg(const char *fmt, ...)
{
	char buf[1024] = { '\0' };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf(buf, fmt, va_alist);
	va_end(va_alist);
	OutputDebugString(buf);
}

DWORD* cTools::Dev(DWORD Base, DWORD Len)
{
	unsigned long i = 0, n = 0;

	for (i = 0; i < Len; i++)
	{
		if (*(BYTE *)(Base + i + 0x00) == 0xC7)n++;
		if (*(BYTE *)(Base + i + 0x01) == 0x06)n++;
		if (*(BYTE *)(Base + i + 0x06) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x07) == 0x86)n++;
		if (*(BYTE *)(Base + i + 0x0C) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x0D) == 0x86)n++;

		if (n == 6) return (DWORD_PTR *)
			(Base + i + 2); n = 0;
	}
	return(0);
}

void* cTools::DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}

void* cTools::DetourRemove(BYTE *src, BYTE *restore, INT len)
{
	DWORD dwBack;
	len = *(BYTE *)(restore - 1);
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(src, restore, len);
	restore[0] = 0xE9;
	*(DWORD *)(restore + 1) = (DWORD)(src - restore) - 5;
	VirtualProtect(src, len, dwBack, &dwBack);
	return 0;
}

BOOL cTools::bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

DWORD cTools::FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

DWORD cTools::GetModuleSize(char * pModuleName)
{
	HMODULE hModule = GetModuleHandle(pModuleName);

	if (!hModule)
		return 0;

	PIMAGE_DOS_HEADER       pImage_Dos_Header = PIMAGE_DOS_HEADER(hModule);
	PIMAGE_NT_HEADERS       pImage_PE_Header = PIMAGE_NT_HEADERS(long(hModule) + pImage_Dos_Header->e_lfanew);
	PIMAGE_OPTIONAL_HEADER pImage_Optional_Header = &pImage_PE_Header->OptionalHeader;

	return pImage_Optional_Header->SizeOfCode;
}

void cTools::MEMwrite(void *adr, void *ptr, int size)
{
	DWORD OldProtection;
	VirtualProtect(adr, size, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy(adr, ptr, size);
	VirtualProtect(adr, size, OldProtection, &OldProtection);
}

void* cTools::DetourCreate(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp;
	DWORD dwback;
	DWORD jumpto, newjump;

	VirtualProtect(src, len, PAGE_READWRITE, &dwback);

	if (src[0] == 0xE9)
	{
		jmp = (BYTE*)malloc(10);
		jumpto = (*(DWORD*)(src + 1)) + ((DWORD)src) + 5;
		newjump = (jumpto - (DWORD)(jmp + 5));
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = newjump;
		jmp += 5;
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = (DWORD)(src - jmp);
	}
	else
	{
		jmp = (BYTE*)malloc(5 + len);
		memcpy(jmp, src, len);
		jmp += len;
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	}
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

	for (int i = 5; i < len; i++)
		src[i] = 0x90;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}