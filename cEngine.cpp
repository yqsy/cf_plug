#include "Headers.h"

extern cVars CVars;
cAddress Address;

#define GetPlayerByIndex(LTClientShell, dwID) (CPlayer*)(LTClientShell + (dwID * Address.dwCPlayerSize) + Address.dwCPlayerStart)

//=== Ingame MSGBOX ===
void cEngine::PushNMessage(const char *szMessage)
{
	typedef void(__cdecl *pGameMsgBox_t)(DWORD, DWORD, DWORD, const char*);
	pGameMsgBox_t pGameMsgBox = (pGameMsgBox_t)Address.MSGBOX;
	pGameMsgBox(Address.MSGBOX_3Push, Address.MSGBOX_2Push, Address.MSGBOX_1Push, szMessage);
}

void cEngine::PushMessage(char* Message)
{
	DWORD MessageAddress = Address.MSGBOX;
	__asm
	{
		PUSH Message;
		PUSH Address.MSGBOX_1Push;
		PUSH Address.MSGBOX_2Push;
		PUSH Address.MSGBOX_3Push;
		call MessageAddress;
		add esp, Address.MSGBOX_Add;
	}
}

void cEngine::PushToConsole(char* Command) 
{
	DWORD dwCShell = (DWORD)GetModuleHandle("CShell.dll");
	if (dwCShell != NULL)
	{
		DWORD *zAddress = (DWORD*)Address.aILTClient;
		void* szConsole = (void*)*(DWORD*)(*zAddress + Address.PTCOffset);
		_asm
		{
			PUSH Command
			CALL szConsole
			ADD ESP, 0x4
		}
	}
}

float cEngine::GetDistance(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB)
{
	return sqrt(((VecA.x - VecB.x) * (VecA.x - VecB.x)) +
		((VecA.y - VecB.y) * (VecA.y - VecB.y)) +
		((VecA.z - VecB.z) * (VecA.z - VecB.z)));
}

bool cEngine::WorldToScreen(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 *InOut)
{
	D3DXVECTOR3 vScreen;
	D3DXVECTOR3 PlayerPos(InOut->x, InOut->y, InOut->z);
	D3DVIEWPORT9 viewPort = { 0 };
	D3DXMATRIX projection, view, world;
	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &projection);
	pDevice->GetTransform(D3DTS_WORLD, &world);
	pDevice->GetViewport(&viewPort);
	D3DXVec3Project(&vScreen, &PlayerPos, &viewPort, &projection, &view, &world);
	if (vScreen.z < 1.0f && vScreen.x > 0.0f && vScreen.y > 0.0f && vScreen.x < viewPort.Width && vScreen.y < viewPort.Height)
	{
		*InOut = vScreen;
		return true;
	}
	return false;
}

char cEngine::GetMyIndex(DWORD pLTCLientShell)
{
	if (pLTCLientShell)
	{
		BYTE clients = *(BYTE*)(pLTCLientShell + Address.dwCPlayerStart);
		if (clients >= 0 && clients < 16)
			return *(char*)(pLTCLientShell + clients * Address.dwCPlayerSize + Address.MEOffset);
	}
	return -1;
}

bool cEngine::IsValidClient(CPlayer* Clients)
{
	if (Clients->Health > 0)
		return true;
	return false;
}

bool cEngine::IsTM(DWORD pLTClientShell, CPlayer* pPlayer)
{
	bool IsAlly = true;
	CPlayer* Me = GetPlayerByIndex(pLTClientShell, GetMyIndex(pLTClientShell));

	if (Me->Team != pPlayer->Team)
		IsAlly = false;

	return IsAlly;
}

IntersectQuery iQuery;
IntersectInfo iInfo;

typedef bool(*IntersectSegment)(const IntersectQuery& Query, IntersectInfo *pInfo);
IntersectSegment oIntersectSegment;

bool __cdecl cEngine::IsVisible(D3DXVECTOR3 MePos, D3DXVECTOR3 TargetPos)
{
	oIntersectSegment = (IntersectSegment)(Address.aIntersectSegment + 0x3);

	iQuery.m_From = MePos;
	iQuery.m_To = TargetPos;

	return !oIntersectSegment(iQuery, &iInfo);
}

