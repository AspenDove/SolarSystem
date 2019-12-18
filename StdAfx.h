// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
											//why u do this...
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"User32.lib")
									
// DirectX Header Files
#include <d3dx9.h>
#include <d3d9.h>
#include <D3dx9math.h>

// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <time.h>
#include <CommCtrl.h>
//#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <limits.h>
#include <math.h>
#include <process.h>

// C++ RunTime Header Files
#include <string>

// Local Header Files
#include "d3dUtility.h"
#include "d3dvertex.h"
#include "constant.h"
#include "Stars.h"
#include "D3DXCamera.h"
#include "SkyBox.h"
#include "D3DXControl.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
