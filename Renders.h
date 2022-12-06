#pragma once

/* VC DircetX */
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
/* VC DircetX */

/* D3D */
ID3DXLine* d3dLine;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPD3DXFONT pFont;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
RECT rc2;
/* D3D */

/* HWND */
HWND hWnd;
HWND gWnd;
/* HWND */

#define WHITE            D3DCOLOR_ARGB(255, 255, 255, 255)
#define RED                D3DCOLOR_ARGB(255, 255, 000, 000)
#define GREEN            D3DCOLOR_ARGB(255, 000, 255, 000)
#define BLUE            D3DCOLOR_ARGB(255, 000, 000, 255) 
#define BLACK            D3DCOLOR_ARGB(150, 000, 000, 000)
#define mBLACK            D3DCOLOR_ARGB(100, 000, 000, 205)
#define PURPLE            D3DCOLOR_ARGB(255, 125, 000, 255) 
#define GREY            D3DCOLOR_ARGB(255, 128, 128, 128) 
#define YELLOW            D3DCOLOR_ARGB(255, 255, 255, 000) 
#define ORANGE            D3DCOLOR_ARGB(255, 255, 125, 000)

VOID Cleanup()
{
	d3dLine->Release();
	d3d->Release();
	d3ddev->Release();
}

void DrawLine(float x, float y, float xx, float yy, D3DCOLOR color, float ww)
{
	D3DXVECTOR2 dLine[2];

	d3dLine->SetWidth(ww);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = xx;
	dLine[1].y = yy;

	d3dLine->Draw(dLine, 2, color);
}