#include "P4DrawManager.h"

#include <d3d9.h>
#include <d3dx9.h>
#include "SqchSDK\SDK.h"

struct D3DVertex
{
	D3DVertex(float _x, float _y, float _z, DWORD _color)
		: x(_x), y(_y), z(_z), color(_color)
	{
	}

	float x;
	float y;
	float z;
	float rhw = 0;
	DWORD color;
};

P4DrawManager::P4DrawManager(IDirect3DDevice9* pDevice)
{
	m_pDevice = pDevice;
	CreateObjects();
}

P4DrawManager::~P4DrawManager()
{
	ReleaseObjects();
}

void P4DrawManager::OnLostDevice()
{
	ReleaseObjects();
}

void P4DrawManager::OnResetDevice()
{
	CreateObjects();
}

void P4DrawManager::ReleaseObjects()
{
	if (m_pStateBlock) m_pStateBlock->Release();
	if (m_pDefaultFont) m_pDefaultFont->Release();
}

void P4DrawManager::CreateObjects()
{
	if (FAILED(m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock))) {
		throw "Failed to create the state block";
	}
	if (FAILED(D3DXCreateFontA(m_pDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &m_pDefaultFont))) {
		throw "Failed to create the default font";
	}
}

void P4DrawManager::BeginRendering()
{
	m_pStateBlock->Capture();


	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

void P4DrawManager::EndRendering()
{
	m_pStateBlock->Apply();
}

void P4DrawManager::RenderLine(DWORD color, int x1, int y1, int x2, int y2)
{
	D3DVertex vertices[2] = {
		D3DVertex(x1, y1, 1.0f, color),
		D3DVertex(x2, y2, 1.0f, color)
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(D3DVertex));
}
void P4DrawManager::RenderRect(DWORD color, int x, int y, int w, int h)
{
	D3DVertex vertices[5] = {
		D3DVertex(x, y, 1.0f, color),
		D3DVertex(x + w, y, 1.0f, color),
		D3DVertex(x + w, y + h, 1.0f, color),
		D3DVertex(x, y + h, 1.0f, color),
		D3DVertex(x, y, 1.0f, color)
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertices, sizeof(D3DVertex));
}
void P4DrawManager::RenderCircle(DWORD color, int x, int y, int r, int resolution)
{
	const double PI = 3.14159265358979323846264;
	float curPointX;
	float curPointY;
	float oldPointX;
	float oldPointY;

	for (int i = 0; i <= resolution; ++i) {
		curPointX = x + r * cos(2 * PI * i / resolution);
		curPointY = y - r * sin(2 * PI * i / resolution);
		if (i > 0) {
			RenderLine(color, curPointX, curPointY, oldPointX, oldPointY);
		}
		oldPointX = curPointX;
		oldPointY = curPointY;
	}
}
void P4DrawManager::RenderText(DWORD color, int x, int y, bool centered, const char* fmt, ...)
{
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	auto drawShadow = [&](RECT rect) {
		rect.left++;
		m_pDefaultFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
		rect.top++;
		m_pDefaultFont->DrawTextA(NULL, buffer, -1, &rect, DT_TOP | DT_LEFT | DT_NOCLIP, 0xFF000000);
	};

	if (centered) {
		RECT rec = { 0,0,0,0 };
		m_pDefaultFont->DrawTextA(NULL, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, color);
		rec = { x - rec.right / 2, y, 0, 0 };

		drawShadow(rec);
		m_pDefaultFont->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, color);

	}
	else {
		RECT rec = { x,y,1000,100 };

		drawShadow(rec);
		m_pDefaultFont->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, color);
	}
}
void P4DrawManager::FillRect(DWORD color, int x, int y, int w, int h)
{
	D3DVertex vertices[4] = {
		D3DVertex(x, y, 1.0f, color),
		D3DVertex(x + w, y, 1.0f, color),
		D3DVertex(x, y + h, 1.0f, color),
		D3DVertex(x + w, y + h, 1.0f, color)
	};
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertices[0], sizeof(D3DVertex));
}
