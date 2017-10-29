#include "Headers.h"
#include "DevDeclares.h"

DirectXHook D3D;

cVars CVars;
extern cTools Tools;
cMenu* pMenu;
cESP ESP;

extern "C" LPVOID _ReturnAddress();

unsigned long uiStride;
LPDIRECT3DVERTEXBUFFER9 StreamData;
UINT OffsetInBytes;
UINT m_Stride;

bool bInit = false;
bool bRunOnce = false;

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
    if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
        return E_FAIL;

    WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
        | (WORD)(((colour32 >> 20) & 0xF) << 8)
        | (WORD)(((colour32 >> 12) & 0xF) << 4)
        | (WORD)(((colour32 >> 4) & 0xF) << 0);

    D3DLOCKED_RECT d3dlr;
    (*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
    WORD *pDst16 = (WORD*)d3dlr.pBits;

    for (int xy = 0; xy < 8 * 8; xy++)
        *pDst16++ = colour16;

    (*ppD3Dtex)->UnlockRect(0);

    return S_OK;
}

void MenuItems()
{
    pMenu->AddGroup("[視覺]", &CVars.Mvisible1, opt_Grp);
    if (CVars.Mvisible1)
    {
        pMenu->AddItem("目標選擇", &CVars.Targets, sESPTarget, 2);
        pMenu->AddItem("可見檢測", &CVars.VisCheck, opt_OFFON);
        pMenu->AddItem("2D方塊", &CVars.Box2DESP, opt_OFFON);
        pMenu->AddItem("3D方塊", &CVars.Box3DESP, opt_OFFON);
        pMenu->AddItem("骨骼線", &CVars.BoneLineESP, opt_OFFON);
        pMenu->AddItem("骨骼點", &CVars.BonePointESP, opt_OFFON);
        pMenu->AddItem("顯示血條", &CVars.DrawHealth, sHealth, 4);
        pMenu->AddItem("顯示名字", &CVars.NameESP, opt_OFFON);
        pMenu->AddItem("顯示距離", &CVars.DistanceESP, opt_OFFON);
        pMenu->AddItem("顯示武器", &CVars.WeaponESP, opt_OFFON);
        pMenu->AddItem("顯示連線", &CVars.LineESP, opt_OFFON);
        pMenu->AddItem("顯示等級", &CVars.RankESP, opt_OFFON);
        pMenu->AddItem("顯示C4", &CVars.C4ESP, opt_OFFON);
        pMenu->AddItem("顯示QQ號", &CVars.QQESP, opt_OFFON);
        pMenu->AddItem("雷達", &CVars.Radar2D, sRadar, 3);
        pMenu->AddItem("准星", &CVars.DrawXHair, opt_OFFON);
        //  pMenu->AddItem("狀態信息",  &CVars.StatsBox, opt_OFFON);
    }

    pMenu->AddGroup("[透視]", &CVars.Mvisible2, opt_Grp);
    if (CVars.Mvisible2)
    {
        pMenu->AddItem("人物高亮", &CVars.HighLight, opt_OFFON);    //
        pMenu->AddItem("人物全白", &CVars.PlayerWhite, opt_OFFON);  //
        pMenu->AddItem("人物線條", &CVars.PlayerFrame, opt_OFFON);  //
        //  pMenu->AddItem("去除煙霧", &CVars.NoSmoke, opt_OFFON);
        //  pMenu->AddItem("去除閃光", &CVars.NoFlash, opt_OFFON);
        pMenu->AddItem("去除世界", &CVars.NoWorld, opt_OFFON);  //
        pMenu->AddItem("去除天空", &CVars.NoSky, opt_OFFON);    //
        pMenu->AddItem("白色牆壁", &CVars.WhiteWall, opt_OFFON);    //
    }

    pMenu->AddGroup("[自瞄]", &CVars.Mvisible3, opt_Grp);
    if (CVars.Mvisible3)
    {
        pMenu->AddItem("自瞄開關", &CVars.Aimbot, opt_OFFON);
        pMenu->AddItem("自瞄位置", &CVars.AimBoneId, sAimSlot, 4);
        pMenu->AddItem("自瞄鍵", &CVars.AimKey, sAimKey, 6);
    }

    pMenu->AddGroup("[關于]", &CVars.Mvisible12, opt_Grp);
    if (CVars.Mvisible12)
    {
        pMenu->AddItem("仅供学习", &CVars.StatsBox, opt_OFFON);
    }
}

void MenuDrawing()
{
    if (pMenu == 0) {
        pMenu = new cMenu(NULL, 390, 134);
        pMenu->visible = 1;
        pMenu->col_title = TextWhite;

    }
    else {
        if (pMenu->noitems == 0) MenuItems();

        pMenu->Show();
        pMenu->Nav();

    }
}

