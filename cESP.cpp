#include "Headers.h"

extern cVars CVars;
extern cAddress Address;
cEngine Engine;
extern DirectXHook D3D;
extern cTools Tools;

//bool isingame = false;
bool isingame = true;
bool isfiring = true;

#define GetPlayerByIndex(LTClientShell, dwID) (CPlayer*)(LTClientShell + (dwID * dwCPlayerSize) + dwCPlayerStart)

void cESP::Initialize()
{
	Address.MemoryAdressen();
	//0x1300AD0
}

void cESP::Console()
{
	if (!Address.goodtogo) return;

	if (CVars.NoSky)
		Engine.PushToConsole("SkyScale 0");			// PushToConsole("SkyScale 0");  //去除天空 1
	else
		Engine.PushToConsole("SkyScale 1");

	if (CVars.NoWorld)
		Engine.PushToConsole("DrawWorld 0");			// PushToConsole("DrawWorld 0"); //去除世界 1
	else
		Engine.PushToConsole("DrawWorld 1");

	if (CVars.PlayerFrame)
		Engine.PushToConsole("WireframeModels 1");		// PushToConsole("WireframeModels 1");	//人物线条 0 
	else
		Engine.PushToConsole("WireframeModels 0");

	if (CVars.WhiteWall)
		Engine.PushToConsole("DrawFlat 1");			// PushToConsole("DrawFlat 1");  //白色世界 0
	else
		Engine.PushToConsole("DrawFlat 0");

	if (CVars.PlayerWhite)
		Engine.PushToConsole("TextureModels 0");		// PushToConsole("TextureModels 0"); //人物白色 1												
	else
		Engine.PushToConsole("TextureModels 1");

	if (CVars.HighLight)
		Engine.PushToConsole("LightModels 1");			// PushToConsole("LightModels 1");  //模型加亮 0
	else
		Engine.PushToConsole("LightModels 0");

}



int GetNearestByCrossHair(LPDIRECT3DDEVICE9 pDevice)
{
	int iAimAt = -1;
	float MaxDistance = 0xFFFFFFF;

	D3DVIEWPORT9 viewP;
	pDevice->GetViewport(&viewP);
	float ScreenX = viewP.Width / 2;
	float ScreenY = viewP.Height / 2;
	float PosX = 0.0f, PosY = 0.0f;

	DWORD pLTClientShell = *(DWORD*)Address.aLTClientShell;
	DWORD ClientShell = *(DWORD*)Address.CShellPtr;

	if (!pLTClientShell || !ClientShell) return -1;

	int MyIndex = Engine.GetMyIndex(pLTClientShell);
	if (MyIndex < 0 || MyIndex > 16) return -1;

	CPlayer * Me = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (MyIndex * Address.dwCPlayerSize));
	if (!Me->IsValid()) return -1;

	for (int i = 0; i < 16; i++)
	{
		CPlayer * pPlayer = (CPlayer*)((pLTClientShell + (Address.dwCPlayerStart)) + (i * Address.dwCPlayerSize));
		if (!pPlayer->IsValid()) continue;
		if (!Engine.IsValidClient(pPlayer)) continue;
		if (pPlayer == Me) continue;
		if (Engine.IsTM(pLTClientShell, pPlayer)) continue;

		D3DXVECTOR3 pos = Engine.GetBonePosition(pPlayer->Object, 6);

		if (Engine.WorldToScreen(pDevice, &pos))
		{
			PosX = pos.x > ScreenX ? pos.x - ScreenX : ScreenX - pos.x;
			PosY = pos.y > ScreenY ? pos.y - ScreenY : ScreenY - pos.y;

			float Tmp = sqrt(PosX*PosX + PosY*PosY);

			if (Tmp < MaxDistance)
			{
				MaxDistance = Tmp;
				iAimAt = i;
			}
		}
	}
	return iAimAt;
}

float position_x = 83;
float position_y = 86;

float radius_x = 64;
float radius_y = 64;

