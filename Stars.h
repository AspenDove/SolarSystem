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
	float m_R;//�뾶��m��
	float m_AT;//��ת���ڣ�s��

	bool bIsTooNear = false;
	//�豸ָ��
	IDirect3DDevice9 *m_d3dDevice;

	ID3DXMesh		   *pMesh = NULL;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXMATRIX         m_matWorld;
	D3DXVECTOR3        m_vPos = { 0.0f, 0.0f, 0.0f };
	//�⻷
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
	float m_A_TRACK;//�����ǣ�rad���뷨�ߵļнǣ�
	float m_A_AXIS;//��ת����ǣ�rad��
	float m_RT;//��ת���ڣ�s��
	float m_DIS;//����ת������루m��

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
	float m_CENTER_DIS;//��ת����Ĺ�ת����
	float m_CENTER_RT;//��ת����Ĺ�ת����
	float m_T_LINEAXIS;//�ư׽���ת������
	bool m_bPrecession;//���߽���
	Moon(){}
	Moon(IDirect3DDevice9 *d3dDevice, LPCWSTR szTextureName,
		float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS, float CENTER_DIS, float CENTER_RT,float T_LINEAXIS, bool bP) :
		Planet(d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS),
		m_CENTER_DIS(CENTER_DIS), m_CENTER_RT(CENTER_RT),m_T_LINEAXIS(T_LINEAXIS), m_bPrecession(bP){}

	virtual void SetMatrix(float timeDelta);
	virtual void Render(float timeDelta);
};
#endif //_STARS_