#include "stdafx.h"

/******************************/
/*	���ܣ��趨�����Ĭ����Ϣ    */
/*  ������void ��              */
/******************************/
D3DXCamera::D3DXCamera(D3DXVECTOR3 vPos)
{
	this->m_vCameraLook = { 0.0f, 0.0f, 1.0f };
	this->m_vCameraPos = vPos;
	this->m_vCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&this->m_vCameraRight, &this->m_vCameraUp, &this->m_vCameraLook);

	D3DXVec3Normalize(&m_vCameraRight, &m_vCameraRight);
	D3DXVec3Normalize(&m_vCameraUp, &m_vCameraUp);
	D3DXVec3Normalize(&m_vCameraLook, &m_vCameraLook);
}

/******************************/
/*	���ܣ��趨�����λ������    */
/*  ������x,y,z��άλ������     */
/******************************/
void D3DXCamera::SetCameraPos(float x, float y, float z)
{
	m_vCameraPos.x = x;
	m_vCameraPos.y = y;
	m_vCameraPos.z = z;
}

/******************************/
/*	���ܣ��趨�����λ������    */
/*  ������x,y,z��άλ������     */
/******************************/
void D3DXCamera::SetCameraPos(D3DXVECTOR3 vPos)
{
	m_vCameraPos = vPos;
}

/******************************/
/*	���ܣ���ȡ�����λ������  */
/*  ������x,y,z��άλ������   */
/******************************/
void D3DXCamera::GetCameraPos(float *x, float *y, float *z)
{
	*x = m_vCameraPos.x;
	*y = m_vCameraPos.y;
	*z = m_vCameraPos.z;
}

/******************************/
/*	���ܣ���ȡ�����λ������  */
/*  ������x,y,z��άλ������   */
/******************************/
D3DXVECTOR3 D3DXCamera::GetCameraPos(void)
{
	return m_vCameraPos;
}

/******************************/
/*	���ܣ�������������ƶ�      */
/*  ������fLR �����ƶ����� ���� */
/******************************/
void D3DXCamera::MoveCameraLR(float fLR)
{
	D3DXVec3Normalize(&m_vCameraRight, &m_vCameraRight);
	m_vCameraPos += (fLR * m_vCameraRight);
}

/*******************************/
/*	���ܣ��������ǰ���ƶ�      */
/*  ������fFB ǰ���ƶ����� ǰ�� */
/******************************/
void D3DXCamera::MoveCameraFB(float fFB)
{
	D3DXVec3Normalize(&m_vCameraLook, &m_vCameraLook);
	m_vCameraPos += (fFB * m_vCameraLook);
}

/*******************************/
/*	���ܣ��������ǰ���ƶ�      */
/*  ������fUD ǰ���ƶ����� ���� */
/******************************/
void D3DXCamera::MoveCameraUD(float fUD)
{
	D3DXVec3Normalize(&m_vCameraUp, &m_vCameraUp);
	m_vCameraPos += (fUD * m_vCameraUp);
}

/******************************/
/*	���ܣ��������������ת     */
/*  ������fYaw��ת�̶�    ���� */
/******************************/
void D3DXCamera::RotateCameraLR(float fYaw)
{
	D3DXMATRIX matYaw;
	D3DXMatrixIdentity(&matYaw);

	D3DXMatrixRotationAxis(&matYaw, &m_vCameraUp, fYaw);
	//��ת
	D3DXVec3TransformCoord(&m_vCameraLook, &m_vCameraLook, &matYaw);
	D3DXVec3TransformCoord(&m_vCameraRight, &m_vCameraRight, &matYaw);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matYaw);
}

/******************************/
/*	���ܣ��������������ת     */
/*  ������fYaw��ת�̶�    ���� */
/******************************/
void D3DXCamera::RotateCameraUD(float fPitch)
{
	D3DXMATRIX matPitch;
	D3DXMatrixIdentity(&matPitch);

	D3DXMatrixRotationAxis(&matPitch, &m_vCameraRight, fPitch);
	//��ת
	D3DXVec3TransformCoord(&m_vCameraLook, &m_vCameraLook, &matPitch);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matPitch);
}

