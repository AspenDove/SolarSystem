//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.cpp
// 
// Author: Sun (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

float g_FPS = 0.1f;

/*typedef BOOL (FAR WINAPI*LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);

BOOL SetLayeredWindowAttributes(
HWND hwnd,           // handle to the layered window
COLORREF crKey,      // specifies the color key
BYTE bAlpha,         // value for the blend function
DWORD dwFlags        // action
)
{
LAYERFUNC SetLayer;
HMODULE hMod = LoadLibrary(TEXT("user32.dll"));
SetLayer = (LAYERFUNC)GetProcAddress(hMod,"SetLayeredWindowAttributes");
BOOL bReturn = SetLayer(hwnd,crKey,bAlpha,dwFlags);
return bReturn;
}*/

bool d3d::InitD3D(
	HWND hDeviceWindow,
	HINSTANCE hInstance,
	int width, int height,
	bool windowed,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device)
{
	//
	// Init D3D: 
	//
	D3DDISPLAYMODE displayMode;
	HRESULT hr = 0;

	//创建D3D对象
	IDirect3D9* d3d9 = 0;
	if (NULL == (d3d9 = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		::MessageBox(0, TEXT("Direct3DCreate9() - FAILED"), 0, 0);
		return false;
	}

	// Get the desktop display mode.
	if (FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	// 默认不使用多采样
	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
	DWORD dwMultiSampleQuality = 0;
	
	// 检查是否支持4倍速率采样
	/*if (d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format,
		!windowed,D3DMULTISAMPLE_2_SAMPLES,&dwMultiSampleQuality) == D3D_OK)
		multiType = D3DMULTISAMPLE_2_SAMPLES;// 保存多采样类型
	*/
	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = windowed ? width : GetSystemMetrics(SM_CXSCREEN);
	d3dpp.BackBufferHeight = windowed ? height : GetSystemMetrics(SM_CYSCREEN);
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = multiType;
	d3dpp.MultiSampleQuality = dwMultiSampleQuality;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;// //D3DSWAPEFFECT_FLIP;//
	d3dpp.hDeviceWindow = hDeviceWindow;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	//D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hDeviceWindow,      // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hDeviceWindow,
			vp,
			&d3dpp,
			device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	return true;
}

LRESULT d3d::D3DXBitblt(IDirect3DDevice9* device, HDC hDCDes, int szWidth, int szHeight)
{
	LRESULT hr = true;

	LPDIRECT3DSURFACE9 surfTemp, surfDC;
	if (FAILED(hr = device->CreateOffscreenPlainSurface(szWidth, szHeight, D3DFMT_X8R8G8B8,
		D3DPOOL_SYSTEMMEM, &surfDC, NULL)))
	{
		surfDC->Release();
		return hr;
	}

	if (FAILED(hr = device->GetBackBuffer(NULL, NULL, D3DBACKBUFFER_TYPE_MONO, &surfTemp)))
	{
		surfTemp->Release();
		return hr;
	}

	if (FAILED(hr = device->GetRenderTargetData(surfTemp, surfDC)))
	{
		return hr;
	}
	HDC hDC;

	if (FAILED(hr = surfDC->GetDC(&hDC)))
	{
		return hr;
	}

	if (FAILED(hr = BitBlt(hDCDes, 0, 0, szWidth, szHeight, hDC, 0, 0, SRCCOPY)))
	{
		return hr;
	}

	surfTemp->Release();
	surfDC->Release();
	return hr;
}

LPD3DXMESH d3d::GenerateSphereMesh(LPDIRECT3DDEVICE9 pDev,
	float fRadius, UINT uSlices, UINT uStacks)
{
	////创建D3D球体////
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDev, fRadius, uSlices, uStacks, &mesh, NULL)))
	{
		return NULL;
	}
	//////////////////	

	////复制球体网格数据////
	LPD3DXMESH texMesh;
	if (FAILED(mesh->CloneMeshFVF(D3DXMESH_MANAGED
		/*D3DXMESH_SYSTEMMEM*/, VERTEX::FVF, pDev, &texMesh)))
	{
		return NULL;
	}
	//释放原有网格		  
	mesh->Release();
	////////////////////////

	////添加纹理数据////
	VERTEX* pVerts = NULL;
	if (SUCCEEDED(texMesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&pVerts)))
	{
		//获取顶点数量
		int numVerts = texMesh->GetNumVertices();
		for (int i = 0; i < numVerts; ++i)
		{
			//计算纹理坐标
			pVerts->m_u = 0.5f + atan2(pVerts->m_Normal.z, pVerts->m_Normal.x) / 2.0f / D3DX_PI;
			pVerts->m_v = 0.5f - asin(pVerts->m_Normal.y) / D3DX_PI;

			++pVerts;
		}
		//解锁顶点缓冲区
		texMesh->UnlockVertexBuffer();
	}
	////////////////////
	return texMesh;
}

