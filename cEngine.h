#ifndef _CENGINE_H_
#define _CENGINE_H_
#pragma once

class cEngine
{
public:
    void PushNMessage(const char *szMessage);
    void PushMessage(char* Message);
    void PushToConsole(char* Command);
    float GetDistance(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB);
    bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 *InOut);
    char GetMyIndex(DWORD pLTCLientShell);
    bool IsValidClient(CPlayer* Clients);
    bool IsTM(DWORD pLTClientShell, CPlayer* pPlayer);
    bool __cdecl IsVisible(D3DXVECTOR3 MePos, D3DXVECTOR3 TargetPos);
    D3DXVECTOR3 GetMidPoint(D3DXVECTOR3 V1, D3DXVECTOR3 V2);
    void GetBonePositionEx(cObject* obj, UINT Bone, Transform* Trans);
    void GetNodePosition(cObject* obj, UINT Bone, D3DXVECTOR3 &Out);
    void DrawBone(LPDIRECT3DDEVICE9 pDevice, cObject *Obj, UINT iStart, UINT iEnd, D3DCOLOR Color);
    D3DXVECTOR3 GetBonePosition(cObject* obj, UINT Bone);
    float Distance2D(float x1, float y1, float x2, float y2);
    D3DCOLOR GetESPColor(CPlayer* Me, CPlayer* pPlayer, D3DXVECTOR3 mypos, D3DXVECTOR3 pheadpos);
    void SetMouseCursor(LPDIRECT3DDEVICE9 pDevice, CPlayer* Me, CPlayer* pPlayer, D3DXVECTOR3 pos);

};

extern cEngine Engine;

#endif