/******************************/
/*	���ܣ��������������б       */
/*  ������fAngle��б�̶�   ���� */
/******************************/
void D3DXCamera::SlopeCameraLR(float fAngle)
{
	D3DXMATRIX matAngle;
	D3DXMatrixIdentity(&matAngle);

	D3DXMatrixRotationAxis(&matAngle, &m_vCameraLook, fAngle);
	//��б
	D3DXVec3TransformCoord(&m_vCameraRight, &m_vCameraRight, &matAngle);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matAngle);
}

/******************************/
/*	���ܣ�ʹ�ü����ƶ������  */
/*  ������hWnd ���ھ��       */
/******************************/
void D3DXCamera::KeyBoardMove(float fMoveRatio, float RotateRatio)
{
	if (KEYDOWN('W'))this->MoveCameraFB(fMoveRatio);
	if (KEYDOWN('S'))this->MoveCameraFB(-fMoveRatio);
	if (KEYDOWN('D'))this->MoveCameraLR(fMoveRatio);
	if (KEYDOWN('A'))this->MoveCameraLR(-fMoveRatio);
	if (KEYDOWN('Q'))this->SlopeCameraLR(RotateRatio);
	if (KEYDOWN('E'))this->SlopeCameraLR(-RotateRatio);
	if (KEYDOWN(VK_SPACE)) this->MoveCameraUD(fMoveRatio);
	if (KEYDOWN(VK_CONTROL))this->MoveCameraUD(-fMoveRatio);

	if (KEYDOWN(VK_UP))   this->RotateCameraUD(-RotateRatio);
	if (KEYDOWN(VK_DOWN)) this->RotateCameraUD(RotateRatio);
	if (KEYDOWN(VK_LEFT)) this->RotateCameraLR(-RotateRatio);
	if (KEYDOWN(VK_RIGHT))this->RotateCameraLR(RotateRatio);
}

