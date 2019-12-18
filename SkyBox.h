#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "d3dvertex.h"
#ifndef _SKYBOX_
#define _SKYBOX_

class D3DXSkybox
{
private:
	LPDIRECT3DDEVICE9            m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9      m_pVB;
	LPDIRECT3DINDEXBUFFER9       m_pIB;
	LPDIRECT3DTEXTURE9           m_pTextrue[6];
public:
	D3DXSkybox(LPDIRECT3DDEVICE9  g_pD3DDevice);
	~D3DXSkybox(void);
	HRESULT InitSkyBox(float scale);
	void     Render(D3DXVECTOR3 CamPos);
	bool  SetTexture(const LPCWSTR TextureFile, int flag);
};
#endif//_SKYBOX_
