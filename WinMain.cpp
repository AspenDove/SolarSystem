#include "stdafx.h"
#include "resource.h"

extern D3DXControl *g_pControl;
extern bool isWindowed;
extern int Width, Height;
extern IDirect3DDevice9* g_pd3dDevice;
bool Setup(void);
void Cleanup(void);
bool FrameMove(float timeDelta);
bool Display(float timeDelta);
DWORD WINAPI WelcomeThread(PVOID pvParam);

HINSTANCE g_hInst;							

typedef unsigned(__stdcall *PTHREAD_START) (void*);
#define BeginThread(psa,cbStack,pfnStartAddr,       \
					pvParam,fdwCreate,pdwThreadID)  \
					((HANDLE)_beginthreadex(        \
					(void*)(psa),                   \
					(unsigned)(cbStack),            \
					((PTHREAD_START)(pfnStartAddr)),\
					(void*)(pvParam),               \
					(unsigned)(fdwCreate),          \
					(unsigned*)(pdwThreadID)))

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	HANDLE hTmp = CreateMutex(NULL, FALSE, TEXT("{TG45TER-54U356-GU-H564UU-G54G}"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("太阳系已经在运行！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
		CloseHandle(hTmp);
		return EXIT_FAILURE;
	}
	else;
	//创建信号量使欢迎窗口和主窗口线程同步
	HANDLE hSemaphore = CreateSemaphore(NULL, 0, 2, TEXT("solarsystem-setup-ready"));

	HWND hDeviceWindow = NULL;
	g_hInst = hInstance;
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = /*CS_HREDRAW | CS_VREDRAW | */CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOLARSYSTEM));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIconSm = NULL;//TO FIND A DEFAULT ONE
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;//(PTCHAR)IDC_SOLARSYSTEM;
	wc.lpszClassName = TEXT("SolarSystem");

	if (!RegisterClassEx(&wc))
	{
		::MessageBox(0, TEXT("RegisterClass() - FAILED"), 0, 0);
		return false;
	}
	hDeviceWindow = ::CreateWindowEx(
		NULL,//0x00080000,//WS_EX_LAYERED ,
		TEXT("SolarSystem"),
		TEXT("SolarSystem"),
		WS_POPUP | WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2 + GetSystemMetrics(SM_CXDLGFRAME),
		GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2 + GetSystemMetrics(SM_CYICON) + GetSystemMetrics(SM_CYDLGFRAME),
		Width, Height,
		NULL, NULL, hInstance, NULL);

	if (!hDeviceWindow)
	{
		::MessageBox(0, TEXT("CreateWindow() - FAILED"), 0, 0);
		return false;
	}
	HWND *phWelcome = new HWND;
	HANDLE hThread = BeginThread(NULL, NULL, WelcomeThread, (void*)phWelcome, NULL, NULL);

	if (!d3d::InitD3D(hDeviceWindow, hInstance,
		Width, Height, isWindowed, D3DDEVTYPE_HAL, &g_pd3dDevice))
	{
		TerminateThread(hThread, NULL);//出错，立即关闭线程
		::MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);
		return 0;
	}

	if (!Setup())
	{
		TerminateThread(hThread, NULL);//出错，立即关闭线程
		::MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);
		return 0;
	}

	//资源加载完毕
	DWORD dwExitCode = NULL;
	GetExitCodeThread(hThread, &dwExitCode);
	if (dwExitCode == EXIT_FAILURE)//线程出了问题
	{
		CloseHandle(hThread);
		CloseHandle(hSemaphore);
		return EXIT_FAILURE;
	}
	
	WaitForSingleObject(hSemaphore, INFINITE);
	ShowWindow(*phWelcome, SW_FORCEMINIMIZE);
	ShowWindow(hDeviceWindow, SW_SHOW);
	UpdateWindow(hDeviceWindow);
	SetForegroundWindow(hDeviceWindow);
	SetFocus(hDeviceWindow);

	PSTMSG(*phWelcome, WM_CLOSE, (WPARAM)NULL, (LPARAM)NULL);
	DestroyWindow(*phWelcome);
	CloseHandle(hThread);
	CloseHandle(hSemaphore);

	/*HWND hForegdWnd = GetForegroundWindow();
	DWORD dwCurID = GetCurrentThreadId();
	DWORD dwForeID = GetWindowThreadProcessId(hForegdWnd, NULL);
	AttachThreadInput(dwCurID, dwForeID, TRUE);
	SetForegroundWindow(hDeviceWindow);
	AttachThreadInput(dwCurID, dwForeID, FALSE);*/

	/*AttachThreadInput(GetWindowThreadProcessId(GetForegroundWindow(), NULL),
		GetCurrentThreadId(), TRUE);
	SetForegroundWindow(hDeviceWindow);
	SetFocus(hDeviceWindow);
	AttachThreadInput(GetWindowThreadProcessId(GetForegroundWindow(), NULL),
		GetCurrentThreadId(), FALSE);*/
	d3d::EnterMsgLoop(NULL, FrameMove, Display);

	Cleanup();

	g_pd3dDevice->Release();

	return 0;

}
LRESULT CALLBACK WelcomeProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE  hInstance;
	static HDC        hdcMem, hdc;
	static HBITMAP    hBitmap;
	HRGN              hRgn;
	PAINTSTRUCT       ps;
	switch (message)
	{
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hRgn = CreateRoundRectRgn(0, 0, 896, 404, 118, 70);
		SetWindowRgn(hWnd, hRgn, TRUE);
		DeleteObject(hRgn);
		hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_WELCOME));
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);
		return 0;
	case WM_NCHITTEST:
		UINT nHitTest;
		nHitTest = DefWindowProc(hWnd, message, wParam, lParam);

		if (nHitTest == HTCLIENT && GetAsyncKeyState(MK_LBUTTON) < 0)
			nHitTest = HTCAPTION;
		return nHitTest;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, 896, 404, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_NCDESTROY:
	case WM_DESTROY:
		DeleteDC(hdcMem);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD WINAPI WelcomeThread(PVOID pvParam)
{
	HWND         hWndWelcome;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 4;
	wndclass.hInstance = g_hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("welcome");
	wndclass.lpfnWndProc = WelcomeProc;

	RegisterClass(&wndclass);
	hWndWelcome = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, TEXT("welcome"), TEXT("Solarsystem"), WS_POPUP | WS_VISIBLE,
		GetSystemMetrics(SM_CXSCREEN) / 2 - 448, GetSystemMetrics(SM_CYSCREEN) / 2 - 202, 896, 404,
		NULL, NULL, g_hInst, NULL);
	if (!hWndWelcome)
	{
		MessageBox(hWndWelcome, TEXT("创建欢迎窗口失败！"), TEXT("错误！"), MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}
	*((HWND*)(pvParam)) = hWndWelcome;//保存窗口句柄
	//我已创建完毕
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, TEXT("solarsystem-setup-ready"));
	ReleaseSemaphore(hSemaphore, 1, NULL);

	if (!SetLayeredWindowAttributes(hWndWelcome, NULL, 100, LWA_ALPHA))
	{
		MessageBox(hWndWelcome, TEXT("SetLayeredWindowAttributes调用失败！"), TEXT("错误！"), MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

	ShowWindow(hWndWelcome, SW_SHOWNORMAL);
	UpdateWindow(hWndWelcome);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}