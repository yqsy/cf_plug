#pragma once

#include "Headers.h"

class DirectXHook
{
public:
    static void Initialize();
    D3DVIEWPORT9 Viewport;
    LPD3DXFONT pFont;
    LPD3DXLINE pLine;
};

extern IDirect3DDevice9 * pGameDevice;
extern DirectXHook D3D;