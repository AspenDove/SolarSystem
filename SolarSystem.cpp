#include "stdafx.h"
#include "resource.h"

// Global Variables:
bool isWindowed = true;
bool bStatistic = false;
int Width = 1200, Height = 1000;
int iTimeK = 10;
IDirect3DDevice9* g_pd3dDevice = 0;

D3DXControl *g_pControl;
D3DXCamera *g_pCamera;
D3DXCameraLensFlare *g_pLensFlare;

FixedStar *g_pSun;
Planet *g_pEarth, *g_pMercury, *g_pVenus, *g_pMars, *g_pJupiter, *g_pSaturn, *g_pUranus, *g_pNeptune;
Moon *g_pMoon;

D3DXSkybox *g_pSkybox;
ID3DXFont* g_pMSYaHei = NULL;

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool Setup(void)
{
	////初始化星球////
	g_pSun     = new FixedStar(g_pd3dDevice, TEXT("texSun.png"), R_SUN, AT_SUN, TRUE);
	g_pEarth   = g_pSun->CreatePlanet(TEXT("texEarth.jpg"), R_EARTH, AT_EARTH, RT_EARTH, DIS_SUN_EARTH, TA_EARTH, AA_EARTH);
	g_pMoon    = g_pEarth->CreateMoon(TEXT("texMoon.jpg"), R_MOON, RT_MOON, RT_MOON, DIS_EARTH_MOON, TA_MOON, AA_MOON, TLA_MOON, true);
	g_pMercury = g_pSun->CreatePlanet(TEXT("texMercury.jpg"), R_MERCURY, AT_MERCURY, RT_MERCURY, DIS_SUN_MERCURY, TA_MERCURY, AA_MERCURY);
	g_pVenus   = g_pSun->CreatePlanet(TEXT("texVenus.jpg"), R_VENUS, AT_VENUS, RT_VENUS, DIS_SUN_VENUS, TA_VENUS, AA_VENUS);
	g_pMars    = g_pSun->CreatePlanet(TEXT("texMars.jpg"), R_MARS, AT_MARS, RT_MARS, DIS_SUN_MARS, TA_MARS, AA_MARS);
	g_pJupiter = g_pSun->CreatePlanet(TEXT("texJupiter.jpg"), R_JUPITER, AT_JUPITER, RT_JUPITER, DIS_SUN_JUPITER, TA_JUPITER, AA_JUPITER);
	g_pSaturn  = g_pSun->CreatePlanet(TEXT("texSaturn.jpg"), R_SATURN, AT_SATURN, RT_SATURN, DIS_SUN_SATURN, TA_SATURN, AA_SATURN);
	g_pUranus  = g_pSun->CreatePlanet(TEXT("texUranus.jpg"), R_URANUS, AT_URANUS, RT_URANUS, DIS_SUN_URANUS, TA_URANUS, AA_URANUS);
	g_pNeptune = g_pSun->CreatePlanet(TEXT("texNeptune.jpg"), R_NEPTUNE, AT_NEPTUNE, RT_NEPTUNE, DIS_SUN_NEPTUNE, TA_NEPTUNE, AA_NEPTUNE);
	//////////////////

	////初始化字体////
	D3DXCreateFont(g_pd3dDevice,
		30, 10, 0, 10, false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,//OUT_TT_ONLY_PRECIS,
		DRAFT_QUALITY, 0,
		TEXT("微软雅黑"), &g_pMSYaHei);
	//////////////////

	////初始化控制对象////
	g_pControl = new D3DXControl(g_pd3dDevice);
	/////////////////////

	////初始化摄像机////
	g_pCamera = new D3DXCamera(g_pd3dDevice);
	g_pCamera->SetCamera();

	g_pLensFlare = new D3DXCameraLensFlare(g_pd3dDevice, isWindowed ? Width : GetSystemMetrics(SM_CXSCREEN),
		isWindowed ? Height : GetSystemMetrics(SM_CYSCREEN));
	///////////////////

	////初始化天空盒////
	g_pSkybox = new D3DXSkybox(g_pd3dDevice);
	///////////////////

	////设置渲染状态////
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, false);//镜面反射
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	 //不裁剪表面
	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	//纹理
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//寻址模式
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 10);
	////////////////////
	return true;
}

void Cleanup(void)
{
	delete g_pSun;
	delete g_pEarth;
	delete g_pMoon;
}

