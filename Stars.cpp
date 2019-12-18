#include "StdAfx.h"

/************************************************************************/
/*	���ܣ����ò���                                                       */
/*	������void �޲���                                                    */
/************************************************************************/
void FixedStar::SetMaterial(void)
{
	D3DMATERIAL9 g_material;
	ZeroMemory(&g_material, sizeof(D3DMATERIAL9));  // ��g_material������ڴ������
	g_material.Diffuse.r = g_material.Ambient.r = 1.0f;
	g_material.Diffuse.g = g_material.Ambient.g = 1.0f;
	g_material.Diffuse.b = g_material.Ambient.b = 1.0f;
	g_material.Diffuse.a = g_material.Ambient.a = 1.0f;
	g_material.Power = 0.0f;//1e30f;
	m_d3dDevice->SetMaterial(&g_material);
}

/************************************************************************/
/*	���ܣ���������                                                       */
/*	������void �޲���                                                    */
/************************************************************************/
void FixedStar::SetSunLight(void)
{
	D3DLIGHT9 g_light;
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	// ���ù�Դ����
	g_light.Type = D3DLIGHT_POINT;  // ����Ϊ���Դ
	g_light.Diffuse = d3d::WHITE;
	g_light.Position.x = g_light.Position.y = g_light.Position.z = 0.0f;
	g_light.Range = FLT_MAX;

	// ע��0�ŵƹ�Դ
	m_d3dDevice->SetLight(0, &g_light);   // ���øù�Դ
	m_d3dDevice->LightEnable(0, TRUE);    // ���øù�Դ
}

/************************************************************************/
/*	���ܣ��������ǵĹ���                                                  */
/*	������void �޲���                                                    */
/************************************************************************/
void FixedStar::SetPlanetLight(void)
{
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(10,10,10));
	m_d3dDevice->SetLight(0, &d3d::InitPointLight(&D3DXVECTOR3(0.0F, 0.0F, 0.0F), &d3d::WHITE));
	m_d3dDevice->LightEnable(0, TRUE);    // ���øù�Դ
}

