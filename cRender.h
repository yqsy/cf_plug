#ifndef _RENDER_H_
#define _RENDER_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class RenderClass
{
public:
    RenderClass()
    {
        PI = 3.1415926535897932384626433832795;
    }
    ID3DXFont *pFont[10];

    void DrawString(int x, int y, DWORD color, int orientation, LPD3DXFONT g_pFont, const char *fmt, ...);
    void FillRGB(int x, int y, int w, int h, D3DCOLOR color);
    void FillRGBA(float x, float y, float w, float h, D3DCOLOR color);
    void DrawOPX(int x, int y, int width, int height, D3DCOLOR color, int line);
    void DrawStatsBox(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR bcolor, int l);
    void DrawBox(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR bcolor, int l);
    void DrawLineBox(int x, int y, int width, int height, D3DCOLOR color);
    void DrawPoint(int x, int y, int w, int h, D3DCOLOR color);
    void Line(float x1, float y1, float x2, float y2, D3DCOLOR color);
    void Circle(float x, float y, float rad, bool center, D3DCOLOR color);
    void DrawBorder(int x, int y, int w, int h, int o, D3DCOLOR color);
    void Draw_ESP_Border(int x, int y, int w, int h, D3DCOLOR Color);

    void GetDevice(LPDIRECT3DDEVICE9 pDev) { pDevice = pDev; }

private:
    double PI;
    LPDIRECT3DDEVICE9 pDevice;
    int FontNr;
};

#endif

extern RenderClass Render;