const int degrees = 360;
const float degrees_to_radians = 3.1415926535897932384626433832795 / 180.0f;

void RadarESP(int type, IDirect3DDevice9* pDevice)
{
	if (!Address.goodtogo) return;

	DWORD pLTClientShell = *(DWORD*)Address.aLTClientShell;
	DWORD ClientShell = *(DWORD*)Address.CShellPtr;
	CLTPlayerClient* pClient = (CLTPlayerClient*)(*(DWORD*)(Address.aCLTPlayerClient + Address.CLTPlayerClientOffset));

	int MyIndex = Engine.GetMyIndex(pLTClientShell);
	if (MyIndex < 0 || MyIndex > 16) return;

	CPlayer * Me = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (MyIndex * Address.dwCPlayerSize));
	if (!Me->IsValid()) return;

	D3DXVECTOR3 Pos2 = Engine.GetBonePosition(Me->Object, 3);

	for (int i = 0; i < 16; i++)
	{
		CPlayer * Target = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (i * Address.dwCPlayerSize));
		if (!Target->IsValid()) continue;
		if (Target == Me) continue;
		if (!Engine.IsValidClient(Target)) continue;

		D3DXVECTOR3 Pos1 = Engine.GetBonePosition(Target->Object, 6);

		float part1 = pow((Pos2.x - Pos1.x), 2);
		float part2 = pow((Pos2.z - Pos1.z), 2);
		float underRadical = part1 + part2;
		float  result = (float)sqrt(underRadical);
		result /= 64.4;

		float dx = Pos2.x - Pos1.x;
		float dy = Pos2.z - Pos1.z;
		float yaw = atan2f(dy, dx);

		if (result > radius_x)
			result = radius_x;

		//		DWORD dwOldProtect, dwBkup;
		//		VirtualProtect((DWORD*)pClient, 500, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		float myYaw = pClient->CameraYaw;
		//		VirtualProtect((DWORD*)pClient, 500, dwOldProtect, &dwBkup);
		myYaw *= -1;
		myYaw = yaw - myYaw;

		const float radb = myYaw;

		float x2 = position_x - (cos(radb) * result);
		float y2 = position_y + (sin(radb) * result);

		D3DRECT back2 = { x2 - 1, y2 - 1, x2 + 2, y2 + 2 };
		if (type == 1)
		{
			if (Me->Team != Target->Team)
				pDevice->Clear(1, &back2, D3DCLEAR_TARGET, Red, 0, 0);
		}
		else if (type == 2)
		{
			if (Me->Team != Target->Team)
				pDevice->Clear(1, &back2, D3DCLEAR_TARGET, Red, 0, 0);
			else if (type)
				pDevice->Clear(1, &back2, D3DCLEAR_TARGET, Green, 0, 0);
		}
	}
}