void pCreateLine(LPDIRECT3DDEVICE9 pDevice)
{
    bool bLine = false;
    if (D3D.pLine)
    {
        D3D.pLine->Release();
        D3D.pLine = NULL;
        bLine = false;
    }

    if (!bLine)
    {
        D3DXCreateLine(pDevice, &D3D.pLine);
        bLine = true;
    }
}
void pCreateFont(LPDIRECT3DDEVICE9 pDevice)
{
    bool bFont = false;
    if (D3D.pFont)
    {
        D3D.pFont->Release();
        D3D.pFont = NULL;
        bFont = false;
    }

    if (!bFont)
    {
        D3DXCreateFont(pDevice, 15, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &D3D.pFont);
        bFont = true;
    }
}
HRESULT WINAPI nEndScene(LPDIRECT3DDEVICE9 pDevice)
{   
    _asm pushad;
    Render.GetDevice(pDevice);
    pDevice->GetViewport(&D3D.Viewport);
    pCreateFont(pDevice);
    pCreateLine(pDevice);

    MenuDrawing();

    ESP.Console();
    ESP.DrawESP(pDevice);
    ESP.DrawMiscESP(pDevice);
    ESP.PlayerAimbot(pDevice);

    Render.DrawString(0, 0, ColRed, 0, D3D.pFont, "仅供学习");

    if (CVars.DrawXHair)
    {
        Render.FillRGBA(D3D.Viewport.Width / 2 - 14 - 2, D3D.Viewport.Height / 2 - 2, 9, 1, D3DCOLOR_ARGB(200, 0, 255, 0));
        Render.FillRGBA(D3D.Viewport.Width / 2 + 5 - 2, D3D.Viewport.Height / 2 - 2, 9, 1, D3DCOLOR_ARGB(200, 0, 255, 0));
        Render.FillRGBA(D3D.Viewport.Width / 2 - 2, D3D.Viewport.Height / 2 - 14 - 2, 1, 9, D3DCOLOR_ARGB(200, 0, 255, 0));
        Render.FillRGBA(D3D.Viewport.Width / 2 - 2, D3D.Viewport.Height / 2 + 5 - 2, 1, 9, D3DCOLOR_ARGB(200, 0, 255, 0));
        Render.FillRGBA(D3D.Viewport.Width / 2 - 2, D3D.Viewport.Height / 2 - 2, 1, 1, D3DCOLOR_ARGB(200, 0, 255, 0));
    }
    OutputDebugString("3");
    _asm popad;
    return pEndScene(pDevice);
}


DWORD* FindDevice(DWORD Base)
{
    for (long i = 0, n = 0; i < 0x128000; i++)
    {
        if (*(BYTE *)(Base + i + 0x00) == 0xC7)n++;
        if (*(BYTE *)(Base + i + 0x01) == 0x06)n++;
        if (*(BYTE *)(Base + i + 0x06) == 0x89)n++;
        if (*(BYTE *)(Base + i + 0x07) == 0x86)n++;
        if (*(BYTE *)(Base + i + 0x0C) == 0x89)n++;
        if (*(BYTE *)(Base + i + 0x0D) == 0x86)n++;

        if (n == 6) return (DWORD*)
            (Base + i + 2); n = 0;
    }
    return(0);
}

LPDIRECT3DDEVICE9 nm_pD3Ddev;
PBYTE HookVTable(PDWORD* dwVTable, PBYTE dwHook, INT Index)
{
    DWORD dwOld = 0;
    VirtualProtect((void*)((*dwVTable) + (Index * 4)), 4, PAGE_EXECUTE_READWRITE, &dwOld);
    PBYTE pOrig = ((PBYTE)(*dwVTable)[Index]);
    (*dwVTable)[Index] = (DWORD)dwHook;
    VirtualProtect((void*)((*dwVTable) + (Index * 4)), 4, dwOld, &dwOld);

    return pOrig;
}
LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return DefWindowProc(hwnd, uMsg, wParam, lParam); }
void DX_Init(DWORD* table)
{
    WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
    RegisterClassExA(&wc);
    HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    LPDIRECT3DDEVICE9 pd3dDevice;
    pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);

    DWORD* pVTable = (DWORD*)pd3dDevice;
    pVTable = (DWORD*)pVTable[0];

    table[0] = pVTable[42];

    DestroyWindow(hWnd);
}

DWORD WINAPI VMT_Patching(LPVOID  Param)
{
    HookVTable((PDWORD*)nm_pD3Ddev, (PBYTE)nEndScene, 42);
    return 1;
}


bool hooked = false;

void LoopFunction()
{
    if (hooked == false)
    {
        DWORD VTable[3] = { 0 };
        while (GetModuleHandle("d3d9.dll") == NULL)
        {
            Sleep(250);
        }
        DX_Init(VTable);

        HOOK(EndScene, VTable[0]);
        while (!nm_pD3Ddev)
        {
            Sleep(50);
        }
        UNHOOK(EndScene, VTable[0]);
        *(PDWORD)&pEndScene = VTable[0];
        CreateThread(NULL, 0, &VMT_Patching, NULL, 0, NULL);
        hooked = true;
    }
}

void DirectXHook::Initialize()
{
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LoopFunction, NULL, NULL, NULL);
}
