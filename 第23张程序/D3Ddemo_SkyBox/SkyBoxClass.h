#pragma once
//=================================================================================
// 描述：SkyBoxClass.h头文件，定义了类SkyBoxClass
//=================================================================================
#ifndef SKYBOXCLASS_H
#define SKYBOXCLASS_H

#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"

class SkyBoxClass
{
public:
	SkyBoxClass(LPDIRECT3DDEVICE9 pd3dDevice);
	~SkyBoxClass();
	BOOL InitSkyBox(float length);  // 初始化天空盒
	// 从文件加载五个纹理
	BOOL LoadSkyTextureFromFile(const wchar_t *pFrontTextureFile, const wchar_t *pBackTextureFile,
		const wchar_t *pLeftTextureFile, const wchar_t *pRightTextureFile, const wchar_t *pTopTextureFile);
	VOID RenderSkyBox(D3DMATRIX *pMatWorld, BOOL bRenderFrame);
private:
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  // 顶点缓存
	LPDIRECT3DTEXTURE9      m_pTextures[5];   // 纹理对象
	float                   m_fSideLength;    // 天空盒边长
	struct SKYBOXVERTEX
	{
		float _x, _y, _z;
		float _u, _v;
		SKYBOXVERTEX(float x,float y,float z,float u,float v):_x(x),_y(y),_z(z),
			_u(u),_v(v){}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};
};
#endif // !SKYBOXCLASS_H
