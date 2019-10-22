#include "SkyBoxClass.h"

SkyBoxClass::SkyBoxClass(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pVertexBuffer = NULL;
	m_fSideLength = 0.0f;
	for (int i = 0; i < 5; i++)
	{
		m_pTextures[i] = NULL;
	}
}

SkyBoxClass::~SkyBoxClass()
{
	SAFE_RELEASE(m_pVertexBuffer);
	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(m_pTextures[i]);
	}
}

BOOL SkyBoxClass::InitSkyBox(float length)
{
	m_fSideLength = length;
	m_pd3dDevice->CreateVertexBuffer(20 * sizeof(SKYBOXVERTEX), 0, SKYBOXVERTEX::FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
	SKYBOXVERTEX vertex[] =
	{
		// ǰ����ĸ�����
		{ -m_fSideLength / 2, 0.0f,    m_fSideLength / 2, 0.0f, 1.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2,   m_fSideLength / 2, 0.0f, 0.0f, },
		{ m_fSideLength / 2, 0.0f,    m_fSideLength / 2, 1.0f, 1.0f, },
		{ m_fSideLength / 2, m_fSideLength / 2,   m_fSideLength / 2, 1.0f, 0.0f, },

		// ������ĸ�����
		{ m_fSideLength / 2, 0.0f,   -m_fSideLength / 2, 0.0f, 1.0f, },
		{ m_fSideLength / 2, m_fSideLength / 2,  -m_fSideLength / 2, 0.0f, 0.0f, },
		{ -m_fSideLength / 2, 0.0f,   -m_fSideLength / 2, 1.0f, 1.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2,  -m_fSideLength / 2, 1.0f, 0.0f, },

		// ������ĸ�����
		{ -m_fSideLength / 2, 0.0f,   -m_fSideLength / 2, 0.0f, 1.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2,  -m_fSideLength / 2, 0.0f, 0.0f, },
		{ -m_fSideLength / 2, 0.0f,    m_fSideLength / 2, 1.0f, 1.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2,   m_fSideLength / 2, 1.0f, 0.0f, },

		// ������ĸ�����
		{ m_fSideLength / 2, 0.0f,   m_fSideLength / 2, 0.0f, 1.0f, },
		{ m_fSideLength / 2, m_fSideLength / 2,  m_fSideLength / 2, 0.0f, 0.0f, },
		{ m_fSideLength / 2, 0.0f,  -m_fSideLength / 2, 1.0f, 1.0f, },
		{ m_fSideLength / 2, m_fSideLength / 2, -m_fSideLength / 2, 1.0f, 0.0f, },

		// ������ĸ�����
		{ m_fSideLength / 2, m_fSideLength / 2, -m_fSideLength / 2, 1.0f, 0.0f, },
		{ m_fSideLength / 2, m_fSideLength / 2,  m_fSideLength / 2, 1.0f, 1.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2, -m_fSideLength / 2, 0.0f, 0.0f, },
		{ -m_fSideLength / 2, m_fSideLength / 2,  m_fSideLength / 2, 0.0f, 1.0f, },
	};
	void* pVertex;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, vertex, sizeof(vertex));
	m_pVertexBuffer->Unlock();
	return true;
}

BOOL SkyBoxClass::LoadSkyTextureFromFile(wchar_t * pFrontTextureFile, 
	wchar_t * pBackTextureFile, wchar_t * pLeftTextureFile, wchar_t * pRightTextureFile, 
	wchar_t * pTopTextureFile)
{
	D3DXCreateTextureFromFile(m_pd3dDevice, pFrontTextureFile, &m_pTextures[0]);
	D3DXCreateTextureFromFile(m_pd3dDevice, pBackTextureFile, &m_pTextures[0]);
	D3DXCreateTextureFromFile(m_pd3dDevice, pLeftTextureFile, &m_pTextures[0]);
	D3DXCreateTextureFromFile(m_pd3dDevice, pRightTextureFile, &m_pTextures[0]);
	D3DXCreateTextureFromFile(m_pd3dDevice, pTopTextureFile, &m_pTextures[0]);
	return 0;
}

VOID SkyBoxClass::RenderSkyBox(D3DMATRIX * pMatWorld, BOOL bRenderFrame)
{
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMatWorld);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));
	m_pd3dDevice->SetFVF(SKYBOXVERTEX::FVF);
	for (int i = 0; i < 5; i++)
	{
		m_pd3dDevice->SetTexture(0, m_pTextures[i]);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
	// ���Ƿ���Ⱦ�߿�Ĵ������
	if (bRenderFrame)  // ���Ҫ��Ⱦ���߿�Ļ�
	{
		// �����ģʽ��Ϊ�߿����
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  
		// һ��forѭ������5������߿���Ƴ���															  
		for (int i = 0; i<5; i++)
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);	  // ���ƶ��� 
		}
		// �����ģʽ����ʵ�����
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	 
	}
}