bool FrameMove(float timeDelta)
{
	if (!g_pd3dDevice)return false;

	////设置投影矩阵////
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj, g_pControl->m_fFov, isWindowed ? ((float)Width / (float)Height) :
		((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 1.0f, FLT_MAX);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	////////////////////

	if (KEYDOWN(VK_F1))
	{
		g_pControl->m_Target = SUN;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F2))
	{
		g_pControl->m_Target = MERCURY;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F3))
	{
		g_pControl->m_Target = VENUS;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F4))
	{
		g_pControl->m_Target = EARTH;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F5))
	{
		g_pControl->m_Target = MARS;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F6))
	{
		g_pControl->m_Target = JUPITER;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F7))
	{
		g_pControl->m_Target = SATURN;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F8))
	{
		g_pControl->m_Target = URANUS;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F9))
	{
		g_pControl->m_Target = NEPTUNE;
		g_pControl->m_bMoveTo = true;
	}
	else if (KEYDOWN(VK_F10))
	{
		g_pControl->m_Target = MOON;
		g_pControl->m_bMoveTo = true;
	}

	//if (KEYDOWN(VK_F2))g_pControl->m_bSkybox = !g_pControl->m_bSkybox;//g_pCamera->SetCameraPos(g_pMoon->m_vPos);
	g_pCamera->KeyBoardMove(g_pControl->m_fMoveRatio, g_pControl->m_fRotateRatio);
	g_pCamera->SetCamera();
	switch (g_pControl->m_Target)
	{
	case EARTH:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pEarth->m_vPos, g_pEarth->m_R, timeDelta);	break;
	case VENUS:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pVenus->m_vPos, g_pVenus->m_R, timeDelta);	break;
	case MOON:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pMoon->m_vPos, g_pMoon->m_R, timeDelta); break;
	case SUN:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pSun->m_vPos, g_pSun->m_R, timeDelta);	break;
	case MERCURY:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pMercury->m_vPos, g_pMercury->m_R, timeDelta); break;
	case MARS:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pMars->m_vPos, g_pMars->m_R, timeDelta); break;
	case JUPITER:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pJupiter->m_vPos, g_pJupiter->m_R, timeDelta); break;
	case SATURN:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pSaturn->m_vPos, g_pSaturn->m_R, timeDelta); break;
	case URANUS:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pUranus->m_vPos, g_pUranus->m_R, timeDelta); break;
	case NEPTUNE:
		g_pCamera->CameraLookAtRotating(&g_pControl->m_bAimAt, &g_pControl->m_bMoveTo, g_pNeptune->m_vPos, g_pNeptune->m_R, timeDelta); break;
	default://nothing...
		break;
	}

	return true;
}

bool Display(float timeDelta)
{
	static RECT rc;
	rc.left = rc.top = 0;
	rc.right = isWindowed ? Width : GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = isWindowed ? Height : GetSystemMetrics(SM_CYSCREEN);

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(g_pd3dDevice->BeginScene())) return false;

	////该死的天空盒！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	if (g_pControl->m_bSkybox)
		g_pSkybox->Render(g_pCamera->GetCameraPos());//渲染天空盒，传递camera的参数

	if (bStatistic)
	{
		////显示帧数////
		static TCHAR szText[10];
		int iCount = swprintf_s(szText, TEXT("%0.0f"), d3d::GetFPS());
		g_pMSYaHei->DrawText(NULL, szText, iCount, &rc, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255, 255, 255));
		////////////////

		////显示信息////
		g_pControl->ShowData(g_pMSYaHei, rc);
		///////////////
	}
	////渲染星球////
	//int iTimeK = 10;
	g_pSun->HaloRender(g_pCamera->GetCameraPos());
	g_pSun->Render(timeDelta * iTimeK);
	
	g_pMercury->Render(timeDelta * iTimeK);
	g_pVenus->Render(timeDelta * iTimeK);
	g_pEarth->Render(timeDelta * iTimeK);g_pMoon->Render(timeDelta * iTimeK);
	g_pMars->Render(timeDelta * iTimeK);
	g_pJupiter->Render(timeDelta * iTimeK);
	g_pSaturn->Render(timeDelta * iTimeK);
	g_pUranus->Render(timeDelta * iTimeK);
	g_pNeptune->Render(timeDelta * iTimeK);
	///////////////

	g_pLensFlare->Render();

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	return true;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK d3d::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInst;
	int wmId, wmEvent;
	int xMouse, yMouse;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		//case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		//EndPaint(hWnd, &ps);
		//	break;
	case WM_LBUTTONDOWN:
		xMouse = LOWORD(lParam);//GET_X_LPARAM(lParam);
		yMouse = HIWORD(lParam); //GET_Y_LPARAM(lParam);
		//if (d3d::MeshSphereIntersect(g_pd3dDevice, xMouse, yMouse, &g_pSun->m_vPos, &g_pSun->m_R))
		//	MessageBox(hWnd, TEXT("太阳"), NULL, NULL);
		if (d3d::MeshSphereIntersect(g_pd3dDevice, xMouse, yMouse, &g_pEarth->m_vPos, &g_pEarth->m_R))
			g_pControl->m_bAimAt = true;
		//	if (d3d::MeshSphereIntersect(g_pd3dDevice,xMouse, yMouse, &g_pMoon->m_vPos, &g_pMoon->m_R))
		//		g_pControl->m_bAimAt = true;
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_INSERT)
			bStatistic = !bStatistic;
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		if (wParam == VK_PRIOR)g_pControl->AdjustMoveRatio(0.01F);
		if (wParam == VK_NEXT)g_pControl->AdjustMoveRatio(-0.01F);
		if (wParam == VK_HOME)g_pControl->AdjustRotateRatio(0.001F);
		if (wParam == VK_END)g_pControl->AdjustRotateRatio(-0.001F);
		break;
	case WM_MOUSEWHEEL:
		if (GetKeyState(VK_LSHIFT) & (short)0xF0)
		{
			if ((short)HIWORD(wParam) >= 0)
				iTimeK += 5.f;
			else
				iTimeK -= 5.f;
			break;
		}
		else
		{
			if ((short)HIWORD(wParam) >= 0)
				g_pControl->AdjustFov(D3DX_PI*0.01f);
			else
				g_pControl->AdjustFov(-D3DX_PI*0.01f);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}