void cESP::DrawESP(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Address.goodtogo) return;
//	if (!isingame) return;

	DWORD pLTClientShell = *(DWORD*)Address.aLTClientShell;
	DWORD ClientShell = *(DWORD*)Address.CShellPtr;
	DWORD pCLTPlayerClient = *(DWORD*)(Address.aCLTPlayerClient + Address.CLTPlayerClientOffset);

	if (!pLTClientShell || !pCLTPlayerClient || !ClientShell) return;
	
	INT MyIndex = Engine.GetMyIndex(pLTClientShell);
	if (MyIndex < 0 || MyIndex > 16) return;

	D3DXVECTOR3 mypos, bheadpos, pheadpos, pneckpos, pbodypos;
	DWORD ESP_Color = 0;

	CPlayer * Me = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (MyIndex * Address.dwCPlayerSize));

	if (!Me->IsValid()) return;

	if (Me) mypos = Engine.GetBonePosition(Me->Object, 6);

	for (int i = 0; i < 16; i++)
	{
		CPlayer * pPlayer = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (i * Address.dwCPlayerSize));
		if (!pPlayer) continue;
		if (!pPlayer->IsValid()) continue;
		if (pPlayer == Me) continue;
		if (!Engine.IsValidClient(pPlayer)) continue;
		if (CVars.Targets == 1){ if (pPlayer->Team == Me->Team) continue; }

		bheadpos = Engine.GetBonePosition(pPlayer->Object, 6) + D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		pheadpos = Engine.GetBonePosition(pPlayer->Object, 6);
		pneckpos = Engine.GetBonePosition(pPlayer->Object, 5);
		pbodypos = Engine.GetBonePosition(pPlayer->Object, 2);

		//	Render.DrawString(300, 10 * i, Red, 1, "playerIndex %d %X", i, pPlayer);

		ESP_Color = Engine.GetESPColor(Me, pPlayer, mypos, pheadpos);

		//		DWORD pLTObject = *(DWORD*)(pCLTPlayerClient + 0x354);
		//		LTObject* MyObject = (LTObject*)pLTObject;

		float dist = Engine.GetDistance(mypos, pheadpos);
		if (Engine.WorldToScreen(pDevice, &pheadpos) && Engine.WorldToScreen(pDevice, &pneckpos) && Engine.WorldToScreen(pDevice, &pbodypos) && Engine.WorldToScreen(pDevice, &bheadpos))
		{
			int headcirclerad = Engine.Distance2D(pheadpos.x, pheadpos.y, pneckpos.x, pneckpos.y);

			if (CVars.Box2DESP)
			{
				D3DXVECTOR3 LowBot = Engine.GetMidPoint(Engine.GetBonePosition(pPlayer->Object, 24), Engine.GetBonePosition(pPlayer->Object, 28));
				if (Engine.WorldToScreen(pDevice, &LowBot))
				{
					float w = (LowBot.y - pheadpos.y) / 4;
					Render.DrawBorder(pheadpos.x - w, pheadpos.y, w * 2, LowBot.y - pheadpos.y, 1, ESP_Color);
				}
			}

			if (CVars.Box3DESP)
			{
				D3DXVECTOR3 Pos0, Pos1, Pos2, Pos3, Pos4, Pos5, Pos6, Pos7, Pos8;
				Pos0 = Engine.GetBonePosition(pPlayer->Object, 2);
				Pos1 = Pos0 + D3DXVECTOR3(-50, 150, -50);
				Pos2 = Pos0 + D3DXVECTOR3(-50, -150, -50);
				Pos3 = Pos0 + D3DXVECTOR3(50, -150, -50);
				Pos4 = Pos0 + D3DXVECTOR3(50, 150, -50);
				Pos5 = Pos0 + D3DXVECTOR3(-50, 150, 50);
				Pos6 = Pos0 + D3DXVECTOR3(-50, -150, 50);
				Pos7 = Pos0 + D3DXVECTOR3(50, -150, 50);
				Pos8 = Pos0 + D3DXVECTOR3(50, 150, 50);

				if (Engine.WorldToScreen(pDevice, &Pos1) && Engine.WorldToScreen(pDevice, &Pos2) && Engine.WorldToScreen(pDevice, &Pos3) && Engine.WorldToScreen(pDevice, &Pos4) && Engine.WorldToScreen(pDevice, &Pos5) && Engine.WorldToScreen(pDevice, &Pos6) && Engine.WorldToScreen(pDevice, &Pos7) && Engine.WorldToScreen(pDevice, &Pos8))
				{
					Render.Line(Pos1.x, Pos1.y, Pos2.x, Pos2.y, ESP_Color);
					Render.Line(Pos2.x, Pos2.y, Pos3.x, Pos3.y, ESP_Color);
					Render.Line(Pos3.x, Pos3.y, Pos4.x, Pos4.y, ESP_Color);
					Render.Line(Pos4.x, Pos4.y, Pos1.x, Pos1.y, ESP_Color);
					Render.Line(Pos5.x, Pos5.y, Pos6.x, Pos6.y, ESP_Color);
					Render.Line(Pos6.x, Pos6.y, Pos7.x, Pos7.y, ESP_Color);
					Render.Line(Pos7.x, Pos7.y, Pos8.x, Pos8.y, ESP_Color);
					Render.Line(Pos8.x, Pos8.y, Pos5.x, Pos5.y, ESP_Color);
					Render.Line(Pos1.x, Pos1.y, Pos5.x, Pos5.y, ESP_Color);
					Render.Line(Pos2.x, Pos2.y, Pos6.x, Pos6.y, ESP_Color);
					Render.Line(Pos3.x, Pos3.y, Pos7.x, Pos7.y, ESP_Color);
					Render.Line(Pos4.x, Pos4.y, Pos8.x, Pos8.y, ESP_Color);
				}
			}

			if (CVars.BoneLineESP)
			{
				Engine.DrawBone(pDevice, pPlayer->Object, 6, 0, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 5, 6, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 4, 5, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 3, 4, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 2, 3, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 1, 2, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 21, 1, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 22, 21, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 23, 22, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 24, 23, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 25, 1, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 26, 25, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 27, 26, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 28, 27, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 14, 5, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 15, 14, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 16, 15, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 17, 16, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 18, 17, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 19, 17, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 20, 17, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 7, 5, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 8, 7, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 9, 8, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 10, 9, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 11, 10, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 12, 10, ESP_Color);
				Engine.DrawBone(pDevice, pPlayer->Object, 13, 10, ESP_Color);
				Render.Circle(pheadpos.x, pheadpos.y, headcirclerad * 2, true, ESP_Color);
			}

			if (CVars.BonePointESP)
			{
				for (int b = 1; b <= 28; b++)
				{
					D3DXVECTOR3 TmpPos = Engine.GetBonePosition(pPlayer->Object, b);
					if (Engine.WorldToScreen(pDevice, &TmpPos))
					{
						Render.DrawPoint(TmpPos.x, TmpPos.y, 2, 2, ESP_Color);
					}
					Render.Circle(pheadpos.x, pheadpos.y, headcirclerad * 2, true, ESP_Color);
				}
			}

		}
	}
}

