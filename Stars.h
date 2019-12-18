#if	!defined(_STARS_)
#define _STARS_

#pragma once

#include "constant.h"
#include "d3dUtility.h"
#include <d3dx9.h>
#include <d3d9.h>

class SolarSystem
{
public:
};
class FixedStar;
class Planet;
class Moon;

class FixedStar
{
public:
	float m_R;//半径（m）
	float m_AT;//自转周期（s）

	bool bIsTooNear = false;
	//设备指针
	IDirect3DDevice9 *m_d3dDevice;

	ID3DXMesh		   *pMesh = NULL;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXMATRIX         m_matWorld;
	D3DXVECTOR3        m_vPos = { 0.0f, 0.0f, 0.0f };
	//光环
	D3DMATERIAL9            mtlHalo;
	IDirect3DVertexBuffer9* vbHalo;
	IDirect3DTexture9*      pTexHalo;

	FixedStar(){}
	~FixedStar()
	{
		d3d::Release<ID3DXMesh*>(pMesh);
		d3d::Release<LPDIRECT3DTEXTURE9>(pTexture);
		d3d::Release<IDirect3DVertexBuffer9*>(vbHalo);
		d3d::Release<IDirect3DTexture9*>(pTexHalo);
	}
	void CelestialInitialize(IDirect3DDevice9 *d3dDevice, LPCWSTR szTextureName, float fRadius)
	{
		pMesh = d3d::GenerateSphereMesh(d3dDevice, fRadius, 50, 50);
		D3DXCreateTextureFromFile(d3dDevice, szTextureName, &pTexture);
		m_d3dDevice = d3dDevice;
	}
	void HaloInitialize(float fLength = R_SUN * 128 / 4.5f);

	FixedStar(IDirect3DDevice9 *d3dDevice, LPCWSTR szTextureName,
		float R, float AT, bool bHalo) :m_R(R), m_AT(AT)
	{
		//m_d3dDevice = d3dDevice;
		CelestialInitialize(d3dDevice, szTextureName, R);
		if (bHalo)
			HaloInitialize();
	}
	Planet *FixedStar::CreatePlanet(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS);
	void SetSunLight(void);
	void SetPlanetLight(void);
	void SetMaterial(void);

	virtual void SetMatrix(float timeDelta);
	virtual void Render(float timeDelta);
	void HaloRender(D3DXVECTOR3 vPos);
};

class Planet :public FixedStar
{
public:
	float m_A_TRACK;//轨道倾角（rad）与法线的夹角，
	float m_A_AXIS;//自转轴倾角（rad）
	float m_RT;//公转周期（s）
	float m_DIS;//与主转星体距离（m）

	Planet(){}
	Planet(IDirect3DDevice9 *d3dDevice, LPCWSTR szTextureName,
		float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS) :
		FixedStar(d3dDevice, szTextureName, R, AT, false),
		m_DIS(DIS), m_RT(RT), m_A_TRACK(A_TRACK), m_A_AXIS(A_AXIS){}
	Moon *CreateMoon(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS, float T_LINEAXIS, bool bP);

	virtual void SetMatrix(float timeDelta);
	virtual void Render(float timeDelta);
};

class Moon :public Planet
{
public:
	float m_CENTER_DIS;//主转星球的公转距离
	float m_CENTER_RT;//主转星球的公转周期
	float m_T_LINEAXIS;//黄白交线转动周期
	bool m_bPrecession;//交线进动
	Moon(){}
	Moon(IDirect3DDevice9 *d3dDevice, LPCWSTR szTextureName,
		float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS, float CENTER_DIS, float CENTER_RT,float T_LINEAXIS, bool bP) :
		Planet(d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS),
		m_CENTER_DIS(CENTER_DIS), m_CENTER_RT(CENTER_RT),m_T_LINEAXIS(T_LINEAXIS), m_bPrecession(bP){}

	virtual void SetMatrix(float timeDelta);
	virtual void Render(float timeDelta);
};
#endif //_STARS_