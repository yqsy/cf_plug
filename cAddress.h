#pragma once
#ifndef _CADDRESS_H
#define _CADDRESS_H

class cAddress
{
public:
    // Other //
    DWORD BagMgr = 0;
    DWORD ModelNode = 0;

    // Weapon //
    DWORD weaponMgr = 0;
    DWORD WeaponMgr;

    // ESP //
    DWORD aLTClientShell = 0;
    DWORD CShellPtr = 0;
    DWORD MiscMgr = 0;
    DWORD PlayerSize = 0;
    DWORD dwCPlayerStart = 0;
    DWORD MEOffset = 0;
    DWORD dwCPlayerSize = 0;
    DWORD aLTModel = 0;
    DWORD oGetNodeTransform = 0;

    // Bug Damage //
    DWORD DamagePerMeter = 0;

    // Player Pointer //
    DWORD aCLTPlayerClient = 0;
    DWORD CLTPlayerClientOffset = 0;

    // PTC //
    DWORD aILTClient = 0;
    DWORD PTCOffset = 0;

    DWORD MSGBOX = 0;
    DWORD MSGBOX_1Push = 0;
    DWORD MSGBOX_2Push = 0;
    DWORD MSGBOX_3Push = 0;
    DWORD MSGBOX_Add = 0;

    DWORD PlayerptrMgr = 0;
    DWORD BPlayerInfo = 0;

    // crossfire.exe //
    DWORD WallArray = 0;
    DWORD FireWeaponAddress = 0;
    DWORD WallPointer = 0;

    DWORD aIntersectSegment = 0;

    BYTE StoredBytes[6];

    DWORD Bypass = 0;
    DWORD ByPassLog1 = 0;
    DWORD ByPassLog2 = 0;

    DWORD SendToServer = 0;

    DWORD WeaponFiring = 0;
    DWORD IsNotPausing = 0;
    DWORD IsNotFocus = 0;

    bool goodtogo = false;

    void GetBytes(DWORD Addy);
    void MemoryAdressen();

};

extern cAddress Address;

#endif