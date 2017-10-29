#pragma once

IDirect3DDevice9 * pGameDevice;

typedef HRESULT(WINAPI* BeginScene_)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* EndScene_)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(WINAPI* DrawIndexedPrimitive_)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);
typedef HRESULT(WINAPI* SetStreamSource_)(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride);
typedef HRESULT(WINAPI* Reset_)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(WINAPI* Present_) (LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

Reset_ pReset;
Present_ pPresent;
SetStreamSource_ pSetStreamSource;
DrawIndexedPrimitive_ pDrawIndexedPrimitive;
EndScene_ pEndScene;
BeginScene_ pBeginScene;

LPDIRECT3DTEXTURE9
texRed,
texBlue,
texYellow,
texGreen,
texWhite,
texOrange,
texBlack,
texGray,
texPurple,
texPink,
texCyan,
texSteelBlue,
texLightSteelBlue,
texLightBlue,
texSalmon,
texBrown,
texTeal,
texLime,
texElectricLime,
texGold,
texOrangeRed,
texGreenYellow,
texAquaMarine,
texSkyBlue,
texSlateBlue,
texCrimson,
texDarkOliveGreen,
texPaleGreen,
texDarkGoldenRod,
texFireBrick,
texDarkBlue,
texDarkerBlue,
texDarkYellow,
texLightYellow,
texEmpty;

LPDIRECT3DPIXELSHADER9 Shade_Blue, Shade_Red, Shade_Yellow, Shade_Green, Shade_Fiolet, Shade_Siren, Shade_Orange, Shade_Lemon, Shade_Gray;