/******************************/
/*	���ܣ���������ĳ�����в�ֵ����*/
/*  ������vLookAt ��λ timeDeltaʱ��仯��*/
/******************************/
void D3DXCamera::CameraLookAtRotating(bool *bAimAt, bool *bMoveTo, D3DXVECTOR3 vPos, float fRadius, float timeDelta)
{
	if (*bAimAt || *bMoveTo)
	{
		const float T = 1.0f;//T����ɲ�ֵ
		static float AccumulativeTime = 0;
		AccumulativeTime += timeDelta;
		D3DXVECTOR3 vCameraLook, vCameraRight, vCameraUp;
		D3DXMATRIX matTrans, matTarget;
		D3DXMatrixIdentity(&matTarget);
		D3DXMatrixIdentity(&matTrans);

		if (*bMoveTo)
		{
			D3DXVECTOR3 vDestPos;
			D3DXMATRIX matProj;
			m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			vDestPos = D3DXVECTOR3(0.0f, 3.0f / 4.0f / matProj(1, 1), 1.0f);

			D3DXMATRIX matInvView;
			D3DXMatrixInverse(&matInvView, NULL, &m_matView);
			D3DXVec3TransformNormal(&vDestPos, &vDestPos, &matInvView);
			D3DXVec3Normalize(&vDestPos, &vDestPos);
			D3DXVec3Normalize(&m_vCameraLook, &m_vCameraLook);

			float fAngle = acos(D3DXVec3Dot(&vDestPos, &m_vCameraLook));
			D3DXVECTOR3 vDirection, vDestCameraPos;
			float fDistant = D3DXVec3Length(&(vDirection = vPos - m_vCameraPos));
			vDestCameraPos = vDirection*(1 - fRadius / fDistant / tan(fAngle));

			D3DXMATRIX matOriginal;
			D3DXMatrixIdentity(&matOriginal);
			D3DXMatrixTranslation(&matTrans, vDestCameraPos.x, vDestCameraPos.y, vDestCameraPos.z);

			float w = AccumulativeTime / T;
			D3DXMATRIX matTransforming = pow(w, 5.0f)*matTrans + (1 - pow(w, 0.1f))*matOriginal;
			D3DXVec3TransformCoord(&m_vCameraPos, &m_vCameraPos, &matTransforming);
		}

		D3DXVECTOR3 vTargetAt = vPos - m_vCameraPos;
		D3DXVec3Normalize(&vTargetAt, &vTargetAt);
		D3DXVec3Normalize(&m_vCameraLook, &m_vCameraLook);

		float fAngle = acos(D3DXVec3Dot(&vTargetAt, &m_vCameraLook));
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross(&vAxis, &m_vCameraLook, &vTargetAt);

		D3DXMATRIX matOriginal;
		D3DXMatrixIdentity(&matOriginal);
		if (vAxis != D3DXVECTOR3(0.0F, 0.0F, 0.0F))
		{
			D3DXMatrixRotationAxis(&matTarget, &vAxis, fAngle);
			//D3DXMatrixRotationAxis(&matTarget, &m_vCameraUp, fAngle);
		}

		float w = AccumulativeTime / T;
		if (w > 1.0f)
		{
			D3DXVec3TransformCoord(&m_vCameraLook, &m_vCameraLook, &matTarget);
			D3DXVec3TransformCoord(&m_vCameraRight, &m_vCameraRight, &matTarget);
			D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matTarget);

			SetCamera();

			AccumulativeTime = 0.0f;
			*bMoveTo = false;
			*bAimAt = false;
			return;
		}
		D3DXMATRIX matRotating = pow(w, 2.0f)*matTarget + (1 - pow(w, 2.f))*matOriginal;
		//pow(w, 5.0f)*matTarget + (1 - pow(w, 0.1f))*matOriginal;
		D3DXVec3TransformCoord(&vCameraLook, &m_vCameraLook, &matRotating);
		D3DXVec3TransformCoord(&vCameraRight, &m_vCameraRight, &matRotating);
		D3DXVec3TransformCoord(&vCameraUp, &m_vCameraUp, &matRotating);

		SetCamera(m_vCameraPos, vCameraLook, vCameraUp, vCameraRight);
	}
}

/******************************/
/*	���ܣ����������������    */
/*  ������pDevice D3D�豸     */
/******************************/
void D3DXCamera::SetCamera(void)
{
	SetCamera(m_vCameraPos, m_vCameraLook, m_vCameraUp, m_vCameraRight);
}

/******************************/
/*	���ܣ����������������       */
/*  ���������ַ���              */
/******************************/
void D3DXCamera::SetCamera(D3DXVECTOR3 vPos,
	D3DXVECTOR3 vLook,
	D3DXVECTOR3 vUp,
	D3DXVECTOR3 vRight)
{
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	matView._11 = vRight.x;
	matView._21 = vRight.y;
	matView._31 = vRight.z;
	matView._41 = -D3DXVec3Dot(&vPos, &vRight);
	matView._12 = vUp.x;
	matView._22 = vUp.y;
	matView._32 = vUp.z;
	matView._42 = -D3DXVec3Dot(&vPos, &vUp);
	matView._13 = vLook.x;
	matView._23 = vLook.y;
	matView._33 = vLook.z;
	matView._43 = -D3DXVec3Dot(&vPos, &vLook);
	m_matView = matView;
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}

