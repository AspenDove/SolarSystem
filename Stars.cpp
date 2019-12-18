#include "StdAfx.h"

/************************************************************************/
/*	功能：设置材质                                                       */
/*	参数：void 无参数                                                    */
/************************************************************************/
void FixedStar::SetMaterial(void)
{
	D3DMATERIAL9 g_material;
	ZeroMemory(&g_material, sizeof(D3DMATERIAL9));  // 将g_material对象的内存块清零
	g_material.Diffuse.r = g_material.Ambient.r = 1.0f;
	g_material.Diffuse.g = g_material.Ambient.g = 1.0f;
	g_material.Diffuse.b = g_material.Ambient.b = 1.0f;
	g_material.Diffuse.a = g_material.Ambient.a = 1.0f;
	g_material.Power = 0.0f;//1e30f;
	m_d3dDevice->SetMaterial(&g_material);
}

/************************************************************************/
/*	功能：设置阳光                                                       */
/*	参数：void 无参数                                                    */
/************************************************************************/
void FixedStar::SetSunLight(void)
{
	D3DLIGHT9 g_light;
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	// 设置光源属性
	g_light.Type = D3DLIGHT_POINT;  // 类型为电光源
	g_light.Diffuse = d3d::WHITE;
	g_light.Position.x = g_light.Position.y = g_light.Position.z = 0.0f;
	g_light.Range = FLT_MAX;

	// 注册0号灯光源
	m_d3dDevice->SetLight(0, &g_light);   // 设置该光源
	m_d3dDevice->LightEnable(0, TRUE);    // 启用该光源
}

/************************************************************************/
/*	功能：设置行星的光照                                                  */
/*	参数：void 无参数                                                    */
/************************************************************************/
void FixedStar::SetPlanetLight(void)
{
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(10,10,10));
	m_d3dDevice->SetLight(0, &d3d::InitPointLight(&D3DXVECTOR3(0.0F, 0.0F, 0.0F), &d3d::WHITE));
	m_d3dDevice->LightEnable(0, TRUE);    // 启用该光源
}

/************************************************************************/
/*	功能：设置太阳光环                                                   */
/*	参数：fLength 光环纹理长度                                             */
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
/*	功能：渲染太阳光环                                                     */
/*	参数：vPos 摄像机的位置，永远朝向摄像机                                  */
/************************************************************************/
void FixedStar::HaloRender(D3DXVECTOR3 vPos)
{
	//使用在材质中指定的Alpha分量合成透明
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	/*//使用在纹理中的Alpha通道
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);*/

	//设置透明因子以决定透明度
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));

	m_d3dDevice->SetRenderState(D3DRS_WRAP0, NULL);//禁用纹理环绕！

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
	m_d3dDevice->LightEnable(0, false);    // 禁用该光源

	m_d3dDevice->SetTransform(D3DTS_WORLD, &(matBillboardY*matBillboardA));
	m_d3dDevice->SetFVF(HaloVertex::FVF);
	m_d3dDevice->SetStreamSource(0, vbHalo, 0, sizeof(HaloVertex));
	m_d3dDevice->SetMaterial(&mtlHalo);
	m_d3dDevice->SetTexture(0, pTexHalo);
	m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	m_d3dDevice->LightEnable(0, true);    // 启用该光源
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

/************************************************************************/
/*	功能：设置太阳运动矩阵                                                 */
/*	参数：timeDelta 时间间隔，可以以真实时间乘以比例系数以调整速度             */
/************************************************************************/
void FixedStar::SetMatrix(float timeDelta)
{
	static float AccumulativeTime = 0;
	AccumulativeTime += timeDelta;
	AccumulativeTime = fmod(AccumulativeTime, m_AT);

	//世界变换
	D3DXMATRIXA16 matWorld;
	D3DXMATRIX matRot, matAngle;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matAngle);

	//太阳自转							//deltaAngle = 2*pi/T*deltaT
	D3DXMatrixRotationY(&matRot, 2 * D3DX_PI / m_AT * AccumulativeTime);

	//自转轴与黄道面夹角	  
	D3DXMatrixRotationZ(&matAngle, 2.32710566932577e-3f);//8′……

	m_matWorld = matWorld = matRot * matAngle;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

