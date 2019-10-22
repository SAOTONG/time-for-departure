#pragma once
//=================================================================================
// ������SkyBoxClass.hͷ�ļ�����������SkyBoxClass
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
	BOOL InitSkyBox(float length);  // ��ʼ����պ�
	// ���ļ������������
	BOOL LoadSkyTextureFromFile(const wchar_t *pFrontTextureFile, const wchar_t *pBackTextureFile,
		const wchar_t *pLeftTextureFile, const wchar_t *pRightTextureFile, const wchar_t *pTopTextureFile);
	VOID RenderSkyBox(D3DMATRIX *pMatWorld, BOOL bRenderFrame);
private:
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  // ���㻺��
	LPDIRECT3DTEXTURE9      m_pTextures[5];   // �������
	float                   m_fSideLength;    // ��պб߳�
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