/******************************/
/*	���ܣ���ʼ����ͷ������Ϣ    */
/*  ������void �޲���          */
/******************************/
void D3DXCameraLensFlare::InitializeFlares(void)
{
	//WTF?...
	m_pLensFlare[4] = LensFlare(0, m_pTexFlares[0], -0.3f, 0.7f, D3DCOLOR_XRGB(50, 200, 200));
	m_pLensFlare[5] = LensFlare(0, m_pTexFlares[0], 0.6f, 0.9f, D3DCOLOR_XRGB(150, 227, 185));
	m_pLensFlare[6] = LensFlare(0, m_pTexFlares[0], 0.7f, 0.4f, D3DCOLOR_XRGB(130, 241, 198));
	m_pLensFlare[0] = LensFlare(1, m_pTexFlares[1], -0.5f, 0.7f, D3DCOLOR_XRGB(50, 25, 50));
	m_pLensFlare[1] = LensFlare(1, m_pTexFlares[1], 0.3f, 0.4f, D3DCOLOR_XRGB(0, 220, 230));
	m_pLensFlare[2] = LensFlare(1, m_pTexFlares[1], 1.2f, 1.0f, D3DCOLOR_XRGB(100, 50, 50));
	m_pLensFlare[3] = LensFlare(1, m_pTexFlares[1], 1.5f, 1.5f, D3DCOLOR_XRGB(50, 100, 50));
	m_pLensFlare[7] = LensFlare(2, m_pTexFlares[2], -0.7f, 0.7f, D3DCOLOR_XRGB(50, 100, 25));
	m_pLensFlare[8] = LensFlare(2, m_pTexFlares[2], 0.0f, 0.6f, D3DCOLOR_XRGB(25, 25, 25));
	m_pLensFlare[9] = LensFlare(2, m_pTexFlares[2], 2.2f, 1.4f, D3DCOLOR_XRGB(207, 119, 115));
}

/******************************/
/*	���ܣ������Դ����Ļ�ϵ�����*/
/*  ������void �޲���          */
/******************************/
HRESULT D3DXCameraLensFlare::ComputeLightScreenPos(void)
{
	D3DXVECTOR4 vResult;
	D3DXMATRIX matConcatenate;//�ܱ任����
	D3DXMATRIX matView;    //�۲����
	D3DXMATRIX matProjection;//ͶӰ����
	D3DXMATRIX matViewPort = D3DXMATRIX(//�ӿھ���
		m_vScrCenterPos.x, 0, 0, 0,
		0, -m_vScrCenterPos.y, 0, 0,
		0, 0, 1, 0,
		m_vScrCenterPos.x, m_vScrCenterPos.y, 0, 1);

	D3DXMatrixIdentity(&matConcatenate);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProjection);

	HRESULT hr = 0;

	if (FAILED(hr = m_pd3dDevice->GetTransform(D3DTS_VIEW, &matView)))
		return hr;
	if (FAILED(hr = m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProjection)))
		return hr;

	matConcatenate = matView*matProjection*matViewPort;

	D3DXVec3Transform(&vResult, &m_vLightPos, &matConcatenate);

	m_vLightScrPos = D3DXVECTOR3(vResult.x / vResult.w, vResult.y / vResult.w, vResult.w);

	return hr;
}

/******************************/
/*	���ܣ���Ⱦ����             */
/*  ������void �޲���          */
/******************************/
void D3DXCameraLensFlare::Render(void)
{
	ComputeLightScreenPos();
	if (m_vLightScrPos.z < 0.0f)return;//̫�����ɼ�
	//�ɼ��Ļ���
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	// use alpha in material's diffuse component for alpha
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pFlare->Begin(D3DXSPRITE_ALPHABLEND);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXVECTOR3 vDirection = m_vScrCenterPos - m_vLightScrPos;
	vDirection.z = m_vScrCenterPos.z = m_vLightScrPos.z = 0;
	for (int i = 0; i != 10; ++i)
	{
		D3DXMATRIX matScale;
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixTransformation2D(&matScale, NULL, NULL, &D3DXVECTOR2(m_pLensFlare[i].m_fSize, m_pLensFlare[i].m_fSize), NULL, NULL, NULL);
		m_pFlare->SetTransform(&matScale);
		D3DXVECTOR3 vPos = m_vLightScrPos + vDirection*m_pLensFlare[i].m_fPosition;
		vPos /= m_pLensFlare[i].m_fSize;

		m_pFlare->Draw(m_pLensFlare[i].m_pTexFlare, NULL, &m_pLensFlare[i].m_Center,
			&vPos, m_pLensFlare[i].m_crColor);
	}
	m_pFlare->End();

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

