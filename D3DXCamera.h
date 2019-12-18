#ifndef _D3DXCAMERA_
#define _D3DXCAMERA_

#include <d3dx9.h>
class D3DXCamera
{
private:
	D3DXVECTOR3 m_vCameraPos; //相机位置
	D3DXVECTOR3 m_vCameraLook;	 //视线方向
	D3DXVECTOR3 m_vCameraUp;		 //竖直方向
	D3DXVECTOR3 m_vCameraRight;	 //右手方向

	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DXMATRIX m_matView;
	bool bLookAt = false;
	bool bWatchMode = false;

public:
	D3DXCamera(LPDIRECT3DDEVICE9 pd3dDevice) :D3DXCamera(D3DXVECTOR3(0.0f, 0.0f, -5000.0f))
	{
		m_pd3dDevice = pd3dDevice;
		D3DXMatrixIdentity(&m_matView);
	}
	D3DXCamera(D3DXVECTOR3 vPos);
	void SetCameraPos(float x, float y, float z);
	void GetCameraPos(float *x, float *y, float *z);
	D3DXVECTOR3 GetCameraPos(void);
	void SetCameraPos(D3DXVECTOR3 vPos);

	void MoveCameraLR(float fLR);
	void MoveCameraFB(float fFB);
	void MoveCameraUD(float fUD);

	void RotateCameraLR(float fYaw);
	void RotateCameraUD(float fPitch);
	void SlopeCameraLR(float fAngle);

	void CameraLookAtRotating(bool *bAimAt, bool *bWatchAt, D3DXVECTOR3 vPos, float fRadius, float timeDelta);
	void KeyBoardMove(float fMoveRatio, float RotateRatio);
	void SetCamera(D3DXVECTOR3 vPos, D3DXVECTOR3 vLook, D3DXVECTOR3 vUp, D3DXVECTOR3 vRight);
	void SetCamera(void);
};
struct LensFlare
{
	IDirect3DTexture9 *m_pTexFlare;
	float m_fPosition, m_fSize;
	D3DXCOLOR m_crColor;
	D3DXVECTOR3 m_Center;
	LensFlare(){}
	LensFlare(int iId,IDirect3DTexture9* pTexFlare, float fPos, float fSize, D3DXCOLOR crColor) :
		m_pTexFlare(pTexFlare), m_fPosition(fPos), m_fSize(fSize), m_crColor(crColor)
	{
		switch (iId)
		{
		case 0:m_Center = D3DXVECTOR3(16.0f, 16.0f,0.0f); break;
		case 1:m_Center = D3DXVECTOR3(32.0f, 32.0f,0.0f); break;
		case 2:m_Center = D3DXVECTOR3(60.0f, 60.0f,0.0f); break;
		case 3:m_Center = D3DXVECTOR3(75.0f, 75.0f, 0.0f); break;
		}
	}
};
class D3DXCameraLensFlare
{
public:
	IDirect3DDevice9 *m_pd3dDevice;
	IDirect3DTexture9 *m_pTexFlares[5];
	D3DXVECTOR3 m_vLightPos, m_vScrCenterPos, m_vLightScrPos;
	ID3DXSprite *m_pFlare;
	LensFlare *m_pLensFlare;
public:
	D3DXCameraLensFlare(IDirect3DDevice9 *pd3dDevice, int cxClient, int cyClient)
		:m_vScrCenterPos(cxClient / 2.0f, cyClient / 2.0f, 0.0f)
	{
		m_pd3dDevice = pd3dDevice;
		m_vLightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXCreateTextureFromFile(m_pd3dDevice, TEXT("flare1.png"), &m_pTexFlares[0]);
		D3DXCreateTextureFromFile(m_pd3dDevice, TEXT("flare2.png"), &m_pTexFlares[1]);
		D3DXCreateTextureFromFile(m_pd3dDevice, TEXT("flare3.png"), &m_pTexFlares[2]);
		D3DXCreateTextureFromFile(m_pd3dDevice, TEXT("flare4.png"), &m_pTexFlares[3]);

		D3DXCreateSprite(m_pd3dDevice, &m_pFlare);
		m_pLensFlare = new LensFlare[10];
		ZeroMemory(m_pLensFlare, 10 * sizeof(LensFlare));
		InitializeFlares();
	}
	~D3DXCameraLensFlare()
	{
		d3d::Release<ID3DXSprite*>(m_pFlare);
		d3d::Release<IDirect3DTexture9*>(m_pTexFlares[0]);
		d3d::Release<IDirect3DTexture9*>(m_pTexFlares[1]);
		d3d::Release<IDirect3DTexture9*>(m_pTexFlares[2]);
		d3d::Release<IDirect3DTexture9*>(m_pTexFlares[3]);
	};
	void InitializeFlares(void);
	HRESULT ComputeLightScreenPos(void);
	void Render(void);
};

#endif //_D3DXCAMERA_