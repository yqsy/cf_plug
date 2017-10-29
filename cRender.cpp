#include "Headers.h"

RenderClass Render;
extern DirectXHook D3D;

struct D3DTLVERTEX
{
    float fX;
    float fY;
    float fZ;
    float fRHW;
    D3DCOLOR Color;
    float fU;
    float fV;
};

D3DTLVERTEX CreateD3DTLVERTEX(float X, float Y, float Z, float RHW, D3DCOLOR color, float U, float V)
{
    D3DTLVERTEX v;

    v.fX = X;
    v.fY = Y;
    v.fZ = Z;
    v.fRHW = RHW;
    v.Color = color;
    v.fU = U;
    v.fV = V;

    return v;
}

void RenderClass::FillRGB(int x, int y, int w, int h, D3DCOLOR color)
{
    if (w < 0)w = 1;
    if (h < 0)h = 1;
    if (x < 0)x = 1;
    if (y < 0)y = 1;

    D3DRECT rec = { x, y, x + w, y + h };
    pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void RenderClass::DrawString(int x, int y, DWORD color, int orientation, LPD3DXFONT g_pFont, const char *fmt, ...)
{
    RECT FontPos = { x, y, x + 120, y + 16 };
    char buf[1024] = { '\0' };
    va_list va_alist;

    va_start(va_alist, fmt);
    vsprintf(buf, fmt, va_alist);
    va_end(va_alist);
    switch (orientation)
    {
    case 0:
        g_pFont->DrawText(NULL, buf, -1, &FontPos, DT_LEFT | DT_NOCLIP, color);
        break;
    case 1:
        g_pFont->DrawText(NULL, buf, -1, &FontPos, DT_CENTER | DT_NOCLIP, color);
        break;
    case 2:
        g_pFont->DrawText(NULL, buf, -1, &FontPos, DT_RIGHT | DT_NOCLIP, color);
        break;
    }
}

void RenderClass::FillRGBA(float x, float y, float w, float h, D3DCOLOR color)
{
    D3DXVECTOR2 vLine[2];

    D3D.pLine->SetWidth(w);
    D3D.pLine->SetAntialias(true);
    D3D.pLine->SetGLLines(true);

    vLine[0].x = x + w / 2;
    vLine[0].y = y;
    vLine[1].x = x + w / 2;
    vLine[1].y = y + h;

    D3D.pLine->Begin();
    D3D.pLine->Draw(vLine, 2, color);
    D3D.pLine->End();
}


void RenderClass::DrawPoint(int x, int y, int w, int h, D3DCOLOR color)
{
    FillRGBA((int)x, (int)y, (int)w, (int)h, color);
}

void RenderClass::DrawOPX(int x, int y, int width, int height, D3DCOLOR color, int line)
{
    FillRGBA(x, y + height, width, line, color);
    FillRGBA(x, y, line, height, color);
    FillRGBA(x, y, width, line, color);
    FillRGBA(x + width, y, line, height, color);
}

void RenderClass::DrawStatsBox(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR bcolor, int l)
{
    DrawOPX(x, y, w, h, color, l);
    FillRGBA(x, y, w, h, bcolor);
}

void RenderClass::Line(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    D3D.pLine->Begin();
    D3D.pLine->Draw(line, 2, color);
    D3D.pLine->End();
}
void RenderClass::Circle(float x, float y, float rad, bool center, D3DCOLOR color)
{
    const int NUMPOINTS = 34;

    if (!center)
    {
        x -= rad;
        y -= rad;
    }

    D3DTLVERTEX Circle[NUMPOINTS + 1];
    int i;
    float X;
    float Y;
    float Theta;
    float WedgeAngle;
    WedgeAngle = (float)((2 * PI) / NUMPOINTS);
    for (i = 0; i <= NUMPOINTS; i++)
    {
        Theta = i * WedgeAngle;
        X = (float)(x + rad * cos(Theta));
        Y = (float)(y - rad * sin(Theta));
        Circle[i] = CreateD3DTLVERTEX(X, Y, 0.0f, 1.0f, color, 0.0f, 0.0f);
    }
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    pDevice->SetTexture(0, NULL);
    pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &Circle[0], sizeof(Circle[0]));
}

void RenderClass::DrawBox(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR bcolor, int l)
{
    DrawOPX(x, y, w, h, color, l);
    FillRGBA(x, y, w, h, bcolor);
}

void RenderClass::DrawLineBox(int x, int y, int width, int height, D3DCOLOR color)
{
    Line(x, y, x + width, y, color);
    Line(x + width, y, x + width, y + height, color);
    Line(x + width, y + height, x, y + height, color);
    Line(x, y + height, x, y, color);
}

void RenderClass::DrawBorder(int x, int y, int w, int h, int o, D3DCOLOR color)
{
    FillRGBA(x, (y + h - o), w, o, color);
    FillRGBA(x, y, o, h, color);
    FillRGBA(x, y, w, o, color);
    FillRGBA((x + w - o), y, o, h, color);
}

void RenderClass::Draw_ESP_Border(int x, int y, int w, int h, D3DCOLOR Color)
{
    DrawBorder(x, y, w, h, 1, Color);
}