void cESP::DrawMiscESP(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Address.goodtogo) return;

	DWORD pLTClientShell = *(DWORD*)Address.aLTClientShell;
	DWORD ClientShell = *(DWORD*)Address.CShellPtr;
	DWORD pCLTPlayerClient = *(DWORD*)(Address.aCLTPlayerClient + Address.CLTPlayerClientOffset);

	if (!pLTClientShell || !pCLTPlayerClient || !ClientShell) return;

	INT MyIndex = Engine.GetMyIndex(pLTClientShell);
	if (MyIndex < 0 || MyIndex > 16) return;

	D3DXVECTOR3 mypos, ptoppos, pbotpos, pheadpos, pbodypos;
	D3DCOLOR ESP_Color = 0;
	D3DCOLOR dwColor;
	D3DXVECTOR3 Box;
	DWORD dwDrawWidth;

	CPlayer * Me = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (MyIndex * Address.dwCPlayerSize));
	if (!Me->IsValid()) return;

	if (Me) mypos = Engine.GetBonePosition(Me->Object, 6);

	if (CVars.Radar2D)
	{
		RadarESP(CVars.Radar2D, pDevice);
	}

	for (int i = 0; i < 16; i++)
	{
		CPlayer * pPlayer = (CPlayer*)((ClientShell + (Address.dwCPlayerStart)) + (i * Address.dwCPlayerSize));
		if (!pPlayer->IsValid()) continue;
		if (pPlayer == Me) continue;
		if (!Engine.IsValidClient(pPlayer)) continue;
		if (CVars.Targets == 1){ if (pPlayer->Team == Me->Team) continue; }

		ptoppos = Engine.GetBonePosition(pPlayer->Object, 6) + D3DXVECTOR3(0, 20, 0);
		pheadpos = Engine.GetBonePosition(pPlayer->Object, 6);
		pbodypos = Engine.GetBonePosition(pPlayer->Object, 2);
		pbotpos = Engine.GetMidPoint(Engine.GetBonePosition(pPlayer->Object, 24), Engine.GetBonePosition(pPlayer->Object, 28)) - D3DXVECTOR3(0, 10, 0);

		ESP_Color = Engine.GetESPColor(Me, pPlayer, mypos, pheadpos);

		int pHealth = pPlayer->Health;

		float dist = Engine.GetDistance(mypos, pheadpos);
		if (Engine.WorldToScreen(pDevice, &pheadpos) && Engine.WorldToScreen(pDevice, &ptoppos) && Engine.WorldToScreen(pDevice, &pbotpos))
		{
			char pName[255];
			char szDist[255];
			char pWeapon[1024];
			char szRank[255];
			char szQQNum[255];
			char szHealth[255];
			char* HasC4 = "->持有 C4<-";

			//		D3DXVECTOR3 LowMid = GetMidPoint(GetBonePosition(pPlayer->Object, 24), GetBonePosition(pPlayer->Object, 28));

			if (CVars.LineESP)
			{
				if (Me->Team != pPlayer->Team)
				{
					Render.Line(D3D.Viewport.Width / 2, D3D.Viewport.Height, pheadpos.x, pheadpos.y, ESP_Color);
				}
			}

			if (CVars.NameESP)
			{
				sprintf_s(pName, "[%s]", pPlayer->Name);
				Render.DrawString(ptoppos.x, ptoppos.y - 30, ESP_Color, 0, D3D.pFont, pName);
			}

			if (CVars.DistanceESP)
			{
				sprintf_s(szDist, "Dist: [%.fm]", dist / 100);
				Render.DrawString(ptoppos.x, ptoppos.y - 45, ESP_Color, 0, D3D.pFont, szDist);
			}

			if (CVars.RankESP)
			{
				sprintf_s(szRank, "Level: [Lv:%d %s]", pPlayer->Rank, ranks[pPlayer->Rank]);
				Render.DrawString(ptoppos.x, ptoppos.y - 60, ESP_Color, 0, D3D.pFont, szRank);
			}

			if (CVars.QQESP)
			{
				sprintf_s(szQQNum, "QQ: [%ld]", pPlayer->QQNum);
				Render.DrawString(ptoppos.x, ptoppos.y - 75, ESP_Color, 0, D3D.pFont, szQQNum);
			}

			if (CVars.C4ESP)
			{
				if (pPlayer->Has_C4)
					Render.DrawString(ptoppos.x, ptoppos.y - 90, ESP_Color, 0, D3D.pFont, HasC4);
			}

			if (CVars.WeaponESP)
			{
				sprintf_s(pWeapon, "Weapon: [%s]", pPlayer->unk->Weapon->WeaponName);
				Render.DrawString(pbotpos.x, pbotpos.y + 5, ESP_Color, 0, D3D.pFont, pWeapon);
			}

			if (CVars.DrawHealth)
			{
				D3DXVECTOR3 LowMid = Engine.GetMidPoint(Engine.GetBonePosition(pPlayer->Object, 24), Engine.GetBonePosition(pPlayer->Object, 28));
				if (Engine.WorldToScreen(pDevice, &LowMid))
				{
					Box = pheadpos - LowMid;
					if (Box.y < 0)
						Box.y *= -1;

					int BoxWidth = (int)Box.y / 2;
					int DrawX = (int)pheadpos.x - (BoxWidth / 2);
					int DrawY = (int)pheadpos.y;

					if (pHealth < 0)
						return;

					dwDrawWidth = pHealth * (LowMid.y - pheadpos.y) / 100;

					if (pHealth > 100)
						pHealth = 100;

					if (pHealth > 80)
						dwColor = 0xFF00FF00;
					else if (pHealth > 60)
						dwColor = 0xFFFFFF00;
					else if (pHealth > 40)
						dwColor = 0xFFFF9B00;
					else
						dwColor = 0xFFFF0000;

					sprintf_s(szHealth, "HP: %d", pHealth);

					if (CVars.DrawHealth == 1)
					{
						Render.FillRGB(DrawX - 6, DrawY - 1, 5, (int)Box.y + 2, BLACK);
						Render.FillRGB(DrawX - 5, DrawY, 3, dwDrawWidth, dwColor);
					}
					else if (CVars.DrawHealth == 2)
					{
						Render.DrawString(ptoppos.x + 10, ptoppos.y - 15, dwColor, 0, D3D.pFont, szHealth);
					}
					else if (CVars.DrawHealth == 3)
					{
						Render.FillRGB(DrawX - 6, DrawY - 1, 5, (int)Box.y + 2, BLACK);
						Render.FillRGB(DrawX - 5, DrawY, 3, dwDrawWidth, dwColor);
						Render.DrawString(ptoppos.x + 10, ptoppos.y - 15, dwColor, 0, D3D.pFont, szHealth);
					}
				}
			}
		}
	}
}

