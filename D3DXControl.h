#if	!defined(_D3DXCONTROL_)
#define _D3DXCONTROL_

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define SUN     1000
#define MERCURY 1001
#define EARTH   1002
#define VENUS   1003
#define MARS    1004
#define JUPITER 1005
#define SATURN  1006
#define URANUS  1007
#define NEPTUNE 1008
#define MOON    1009

class D3DXControl
{
public:
	IDirect3DDevice9 *m_pd3dDevice;
	float m_fRadius;
	float m_fFov;
	float m_fMoveRatio;
	float m_fRotateRatio;

	bool m_bAimAt = false;
	bool m_bMoveTo = false;

	bool m_bSkybox = true;
	bool m_bflare = false;
	int m_Target;
	D3DXControl(IDirect3DDevice9 *pd3dDevice) :m_fRadius(0.0f), m_fFov(D3DX_PI/3.0f)
	{
		m_fMoveRatio = 3.0f;
		m_fRotateRatio = 0.02f;

		m_pd3dDevice = pd3dDevice;
	}
	
	void AdjustMoveRatio(float delta);
	void AdjustRotateRatio(float delta);
	void AdjustFov(float delta);
	void ShowData(ID3DXFont* pFont,RECT rc);
};

#endif //_D3DXCONTROL_