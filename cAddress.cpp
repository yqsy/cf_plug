#include "Headers.h"

extern cTools Tools;

void cAddress::GetBytes(DWORD Addy)
{
	memcpy(StoredBytes, (void*)Addy, 6);
}

void cAddress::MemoryAdressen()
{
	if (!goodtogo)
	{
		DWORD CShell = (DWORD)GetModuleHandle("CShell.dll");
		DWORD CrossFire = (DWORD)GetModuleHandle("crossfire.exe");

		aIntersectSegment = Tools.FindPattern(CrossFire, 0xFFFFFF, (PBYTE)"\x5d\xc3\xcc\x55\x8b\xec\x8b\x45\x00\x50\x8b\x4d\x00\x51\x8b\x15\x00\x00\x00\x00", "xxxxxxxx?xxx?xxx????");

		CShellPtr = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xc6\x01\x01\x8b\x15\x00\x00\x00\x00\x8b\x82\x00\x00\x00\x00\x8B", "xxxxx????xx????x");
		CShellPtr = *(DWORD*)(CShellPtr + 5);

		//	MiscMgr = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xFF\x15\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\xD9\x1A", "xx????xx????xx");
		//	MiscMgr = *(DWORD*)(MiscMgr + 8);

		weaponMgr = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x04\xB1\xD9\xE8", "xx????xxxxx");
		weaponMgr = *(DWORD*)(weaponMgr + 0x2);

		WallPointer = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8b\x15\x00\x00\x00\x00\x83\xc4\x04\x89\x84\x16\x84\x02\x00\x00", "xx????xxxxxxxxxx");
		WallPointer = *(DWORD*)(WallPointer + 0x2);

		DamagePerMeter = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xa1\x00\x00\x00\x00\xd9\x58\x04\x83\xc4\x04", "x????xxxxxx");
		DamagePerMeter = *(DWORD*)(DamagePerMeter + 0x1);

		aLTClientShell = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8b\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x50\xe8\x00\x00\x00\x00\x83\xc4\x00", "xx????x????xx????xx?");
		aLTClientShell = *(DWORD*)(aLTClientShell + 0x2);

		dwCPlayerStart = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x0F\xB6\x8E\x00\x00\x00\x00\x69\xC9\x00\x00\x00\x00\x8A\x84\x31\x00\x00\x00\x00\x5E\xC3", "xxx????xx????xxx????xx");
		dwCPlayerStart = *(DWORD*)(dwCPlayerStart + 0x3);

		MEOffset = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8A\x84\x31\x00\x00\x00\x00\x5E\xC3", "xxx????xx");
		MEOffset = *(DWORD*)(MEOffset + 0x3);

		dwCPlayerSize = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x69\xC9\x00\x00\x00\x00\x8A\x84\x31\x00\x00\x00\x00\x5E\xC3", "xx????xxx????xx");
		dwCPlayerSize = *(DWORD*)(dwCPlayerSize + 0x2);

		aLTModel = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x44\x24\x0C\x8B\x11", "xx????xxxxxx");
		aLTModel = *(DWORD*)(aLTModel + 0x2);

		aCLTPlayerClient = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x59\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x6A\x04\x68\x00\x00\x00\x00", "x????x????x????x????xxxxxxxxxxxxxxx????");
		aCLTPlayerClient = *(DWORD*)(aCLTPlayerClient + 0x1);

		DWORD oCLTPlayerClient = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x39\x53\x00\x74\x00\x8b\x4b\x00\x8b\x01\x8b\x90\x00\x00\x00\x00\xff\xd2", "xx?x?xx?xxxx????xx");
		GetBytes(oCLTPlayerClient + 0x2);
		CLTPlayerClientOffset = StoredBytes[0];

		aILTClient = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x8B\x11\x8B\x82\x00\x00\x00\x00", "xx????xxxx????");
		aILTClient = *(DWORD*)(aILTClient + 0x2);

		BagMgr = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x89\x3D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xx????x????");
		BagMgr = *(DWORD*)(BagMgr + 0x2);

		PTCOffset = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8B\x88\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\xD1\x8B\x0D\x00\x00\x00\x00\x8B\x11\x8B\x82\x00\x00\x00\x00", "xx????x????xxxx????xxxx????");
		PTCOffset = *(DWORD*)(PTCOffset + 0x2);

		PlayerptrMgr = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x59\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x6A\x04\x68", "x????x????x????x????xxxxxxxxxxxxxxx");
		PlayerptrMgr = *(DWORD*)(PlayerptrMgr + 0x1);
		//	PlayerptrMgr += 0x70;

		//	BPlayerInfo = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8b\x0d\x00\x00\x00\x00\x83\xc4\x00\x69\xc0\x00\x00\x00\x00\xd9\x5c\x08\x00", "xx????xx?xx????xxx?");
		//	BPlayerInfo = *(DWORD*)(BPlayerInfo + 2);

		//	FireWeaponAddress = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x83\xEC\x20\x56\x8B\xF1\x8B\x0D\x00\x00\x00\x00\x8B\x01\x8B\x40\x0C\x52\x56\xFF\xD0\x84\xC0", "xxxxxxxx????xxxxxxxxxxx");

		ModelNode = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x8B\x0D\x00\x00\x00\x00\x83\xC4\x04\x89\x44\x0F\x54\x8B\x15\x00\x00\x00\x00", "xx????xxxxxxxxx????");
		ModelNode = *(DWORD*)(ModelNode + 0x2);

		//	Bypass = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\xE8\x00\x00\x00\x00\x8B\xF0\x83\xC4\x04\x85\xF6\x0F\x84\x00\x00\x00\x00\xD9\x86\x00\x00\x00\x00", "x????xxxxxxxxx????xx????");

		DWORD FPMSGBOX = Tools.FindPattern(CShell, 0xFFFFFF, (PBYTE)"\x80\x3d\x00\x00\x00\x00\x00\x74\x00\x83\x7c\x24\x00\x00\xc6\x05\x00\x00\x00\x00\x00", "xx?????x?xxx??xx?????");
		if (FPMSGBOX != NULL){
			MSGBOX = *(DWORD*)(FPMSGBOX + 37) + (FPMSGBOX + 41);
			MSGBOX_1Push = (*(BYTE*)(FPMSGBOX + 31));
			MSGBOX_2Push = (*(BYTE*)(FPMSGBOX + 33));
			MSGBOX_3Push = (*(BYTE*)(FPMSGBOX + 35));
			MSGBOX_Add = (*(BYTE*)(FPMSGBOX + 43));
		}

		MessageBeep(500);

		goodtogo = true;
	}
}