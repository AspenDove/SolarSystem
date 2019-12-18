#include "StdAfx.h"

/************************************************************************/
/* ���ܣ������ƶ�����                                                    */
/* ������delta ����Ϊ���� ����Ϊ��С                                     */
/************************************************************************/
void D3DXControl::AdjustMoveRatio(float delta)
{
	m_fMoveRatio = max(m_fMoveRatio + delta, 0.0f);
}

/************************************************************************/
/* ���ܣ�������ת����                                                    */
/* ������delta ����Ϊ���� ����Ϊ��С                                      */
/************************************************************************/
void D3DXControl::AdjustRotateRatio(float delta)
{
	m_fRotateRatio = max(min(m_fRotateRatio + delta, 2 * D3DX_PI), 0.0f);
}

/************************************************************************/
/* ���ܣ������ӽǴ�С                                                      */
/* ������delta ����Ϊ���� ����Ϊ��С                                        */
/************************************************************************/
void D3DXControl::AdjustFov(float delta)
{
	m_fFov = max(min(m_fFov + delta, 0.3f * D3DX_PI), 0.01f*D3DX_PI);
}

/************************************************************************/
/* ���ܣ���ʾ������Ϣ                                                      */
/* ������pFont ���� rc ��Ļ����                                            */
/************************************************************************/
void D3DXControl::ShowData(ID3DXFont* pFont,RECT rc)
{
	static TCHAR szText[128];
	int iCount = 0;
	iCount = swprintf_s(szText, TEXT("�ӽ�%.2f\r\nת��%.3f\r\n����%.3f"), m_fFov / D3DX_PI,m_fRotateRatio,m_fMoveRatio);
	pFont->DrawText(NULL, szText, iCount, &rc, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
