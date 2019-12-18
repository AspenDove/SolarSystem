#include ".\skybox.h"

D3DXSkybox::D3DXSkybox(LPDIRECT3DDEVICE9 g_pD3DDevice)
{
	m_pD3DDevice = g_pD3DDevice;
	m_pVB = NULL;
	m_pIB = NULL;
	ZeroMemory(m_pTextrue, sizeof(m_pTextrue));

	this->InitSkyBox(20000.0f);
	this->SetTexture(TEXT("sky\\right.jpg"), 0);
	this->SetTexture(TEXT("sky\\left.jpg"), 1);
	this->SetTexture(TEXT("sky\\top.jpg"), 2);
	this->SetTexture(TEXT("sky\\bottom.jpg"), 3);
	this->SetTexture(TEXT("sky\\back.jpg"), 4);
	this->SetTexture(TEXT("sky\\front.jpg"), 5);
}

D3DXSkybox::~D3DXSkybox(void)
{
	for (int i = 0; i != 6; ++i)
		d3d::Release<LPDIRECT3DTEXTURE9>(m_pTextrue[i]);
}
HRESULT D3DXSkybox::InitSkyBox(float scale)
{
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(24 * sizeof(SkyBoxVertex), D3DUSAGE_WRITEONLY,
		SkyBoxVertex::FVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	SkyBoxVertex* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// positive x
	pVertex[0] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	pVertex[1] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	pVertex[2] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	pVertex[3] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);

	// negative x
	pVertex[4] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	pVertex[5] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	pVertex[6] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	pVertex[7] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 1.0f);

	// positive y
	pVertex[8] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	pVertex[9] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	pVertex[10] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	pVertex[11] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 1.0f);

	// negative y
	pVertex[12] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	pVertex[13] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	pVertex[14] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	pVertex[15] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);

	// positive z
	pVertex[16] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	pVertex[17] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	pVertex[18] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	pVertex[19] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 1.0f);

	// negative z
	pVertex[20] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	pVertex[21] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	pVertex[22] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	pVertex[23] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);

	m_pVB->Unlock();

	if (FAILED(m_pD3DDevice->CreateIndexBuffer(36 * sizeof(WORD),D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIB, 0)))
		return E_FAIL;

	WORD* dwIndices = 0;
	m_pIB->Lock(0, 0, (void**)&dwIndices, 0);

	// positive x
	dwIndices[0] = 0;	dwIndices[1] = 1;	dwIndices[2] = 2;
	dwIndices[3] = 0;	dwIndices[4] = 2;	dwIndices[5] = 3;

	// negative x
	dwIndices[6] = 4;  dwIndices[7] = 5;  dwIndices[8] = 6;
	dwIndices[9] = 4;  dwIndices[10] = 6;  dwIndices[11] = 7;

	// positive y
	dwIndices[12] = 8;  dwIndices[13] = 9;  dwIndices[14] = 10;
	dwIndices[15] = 8;  dwIndices[16] = 10; dwIndices[17] = 11;

	// negative y
	dwIndices[18] = 12; dwIndices[19] = 13; dwIndices[20] = 14;
	dwIndices[21] = 12; dwIndices[22] = 14; dwIndices[23] = 15;

	// positive z
	dwIndices[24] = 16; dwIndices[25] = 17; dwIndices[26] = 18;
	dwIndices[27] = 16; dwIndices[28] = 18; dwIndices[29] = 19;

	// negative z
	dwIndices[30] = 20; dwIndices[31] = 21; dwIndices[32] = 22;
	dwIndices[33] = 20; dwIndices[34] = 22; dwIndices[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

bool D3DXSkybox::SetTexture(const LPCWSTR TextureFile, int flag)
{
	return !D3DXCreateTextureFromFile(m_pD3DDevice, TextureFile, &m_pTextrue[flag]);
	//return !FAILED(D3DXCreateTextureFromFileEx(m_pD3DDevice, TextureFile, 1024, 1024, D3DX_DEFAULT, 0,
	//	D3DFMT_UNKNOWN, D3DPOOL_DEFAULT
	//	, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pTextrue[flag]));
} //D3DXCreateTextureFromFile(m_pD3DDevice, TextureFile, &m_pTextrue[flag])

void D3DXSkybox::Render(D3DXVECTOR3 vCameraPos)
{
	m_pD3DDevice->SetRenderState(D3DRS_WRAP0, NULL);//关闭纹理环绕
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);//关闭灯光

	//防止z-buffer写入，关闭深度缓存
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);

	//夹取纹理寻址，无缝连接
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//坐标转换
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matWorld, vCameraPos.x, vCameraPos.y, vCameraPos.z);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(SkyBoxVertex));
	m_pD3DDevice->SetIndices(m_pIB);
	m_pD3DDevice->SetFVF(SkyBoxVertex::FVF);
	//渲染六个面
	for (int i = 0; i != 6; ++i)
	{		
		m_pD3DDevice->SetTexture(0, m_pTextrue[i]);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
 		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,     // 起始地址
			i * 4, // 最小的索引值
			4,     // 顶点数量
			i * 6, // 开始位置
			2);    // 图元数量
	}
	//启用深度缓存，恢复默认
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	//关闭夹取地址
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}