D3DXVECTOR3 cEngine::GetMidPoint(D3DXVECTOR3 V1, D3DXVECTOR3 V2)
{
	D3DXVECTOR3 Mid;
	Mid.x = (V1.x + V2.x) / 2;
	Mid.y = (V1.y + V2.y) / 2;
	Mid.z = (V1.z + V2.z) / 2;
	return Mid;
}

void cEngine::GetBonePositionEx(cObject* obj, UINT Bone, Transform* Trans)
{
	DWORD pLTModel = *(DWORD*)Address.aLTModel;
	__asm
	{
		MOV ECX, pLTModel
		MOV EDX, DWORD PTR DS : [ECX]
		MOV EDX, DWORD PTR DS : [EDX + 0x3C]
		PUSH 1
		PUSH Trans
		PUSH Bone
		PUSH obj
		CALL EDX
	}
}

void cEngine::GetNodePosition(cObject* obj, UINT Bone, D3DXVECTOR3 &Out)
{
	Transform Trans;
	GetBonePositionEx(obj, Bone, &Trans);
	Out = Trans.Pos;
	Out.y += 5;
}

void cEngine::DrawBone(LPDIRECT3DDEVICE9 pDevice, cObject *Obj, UINT iStart, UINT iEnd, D3DCOLOR Color)
{
	D3DXVECTOR3 StartPos, EndPos;
	GetNodePosition(Obj, iStart, StartPos);
	GetNodePosition(Obj, iEnd, EndPos);

	if (WorldToScreen(pDevice, &StartPos) && WorldToScreen(pDevice, &EndPos))
		Render.Line(StartPos.x, StartPos.y, EndPos.x, EndPos.y, Color);
}

D3DXVECTOR3 cEngine::GetBonePosition(cObject* obj, UINT Bone)
{
	Transform Trans;
	GetBonePositionEx(obj, Bone, &Trans);
	return (Trans.Pos);
}

float cEngine::Distance2D(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

D3DCOLOR cEngine::GetESPColor(CPlayer* Me, CPlayer* pPlayer, D3DXVECTOR3 mypos, D3DXVECTOR3 pheadpos)
{
	D3DCOLOR ESP_Color = 0;
	if (CVars.VisCheck)
	{
		if (Me->Team != pPlayer->Team && IsVisible(mypos, pheadpos))
			ESP_Color = ESP_Red;
		else if (Me->Team != pPlayer->Team  && !IsVisible(mypos, pheadpos))
			ESP_Color = ESP_Yellow;
		else if (Me->Team == pPlayer->Team)
			ESP_Color = ESP_Green;
		else
			ESP_Color = ESP_Green;
	}
	else if (!CVars.VisCheck)
	{
		if (Me->Team == pPlayer->Team)
			ESP_Color = ESP_Green;
		else
			ESP_Color = ESP_Red;
	}

	return ESP_Color;
}

void cEngine::SetMouseCursor(LPDIRECT3DDEVICE9 pDevice, CPlayer* Me, CPlayer* pPlayer, D3DXVECTOR3 pos)
{
	CLTPlayerClient* pClient = (CLTPlayerClient*)(*(DWORD*)(Address.aCLTPlayerClient + Address.CLTPlayerClientOffset));
	if (pPlayer && pPlayer->Object && pPlayer->Health > 0 && pPlayer != Me)
	{
		D3DXMATRIX View, iView;
		D3DXVECTOR3 fDist;
		pDevice->GetTransform(D3DTS_VIEW, &View);
		D3DXMatrixInverse(&iView, 0, &View);
		D3DXVec3Subtract(&fDist, &pos, &D3DXVECTOR3(iView._41, iView._42, iView._43));

//		DWORD dwOldProtect, dwBkup;
//		VirtualProtect((DWORD*)pClient, 900, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		pClient->Pitch = (float)atan2(-fDist.y, sqrt(fDist.x * fDist.x + fDist.z * fDist.z));
		pClient->Yaw = (float)atan2(fDist.x, fDist.z);
//		VirtualProtect((DWORD*)pClient, 900, dwOldProtect, &dwBkup);

	}
}