void cESP::PlayerAimbot(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Address.goodtogo) return;

	if (!CVars.Aimbot) return;

	DWORD pLTClientShell = *(DWORD*)Address.aLTClientShell;
	DWORD pCLTPlayerClient = *(DWORD*)(Address.aCLTPlayerClient + Address.CLTPlayerClientOffset);

	if (!pLTClientShell || !pCLTPlayerClient) return;

	INT MyIndex = Engine.GetMyIndex(pLTClientShell);
	if (MyIndex < 0 || MyIndex > 16) return;

	CPlayer * Me = (CPlayer*)((pLTClientShell + (Address.dwCPlayerStart)) + (MyIndex * Address.dwCPlayerSize));
	if (!Me->IsValid()) return;

	int i;
	int BONE_ID;
	switch (int(CVars.AimBoneId))
	{
	case 0: BONE_ID = 6; break;		//Head
	case 1: BONE_ID = 5; break;		//Neck
	case 2: BONE_ID = 7; break;		//Clavicle
	case 3: BONE_ID = 2; break;		//Pelvis
	default: BONE_ID = 6;			//Head
	}

	if (pCLTPlayerClient && isingame)
	{
		i = GetNearestByCrossHair(pDevice);
		if (i < 0 || i > 16) return;
		CPlayer * Target = (CPlayer*)((pLTClientShell + (Address.dwCPlayerStart)) + (i * Address.dwCPlayerSize));
		if (!Target->IsValid()) return;

		D3DXVECTOR3 AimPos = Engine.GetBonePosition(Target->Object, BONE_ID);

		if (CVars.VisCheck && Engine.IsVisible(Engine.GetBonePosition(Me->Object, BONE_ID), Engine.GetBonePosition(Target->Object, BONE_ID)))
		{
			D3DXVECTOR3 DrawPos = AimPos;
			if (Engine.WorldToScreen(pDevice, &DrawPos))
			{
				if (CVars.AimKey == 0)
				{
					Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
					Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
				}
				if (CVars.AimKey == 1)
				{
					if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
				if (CVars.AimKey == 2)
				{
					if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}

				if (CVars.AimKey == 3)
				{
					if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
				if (CVars.AimKey == 4)
				{
					if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}

				if (CVars.AimKey == 5)
				{
					if (GetAsyncKeyState(VK_LMENU) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
			}
		}
		else if (!CVars.VisCheck)
		{
			D3DXVECTOR3 DrawPos = AimPos;
			if (Engine.WorldToScreen(pDevice, &DrawPos))
			{
				if (CVars.AimKey == 0)
				{
					Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
					Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
				}
				if (CVars.AimKey == 1)
				{
					if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
				if (CVars.AimKey == 2)
				{
					if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}

				if (CVars.AimKey == 3)
				{
					if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
				if (CVars.AimKey == 4)
				{
					if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}

				if (CVars.AimKey == 5)
				{
					if (GetAsyncKeyState(VK_LMENU) & 0x8000)
					{
						Engine.SetMouseCursor(pDevice, Me, Target, AimPos);
						Render.DrawString(DrawPos.x, DrawPos.y, ESP_Red, 0, D3D.pFont, "+");
					}
				}
			}
		}
	}

}