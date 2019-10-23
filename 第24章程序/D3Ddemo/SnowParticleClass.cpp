#include "SnowParticleClass.h"

SnowParticleClass::SnowParticleClass(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pVertexBuffer = NULL;
	m_vSnows.reserve(PARTICLE_NUMBER);
	for (int i = 0; i < 6; i++)
	{
		m_pTexture[i] = NULL;
	}
}

HRESULT SnowParticleClass::InitSnowParticle()
{
	srand((unsigned)timeGetTime());
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		// x值在[-SNOW_SYSTEM_LENGTH_X,SNOW_SYSTEM_LENGTH_X / 2)范围内取值
		m_vSnows[i].x = float(rand() % SNOW_SYSTEM_LENGTH_X - SNOW_SYSTEM_LENGTH_X / 2);
		m_vSnows[i].y = float(rand() % SNOW_SYSTEM_WIDTH_Y - SNOW_SYSTEM_WIDTH_Y / 2);
		m_vSnows[i].z = float(rand() % SNOW_SYSTEM_HEIGHT_Z);

		m_vSnows[i].RotationX = (rand() % 100) / 50.0f*D3DX_PI;
		m_vSnows[i].RotationY = (rand() % 100) / 50.0f*D3DX_PI;

		m_vSnows[i].FallSpeed = 300.0f + rand() % 800;
		m_vSnows[i].RotationSpeed = 5.0f + rand() % 10 / 10.0f;

		m_vSnows[i].TextureIndex = rand() % 6;
	}

	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(VERTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	VERTEX vertices[] =
	{
		{ -30.0f,0.0f,0.0f,0.0f,1.0f },
	    { -30.0f,60.0f,0.0f,0.0f,0.0f },
	    { 30.0f,60.0f,0.0f,1.0f,0.0f },
	    { 30.0f,0.0f,0.0f,1.0f,1.0f }
	};
	VOID* pVertex;
	m_pVertexBuffer->Lock(0, sizeof(vertices), &pVertex, 0);
	memcpy(pVertex, vertices, sizeof(vertices));
	m_pVertexBuffer->Unlock();

	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow1.jpg", &m_pTexture[1]);
	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow2.jpg", &m_pTexture[2]);
	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow3.jpg", &m_pTexture[3]);
	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow4.jpg", &m_pTexture[4]);
	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow5.jpg", &m_pTexture[5]);
	D3DXCreateTextureFromFile(m_pd3dDevice, L"GameMedia\\snow6.jpg", &m_pTexture[6]);

	return S_OK;
}

HRESULT SnowParticleClass::UpdateSnowParticle(float fElapsedTime)
{
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		m_vSnows[i].y -= m_vSnows[i].FallSpeed*fElapsedTime;
		if (m_vSnows[i].y < 0)
			m_vSnows[i].y = SNOW_SYSTEM_WIDTH_Y;
		m_vSnows[i].RotationX += m_vSnows[i].RotationSpeed*fElapsedTime;
		m_vSnows[i].RotationY += m_vSnows[i].RotationSpeed*fElapsedTime;
	}
	return S_OK;
}

HRESULT SnowParticleClass::RenderSnowParticle()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 为什么要设置这些
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		D3DXMATRIX matTrans, matRX, matRY, matWorld;
		D3DXMatrixTranslation(&matTrans, m_vSnows[i].x, m_vSnows[i].y, m_vSnows[i].z);
		D3DXMatrixRotationX(&matRX, m_vSnows[i].RotationX);
		D3DXMatrixRotationY(&matRY, m_vSnows[i].RotationY);
		matWorld = matTrans * matRX*matRY;
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetTexture(0, m_pTexture[m_vSnows[i].TextureIndex]);
		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEX));
		m_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
	return S_OK;
}

SnowParticleClass::~SnowParticleClass()
{
	SAFE_RELEASE(m_pVertexBuffer);
	for (int i = 0; i < 6; i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}