/************************************************************************/
/*	功能：渲染太阳                                                        */
/*	参数：timeDelta 时间间隔，可以以真实时间乘以比例系数以调整速度             */
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
/*	功能：设置行星运动矩阵                                                */
/*	参数：timeDelta 时间间隔，可以以真实时间乘以比例系数以调整速度             */
/************************************************************************/
void Planet::SetMatrix(float timeDelta)
{
	static float AAccumulativeTime = 0;//自转
	AAccumulativeTime += timeDelta;
	AAccumulativeTime = fmod(AAccumulativeTime, m_AT);

	static float RAccumulativeTime = 0;//公转
	RAccumulativeTime += timeDelta;
	RAccumulativeTime = fmod(RAccumulativeTime, m_RT);

	D3DXMATRIXA16 matWorld;
	D3DXMATRIX matTrans, matRot0, matRot1, matAngle;
	D3DXVECTOR3 vPos = { m_DIS, 0.0f, 0.0f };//地日距离

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot0);
	D3DXMatrixIdentity(&matRot1);
	D3DXMatrixIdentity(&matAngle);
	//自转
	D3DXMatrixRotationY(&matRot0, -2 * D3DX_PI / m_AT * AAccumulativeTime);

	//黄赤交角							顺时针
	D3DXMatrixRotationZ(&matAngle, m_A_AXIS);

	//令位置向量(vPos)公转						   //北逆南顺
	D3DXMatrixRotationY(&matRot1, -2 * D3DX_PI / m_RT * RAccumulativeTime);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRot1);

	m_vPos = vPos;
	//平移到轨道			
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_matWorld = matWorld = matRot0 * matAngle * matTrans;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

/************************************************************************/
/*	功能：渲染行星														*/
/*	参数：timeDelta 时间间隔，可以以真实时间乘以比例系数以调整速度             */
/************************************************************************/
void Planet::Render(float timeDelta)
{
	this->SetPlanetLight();
	this->SetMaterial();

	//启用纹理环绕
	m_d3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U);
	m_d3dDevice->SetTexture(0, pTexture);
	SetMatrix(timeDelta);
	pMesh->DrawSubset(0);
}

/************************************************************************/
/*	功能：设置月球运动矩阵                                                 */
/*	参数：timeDelta 时间间隔，可以以真实时间乘以比例系数以调整速度             */
/************************************************************************/
void Moon::SetMatrix(float timeDelta)
{
	static float RAccumulativeTime = 0;//公转
	RAccumulativeTime += timeDelta;
	RAccumulativeTime = fmod(RAccumulativeTime, m_RT);

	static float ERAccumulativeTime = 0;//跟随地球公转
	ERAccumulativeTime += timeDelta;
	ERAccumulativeTime = fmod(ERAccumulativeTime, m_CENTER_RT);

	static float AAccumulativeTime = 0;//黄白交线自转
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

	//旋转以朝向地球
	D3DXMatrixRotationY(&matRot0, D3DX_PI/2);

	//平移到月球轨道
	D3DXMatrixTranslation(&matTrans0, 0.0f, 0.0f, m_DIS);
	m_vPos = { 0.0, 0.0f, m_DIS };

	//公转
	D3DXMatrixRotationY(&matRot1, -2 * D3DX_PI / m_RT * RAccumulativeTime);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matRot1);

	if (m_bPrecession)//如果有交线进动
	{
		//黄白交线按顺时针进动
		D3DXMatrixRotationY(&matAxis, 2 * D3DX_PI / m_T_LINEAXIS * AAccumulativeTime);
		D3DXVec3TransformCoord(&vAxis, &vAxis, &matAxis);
	}

	//黄白交角
	D3DXMatrixRotationAxis(&matWhite, &vAxis, this->m_A_TRACK);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matWhite);

	//令位置向量(vPos)公转						   //!!北逆南顺
	D3DXMatrixRotationY(&matRot2, -2 * D3DX_PI / m_CENTER_RT * ERAccumulativeTime);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRot2);

	//平移到轨道			
	D3DXMatrixTranslation(&matTrans1, vPos.x, vPos.y, vPos.z);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matTrans1);

	m_matWorld = matWorld = matRot0 * matTrans0 * matRot1 * matWhite * matTrans1;
	m_d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void Moon::Render(float timeDelta)
{
	this->SetPlanetLight();
	this->SetMaterial();

	//启用纹理环绕
	m_d3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U);
	m_d3dDevice->SetTexture(0, pTexture);
	SetMatrix(timeDelta);
	pMesh->DrawSubset(0);
}

/************************************************************************/
/*	功能：创建一个行星                                                    */
/*	参数：行星的各种参数                                                  */
/************************************************************************/
Planet *FixedStar::CreatePlanet(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS)
{
	return new Planet(m_d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS);
}

/************************************************************************/
/*	功能：创建一个卫星                                                    */
/*	参数：卫星的各种参数                                                  */
/************************************************************************/
Moon *Planet::CreateMoon(LPCWSTR szTextureName, float R, float AT, float RT, float DIS, float A_TRACK, float A_AXIS,float T_LINEAXIS,bool bP)
{
	return new Moon(m_d3dDevice, szTextureName, R, AT, RT, DIS, A_TRACK, A_AXIS, this->m_DIS, this->m_RT, T_LINEAXIS, bP);
}

