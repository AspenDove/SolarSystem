//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <d3dx9.h>
#include <d3d9.h>

#define KEYDOWN(vk_code)  ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)

namespace d3d
{
	bool InitD3D(
		HWND hDeviceWindow,        // [in] Handle to the Window
		HINSTANCE hInstance,       // [in] Application instance.
		int width, int height,     // [in] Backbuffer dimensions.
		bool windowed,             // [in] Windowed (true)or full screen (false).
		D3DDEVTYPE deviceType,     // [in] HAL or REF
		IDirect3DDevice9** device);// [out]The created device.

	D3DMATERIAL9 InitMtrl(
		D3DXCOLOR a,			   // [in] 指定此表面反射环境光数量
		D3DXCOLOR d,			   // [in] 指定此表面反射的漫射光数量
		D3DXCOLOR s,			   // [in] 指定此表面反射的经面广数量
		D3DXCOLOR e,			   // [in] 指定表面的颜色
		float p);				   // [in] 指定高光的锐利值
	
	LPD3DXMESH GenerateSphereMesh(LPDIRECT3DDEVICE9 pDev,
							  	float fRadius,
								UINT slices,
								UINT stacks);

	D3DLIGHT9 InitDirectionalLight(
		D3DXVECTOR3* direction,
		D3DXCOLOR* color);
	
	D3DLIGHT9 InitPointLight(
		const D3DXVECTOR3* position, 
		const D3DXCOLOR* color);

	D3DLIGHT9 InitSpotLight(
		D3DXVECTOR3* position, 
		D3DXVECTOR3* direction, 
		D3DXCOLOR* color);
	
	struct Ray
	{
		D3DXVECTOR3 m_pOrigin;
		D3DXVECTOR3 m_vDirection;
	};
	Ray CalculatePickingRay(IDirect3DDevice9 *pd3dDevice, int xMouse, int yMouse);
	void TransformRay(Ray *ray, D3DXMATRIX *matTrans);
	bool RaySphereIntersect(Ray *ray, D3DXVECTOR3 *pCenter, float *pRadius);
	bool MeshSphereIntersect(IDirect3DDevice9 *pd3dDevice, int xMouse, int yMouse, D3DXVECTOR3 *pCenter, float *pRadius);

	VOID ComputeNormal(D3DXVECTOR3* p0,D3DXVECTOR3* p1,D3DXVECTOR3* p2,D3DXVECTOR3* out);

	int EnterMsgLoop(HWND hDlgModeless, bool(*FrameMove)(float timeDelta),
		bool(*ptr_display)(float timeDelta));

	LRESULT D3DXBitblt(IDirect3DDevice9* device,HDC hDC,int szLength,int szHeight);
	float GetFPS(void);

	const D3DXCOLOR WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR BLACK( D3DCOLOR_XRGB( 0, 0, 0) );
	const D3DXCOLOR RED( D3DCOLOR_XRGB(255, 0, 0) );
	const D3DXCOLOR GREEN( D3DCOLOR_XRGB( 0, 255, 0) );
	const D3DXCOLOR BLUE( D3DCOLOR_XRGB( 0, 0, 255) );
	const D3DXCOLOR YELLOW( D3DCOLOR_XRGB(255, 255, 0) );
	const D3DXCOLOR CYAN( D3DCOLOR_XRGB( 0, 255, 255) );
	const D3DXCOLOR MAGENTA( D3DCOLOR_XRGB(255, 0, 255) );

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK,1e30f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 1e30f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK,1e30f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 1e30f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW,BLACK, 1e30f);
	//const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW,BLACK, 8.0f);

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg, 
		WPARAM wParam,
		LPARAM lParam);	  

	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = NULL;
		}
	}
		
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}
}

#endif // __d3dUtilityH__
