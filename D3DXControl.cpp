#include "StdAfx.h"

/************************************************************************/
/* 功能：调整移动比例                                                    */
/* 参数：delta 整数为增加 负数为减小                                     */
/************************************************************************/
void D3DXControl::AdjustMoveRatio(float delta)
{
	m_fMoveRatio = max(m_fMoveRatio + delta, 0.0f);
}

/************************************************************************/
/* 功能：调整旋转比例                                                    */
/* 参数：delta 整数为增加 负数为减小                                      */
/************************************************************************/
void D3DXControl::AdjustRotateRatio(float delta)
{
	m_fRotateRatio = max(min(m_fRotateRatio + delta, 2 * D3DX_PI), 0.0f);
}

/************************************************************************/
/* 功能：调整视角大小                                                      */
/* 参数：delta 整数为增加 负数为减小                                        */
/************************************************************************/
void D3DXControl::AdjustFov(float delta)
{
	m_fFov = max(min(m_fFov + delta, 0.3f * D3DX_PI), 0.01f*D3DX_PI);
}

/************************************************************************/
/* 功能：显示各种信息                                                      */
/* 参数：pFont 字体 rc 屏幕矩形                                            */
/************************************************************************/
void D3DXControl::ShowData(ID3DXFont* pFont,RECT rc)
{
	static TCHAR szText[128];
	int iCount = 0;
	iCount = swprintf_s(szText, TEXT("视角%.2f\r\n转速%.3f\r\n移速%.3f"), m_fFov / D3DX_PI,m_fRotateRatio,m_fMoveRatio);
	pFont->DrawText(NULL, szText, iCount, &rc, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