/************************************************************************/
/*	���ܣ�����̫���⻷                                                   */
/*	������fLength �⻷������                                             */
/************************************************************************/
void FixedStar::HaloInitialize(float fLength)
{
	fLength /= 2;
	mtlHalo = d3d::WHITE_MTRL;
	mtlHalo.Diffuse.a = 1.0f; // set alpha to 80% opacity

	m_d3dDevice->CreateVertexBuffer(
		6 * sizeof(HaloVertex), D3DUSAGE_WRITEONLY, HaloVertex::FVF,
		D3DPOOL_MANAGED, &vbHalo, 0);

	HaloVertex* v;
	vbHalo->Lock(0, 0, (void**)&v, 0);

	v[0] = HaloVertex(-1.0f*fLength, -1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = HaloVertex(-1.0f*fLength, 1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = HaloVertex(1.0f*fLength, 1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = HaloVertex(-1.0f*fLength, -1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = HaloVertex(1.0f*fLength, 1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = HaloVertex(1.0f*fLength, -1.0f*fLength, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	vbHalo->Unlock();

	D3DXCreateTextureFromFile(
		m_d3dDevice,
		TEXT("Halo.png"),
		&pTexHalo);
}

/************************************************************************/
/*	���ܣ���Ⱦ̫���⻷                                                     */
/*	������vPos �������λ�ã���Զ���������                                  */
/************************************************************************/
void FixedStar::HaloRender(D3DXVECTOR3 vPos)
{
	//ʹ���ڲ�����ָ����Alpha�����ϳ�͸��
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	/*//ʹ���������е�Alphaͨ��
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);*/

	//����͸�������Ծ���͸����
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));

	m_d3dDevice->SetRenderState(D3DRS_WRAP0, NULL);//���������ƣ�

	D3DXMATRIX matBillboardY, matBillboardA;
	D3DXMatrixIdentity(&matBillboardA);
	D3DXMatrixIdentity(&matBillboardY);

	D3DXVECTOR3 vDirection = vPos;
	D3DXVECTOR3 vProjectionXZ = vDirection; vProjectionXZ.y = 0.0f;

	D3DXVec3Normalize(&vProjectionXZ, &vProjectionXZ);
	D3DXVec3Normalize(&vDirection, &vDirection);

	if (vDirection.x > 0.0f)
		D3DXMatrixRotationY(&matBillboardY, -atanf(vDirection.z / vDirection.x) + D3DX_PI / 2);
	else
		D3DXMatrixRotationY(&matBillboardY, -atanf(vDirection.z / vDirection.x) - D3DX_PI / 2);

	float fDot = D3DXVec3Dot(&vProjectionXZ, &vDirection);
	float fAngle = acos(fDot);
	if (fabs(fAngle) > 1e-3f)
	{
		D3DXVECTOR3 vAxis;
		D3DXVec3Cross(&vAxis, &vProjectionXZ, &vDirection);
		D3DXMatrixRotationAxis(&matBillboardA, &vAxis, fAngle);
	}

	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_d3dDevice->LightEnable(0, false);    // ���øù�Դ

	m_d3dDevice->SetTransform(D3DTS_WORLD, &(matBillboardY*matBillboardA));
	m_d3dDevice->SetFVF(HaloVertex::FVF);
	m_d3dDevice->SetStreamSource(0, vbHalo, 0, sizeof(HaloVertex));
	m_d3dDevice->SetMaterial(&mtlHalo);
	m_d3dDevice->SetTexture(0, pTexHalo);
	m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	m_d3dDevice->LightEnable(0, true);    // ���øù�Դ
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

/************************************************************************/
/*	���ܣ�����̫���˶�����                                                 */
/*	������timeDelta ʱ��������������ʵʱ����Ա���ϵ���Ե����ٶ�             */
/************************************************************************/
void FixedStar::SetMatrix(float timeDelta)
{
	static float AccumulativeTime = 0;
	AccumulativeTime += timeDelta;
	AccumulativeTime = fmod(AccumulativeTime, m_AT);

	//����任
	D3DXMATRIXA16 matWorld;
	D3DXMATRIX matRot, matAngle;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matAngle);

	//̫����ת							//deltaAngle = 2*pi/T*deltaT
	D3DXMatrixRotationY(&matRot, 2 * D3DX_PI / m_AT * AccumulativeTime);

	//��ת����Ƶ���н�	  
	D3DXMatrixRotationZ(&matAngle, 2.32710566932577e-3f);//8�䡭��

	m_matWorld = matWorld = matRot * matAngle;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

/************************************************************************/
/*	���ܣ���Ⱦ̫��                                                        */
/*	������timeDelta ʱ��������������ʵʱ����Ա���ϵ���Ե����ٶ�             */
/************************************************************************/
void FixedStar::Render(float timeDelta)
{
	this->SetSunLight();
	this->SetMaterial();

	m_d3dDevice->SetTexture(0, pTexture);
	SetMatrix(timeDelta);
	pMesh->DrawSubset(0);
}

/************************************************************************/
/*	���ܣ����������˶�����                                                */
/*	������timeDelta ʱ��������������ʵʱ����Ա���ϵ���Ե����ٶ�             */
/************************************************************************/
void Planet::SetMatrix(float timeDelta)
{
	static float AAccumulativeTime = 0;//��ת
	AAccumulativeTime += timeDelta;
	AAccumulativeTime = fmod(AAccumulativeTime, m_AT);

	static float RAccumulativeTime = 0;//��ת
	RAccumulativeTime += timeDelta;
	RAccumulativeTime = fmod(RAccumulativeTime, m_RT);

	D3DXMATRIXA16 matWorld;
	D3DXMATRIX matTrans, matRot0, matRot1, matAngle;
	D3DXVECTOR3 vPos = { m_DIS, 0.0f, 0.0f };//���վ���

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot0);
	D3DXMatrixIdentity(&matRot1);
	D3DXMatrixIdentity(&matAngle);
	//��ת
	D3DXMatrixRotationY(&matRot0, -2 * D3DX_PI / m_AT * AAccumulativeTime);

	//�Ƴཻ��							˳ʱ��
	D3DXMatrixRotationZ(&matAngle, m_A_AXIS);

	//��λ������(vPos)��ת						   //������˳
	D3DXMatrixRotationY(&matRot1, -2 * D3DX_PI / m_RT * RAccumulativeTime);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRot1);

	m_vPos = vPos;
	//ƽ�Ƶ����			
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_matWorld = matWorld = matRot0 * matAngle * matTrans;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

/************************************************************************/
/*	���ܣ���Ⱦ����														*/
/*	������timeDelta ʱ��������������ʵʱ����Ա���ϵ���Ե����ٶ�             */
/************************************************************************/
void Planet::Render(float timeDelta)
{
	this->SetPlanetLight();
	this->SetMaterial();

	//����������
	m_d3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U);
	m_d3dDevice->SetTexture(0, pTexture);
	SetMatrix(timeDelta);
	pMesh->DrawSubset(0);
}

/************************************************************************/
/*	���ܣ����������˶�����                                                 */
/*	������timeDelta ʱ��������������ʵʱ����Ա���ϵ���Ե����ٶ�             */
/************************************************************************/
void Moon::SetMatrix(float timeDelta)
{
	static float RAccumulativeTime = 0;//��ת
	RAccumulativeTime += timeDelta;
	RAccumulativeTime = fmod(RAccumulativeTime, m_RT);

	static float ERAccumulativeTime = 0;//�������ת
	ERAccumulativeTime += timeDelta;
	ERAccumulativeTime = fmod(ERAccumulativeTime, m_CENTER_RT);

	static float AAccumulativeTime = 0;//�ư׽�����ת
	AAccumulativeTime += timeDelta;
	AAccumulativeTime = fmod(AAccumulativeTime, m_T_LINEAXIS);

	D3DXMATRIXA16 matWorld;
	D3DXMATRIX matTrans0, matTrans1, matRot0, matRot1, matRot2, matAxis, matWhite;
	D3DXVECTOR3 vAxis = { 1.0f, 0.0f, 1.0f }, vPos = { m_CENTER_DIS, 0.0f, 0.0f };

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTrans0);
	D3DXMatrixIdentity(&matTrans1);
	D3DXMatrixIdentity(&matRot0);
	D3DXMatrixIdentity(&matRot1);
	D3DXMatrixIdentity(&matRot2);
	D3DXMatrixIdentity(&matAxis);
	D3DXMatrixIdentity(&matWhite);

	//��ת�Գ������
	D3DXMatrixRotationY(&matRot0, D3DX_PI/2);

	//ƽ�Ƶ�������
	D3DXMatrixTranslation(&matTrans0, 0.0f, 0.0f, m_DIS);
	m_vPos = { 0.0, 0.0f, m_DIS };

	//��ת
	D3DXMatrixRotationY(&matRot1, -2 * D3DX_PI / m_RT * RAccumulativeTime);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matRot1);

	if (m_bPrecession)//����н��߽���
	{
		//�ư׽��߰�˳ʱ�����
		D3DXMatrixRotationY(&matAxis, 2 * D3DX_PI / m_T_LINEAXIS * AAccumulativeTime);
		D3DXVec3TransformCoord(&vAxis, &vAxis, &matAxis);
	}

	//�ư׽���
	D3DXMatrixRotationAxis(&matWhite, &vAxis, this->m_A_TRACK);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matWhite);

	//��λ������(vPos)��ת						   //!!������˳
	D3DXMatrixRotationY(&matRot2, -2 * D3DX_PI / m_CENTER_RT * ERAccumulativeTime);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRot2);

	//ƽ�Ƶ����			
	D3DXMatrixTranslation(&matTrans1, vPos.x, vPos.y, vPos.z);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matTrans1);

	m_matWorld = matWorld = matRot0 * matTrans0 * matRot1 * matWhite * matTrans1;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void Moon::Render(float timeDelta)
{
	this->SetPlanetLight();
	this->SetMaterial();

	//����������
	m_d3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U);
	m_d3dDevice->SetTexture(0, pTexture);
	SetMatrix(timeDelta);
	pMesh->DrawSubset(0);
}

/************************************************************************/
/*	���ܣ�����һ������                                                    */
/*	���������ǵĸ��ֲ���                                                  */
/************************************************************************/
Planet *FixedStar::CreatePlanet(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS)
{
	return new Planet(m_d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS);
}

/************************************************************************/
/*	���ܣ�����һ������                                                    */
/*	���������ǵĸ��ֲ���                                                  */
/************************************************************************/
Moon *Planet::CreateMoon(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS,float T_LINEAXIS,bool bP)
{
	return new Moon(m_d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS, this->m_DIS, this->m_RT, T_LINEAXIS, bP);
}

