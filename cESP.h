#pragma once

class cESP
{
public:
	static void Initialize();
	void Console();
	void PlayerAimbot(LPDIRECT3DDEVICE9 pDevice);
	void DrawESP(LPDIRECT3DDEVICE9 pDevice);
	void DrawMiscESP(LPDIRECT3DDEVICE9 pDevice);
};

#define MAX_PLAYERS			16
#define M_PI				3.1415926535897932384626433832795f

#define Xor(x,y) (x^y)

#define Green				D3DCOLOR_ARGB(255, 000, 255, 000)
#define Red					D3DCOLOR_ARGB(255, 255, 000, 000)
#define Blue				D3DCOLOR_ARGB(255, 000, 000, 255)
#define Yellow				D3DCOLOR_ARGB(255, 255, 255, 000)
#define BLACK				D3DCOLOR_ARGB(255, 000, 000, 000)

#define ESP_Green			D3DCOLOR_ARGB(255, 000, 255, 000)
#define ESP_Red				D3DCOLOR_ARGB(255, 255, 000, 000)
#define ESP_Yellow			D3DCOLOR_ARGB(255, 255, 255, 000)
#define ESP_BLACK			D3DCOLOR_ARGB(255, 000, 000, 000)