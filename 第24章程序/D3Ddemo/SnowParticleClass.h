#pragma once
#ifndef SNOWPARTICLECLASS_H
#define SNOWPARTICLECLASS_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "D3DUtil.h"

#define PARTICLE_NUMBER 1500
#define SNOW_SYSTEM_LENGTH_X 20000
#define SNOW_SYSTEM_WIDTH_Y 20000
#define SNOW_SYSTEM_HEIGHT_Z 20000

// 雪花粒子的顶点结构
struct VERTEX
{
	int _x, _y, _z;
	int _u, _v;
};
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_TEX1);

// 雪花粒子结构体定义
struct SNOWPARTICLE
{
	float x, y, z;
	float RotationX;
	float RotationY;
	float FallSpeed;
	float RotationSpeed;
	int TextureIndex;
};

// 粒子系统类的定义
class SnowParticleClass
{
public:
	SnowParticleClass(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT InitSnowParticle();
	HRESULT UpdateSnowParticle(float fElapsedTime);
	HRESULT RenderSnowParticle();
	~SnowParticleClass();

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture[6];
	std::vector<SNOWPARTICLE> m_vSnows;
};

#endif // !SNOWPARTICLECLASS_H
