#include "stdafx.h"

/******************************/
/*	功能：设定摄像机默认信息    */
/*  参数：void 无              */
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
/*	功能：设定摄像机位置坐标    */
/*  参数：x,y,z三维位置坐标     */
/******************************/
void D3DXCamera::SetCameraPos(float x, float y, float z)
{
	m_vCameraPos.x = x;
	m_vCameraPos.y = y;
	m_vCameraPos.z = z;
}

/******************************/
/*	功能：设定摄像机位置坐标    */
/*  参数：x,y,z三维位置坐标     */
/******************************/
void D3DXCamera::SetCameraPos(D3DXVECTOR3 vPos)
{
	m_vCameraPos = vPos;
}

/******************************/
/*	功能：获取摄像机位置坐标  */
/*  参数：x,y,z三维位置坐标   */
/******************************/
void D3DXCamera::GetCameraPos(float *x, float *y, float *z)
{
	*x = m_vCameraPos.x;
	*y = m_vCameraPos.y;
	*z = m_vCameraPos.z;
}

/******************************/
/*	功能：获取摄像机位置坐标  */
/*  参数：x,y,z三维位置坐标   */
/******************************/
D3DXVECTOR3 D3DXCamera::GetCameraPos(void)
{
	return m_vCameraPos;
}

/******************************/
/*	功能：将摄像机左右移动      */
/*  参数：fLR 左右移动距离 右正 */
/******************************/
void D3DXCamera::MoveCameraLR(float fLR)
{
	D3DXVec3Normalize(&m_vCameraRight, &m_vCameraRight);
	m_vCameraPos += (fLR * m_vCameraRight);
}

/*******************************/
/*	功能：将摄像机前后移动      */
/*  参数：fFB 前后移动距离 前正 */
/******************************/
void D3DXCamera::MoveCameraFB(float fFB)
{
	D3DXVec3Normalize(&m_vCameraLook, &m_vCameraLook);
	m_vCameraPos += (fFB * m_vCameraLook);
}

/*******************************/
/*	功能：将摄像机前后移动      */
/*  参数：fUD 前后移动距离 上正 */
/******************************/
void D3DXCamera::MoveCameraUD(float fUD)
{
	D3DXVec3Normalize(&m_vCameraUp, &m_vCameraUp);
	m_vCameraPos += (fUD * m_vCameraUp);
}

/******************************/
/*	功能：将摄像机左右旋转     */
/*  参数：fYaw旋转程度    右正 */
/******************************/
void D3DXCamera::RotateCameraLR(float fYaw)
{
	D3DXMATRIX matYaw;
	D3DXMatrixIdentity(&matYaw);

	D3DXMatrixRotationAxis(&matYaw, &m_vCameraUp, fYaw);
	//旋转
	D3DXVec3TransformCoord(&m_vCameraLook, &m_vCameraLook, &matYaw);
	D3DXVec3TransformCoord(&m_vCameraRight, &m_vCameraRight, &matYaw);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matYaw);
}

/******************************/
/*	功能：将摄像机上下旋转     */
/*  参数：fYaw旋转程度    下正 */
/******************************/
void D3DXCamera::RotateCameraUD(float fPitch)
{
	D3DXMATRIX matPitch;
	D3DXMatrixIdentity(&matPitch);

	D3DXMatrixRotationAxis(&matPitch, &m_vCameraRight, fPitch);
	//旋转
	D3DXVec3TransformCoord(&m_vCameraLook, &m_vCameraLook, &matPitch);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matPitch);
}

/******************************/
/*	功能：将摄像机左右倾斜       */
/*  参数：fAngle倾斜程度   左正 */
/******************************/
void D3DXCamera::SlopeCameraLR(float fAngle)
{
	D3DXMATRIX matAngle;
	D3DXMatrixIdentity(&matAngle);

	D3DXMatrixRotationAxis(&matAngle, &m_vCameraLook, fAngle);
	//倾斜
	D3DXVec3TransformCoord(&m_vCameraRight, &m_vCameraRight, &matAngle);
	D3DXVec3TransformCoord(&m_vCameraUp, &m_vCameraUp, &matAngle);
}

/******************************/
/*	功能：使用键盘移动摄像机  */
/*  参数：hWnd 窗口句柄       */
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
/*	功能：对摄像机的朝向进行插值设置*/
/*  参数：vLookAt 方位 timeDelta时间变化量*/
/******************************/
void D3DXCamera::CameraLookAtRotating(bool *bAimAt, bool *bMoveTo, D3DXVECTOR3 vPos, float fRadius, float timeDelta)
{
	if (*bAimAt || *bMoveTo)
	{
		const float T = 1.0f;//T秒完成插值
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
/*	功能：对摄像机进行设置    */
/*  参数：pDevice D3D设备     */
/******************************/
void D3DXCamera::SetCamera(void)
{
	SetCamera(m_vCameraPos, m_vCameraLook, m_vCameraUp, m_vCameraRight);
}

/******************************/
/*	功能：对摄像机进行设置       */
/*  参数：各种分量              */
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
/*	功能：初始化镜头光晕信息    */
/*  参数：void 无参数          */
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
/*	功能：计算光源在屏幕上的坐标*/
/*  参数：void 无参数          */
/******************************/
HRESULT D3DXCameraLensFlare::ComputeLightScreenPos(void)
{
	D3DXVECTOR4 vResult;
	D3DXMATRIX matConcatenate;//总变换矩阵
	D3DXMATRIX matView;    //观察矩阵
	D3DXMATRIX matProjection;//投影矩阵
	D3DXMATRIX matViewPort = D3DXMATRIX(//视口矩阵
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
/*	功能：渲染光晕             */
/*  参数：void 无参数          */
/******************************/
void D3DXCameraLensFlare::Render(void)
{
	ComputeLightScreenPos();
	if (m_vLightScrPos.z < 0.0f)return;//太阳不可见
	//可见的话：
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