float d3d::GetFPS(void)
{
	static float fps = 0;//fps值
	static int frameCount = 0;//帧数
	static float currentTime = 0.0f;//当前时间
	static float lastTime = 0.0f;//持续时间

	frameCount++;
	currentTime = timeGetTime()*0.001f;//单位是秒

	if (currentTime - lastTime > 0.1f)
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}
	return fps;
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;
	return light;
}

D3DLIGHT9 d3d::InitPointLight(const D3DXVECTOR3* position,const D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	//light.Ambient = *color * 0.6f;
	light.Diffuse = *color;
	//light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Range = 20000000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.0f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.4f;
	light.Phi = 0.9f;

	return light;
}

VOID d3d::ComputeNormal(D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2, D3DXVECTOR3* out)
{		//顺时针
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p0;
	D3DXVec3Cross(out, &u, &v);
	D3DXVec3Normalize(out, out);

	return;
}

int d3d::EnterMsgLoop(HWND hDlgModeless,bool(*FrameMove)(float timeDelta),
	bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (hDlgModeless == NULL || !IsDialogMessage(hDlgModeless, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			
			FrameMove(timeDelta);
			ptr_display(timeDelta);

			lastTime = currTime;
		}
	}
	return msg.wParam;
}

/************************************************************************/
/* 功能：计算拾取射线                                                    */
/* 参数：xMouse,yMouse 鼠标的x,y屏幕坐标                                 */
/************************************************************************/
d3d::Ray d3d::CalculatePickingRay(IDirect3DDevice9 *pd3dDevice, int xMouse, int yMouse)
{
	D3DVIEWPORT9 viewport;
	pd3dDevice->GetViewport(&viewport);

	D3DXMATRIX matProj;
	pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	float fX = (((2.0f*xMouse) / viewport.Width) - 1.0f) / matProj(0, 0);
	float fY = (((-2.0f*yMouse) / viewport.Height) + 1.0f) / matProj(1, 1);

	return{ D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(fX, fY, 1.0f) };
}

/************************************************************************/
/* 功能：通过矩阵变换射线                                                */
/* 参数：ray 需要变换的射线 matTrans 变换矩阵                             */
/************************************************************************/
void d3d::TransformRay(Ray *ray, D3DXMATRIX *matTrans)
{
	//w=1 变换原点
	D3DXVec3TransformCoord(&ray->m_pOrigin, &ray->m_pOrigin, matTrans);

	//w=0 变换向量
	D3DXVec3TransformNormal(&ray->m_vDirection, &ray->m_vDirection, matTrans);

	//单位化向量
	D3DXVec3Normalize(&ray->m_vDirection, &ray->m_vDirection);
}

/************************************************************************/
/* 功能：判断射线是否穿过边界球                                           */
/* 参数：ray 需要判断的射线 pCenter 边界球中心 pRadius 边界球半径          */
/* 实际上解方程： |p0 + t*u - c| = r                                     */
/************************************************************************/
bool d3d::RaySphereIntersect(Ray *ray, D3DXVECTOR3 *pCenter, float *pRadius)
{
	D3DXVECTOR3 v = ray->m_pOrigin - *pCenter;

	float b = 2.0f * D3DXVec3Dot(&ray->m_vDirection, &v);
	float c = D3DXVec3Dot(&v, &v) - ((*pRadius)*(*pRadius));

	float fDiscriminant = (b*b) - (4.0f*c);

	if (fDiscriminant < 0.0f)return false;

	fDiscriminant = sqrt(fDiscriminant);

	float fs0 = (-b - fDiscriminant) / 2.0f;
	float fs1 = (-b + fDiscriminant) / 2.0f;

	return (fs0 >= 0.0f || fs1 >= 0.0f);
}

/************************************************************************/
/* 功能：通过屏幕鼠标位置判断网格是否穿过边界球                            */
/* 参数：xMouse，xMouse鼠标屏幕坐标 pCenter中心坐标 pRadius半径           */
/************************************************************************/
bool d3d::MeshSphereIntersect(IDirect3DDevice9 *pd3dDevice,int xMouse, int yMouse, D3DXVECTOR3 *pCenter, float *pRadius)
{
	d3d::Ray ray = d3d::CalculatePickingRay(pd3dDevice,xMouse, yMouse);

	D3DXMATRIX matView;
	pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, NULL, &matView);

	TransformRay(&ray, &viewInverse);
	return RaySphereIntersect(&ray, pCenter, pRadius);
}


