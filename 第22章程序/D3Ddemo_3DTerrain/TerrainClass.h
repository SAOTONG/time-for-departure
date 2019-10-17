#pragma once
//=================================================================================
// ������TerrainClass.hͷ�ļ�����������TerrainClass
//=================================================================================
#ifndef TERRAINCLASS_H
#define TERRAINCLASS_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include "D3DUtil.h"

// TerrainClass��װ����ά����ϵͳ
// �������Ҫʵ��һ�¼������ܣ�
// ���γ�ʼ��
// ������������͵��ļ���ȡ���εĸ߶���Ϣ�������������ͼƬ
// ������Ⱦ����
class TerrainClass
{
public:
	TerrainClass(IDirect3DDevice9 *pd3dDevice);
	~TerrainClass();
	// ���ļ��м��ظ߶�ͼ��������Ϣ
	bool LoadTerrainFromFile(wchar_t *pFileName, wchar_t *pTextureFile);
	// ���γ�ʼ������
	bool InitTerrain(int nRows, int nColumns, float fSpacing, float fHeightScale);
	// ������Ⱦ����
	bool RenderTerrain(D3DXMATRIX *pMatWorld, bool bDrawFrame = FALSE);
private:
	// ָ�����δ�С������
	int m_nMeshRows;           // ��������
	int m_nMeshColumns;        // ��������
	int m_nMeshRowVertexs;     // �����ж�����
	int m_nMeshColumnVertexs;  // �����ж�����
	int m_nNumVertexs;         // �����ܶ�����
	float m_fTerrainWidth;     // ���ο��
	float m_fTerrainDepth;     // �������
	float m_fMeshSpacing;      // ������
	float m_fHeightScale;      // �߶�����ϵ��

	// �����D3D����
	LPDIRECT3DDEVICE9 m_pd3dDevice;           // Direct3D�豸����
	LPDIRECT3DTEXTURE9 m_pTexture;            // �������
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;    // �����������
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  // ���㻺�����

	// ��Ÿ߶�ͼ�еĸ߶���Ϣ������
	std::vector<float> m_vHeightInfo;

	// ����FVF�����ʽ�Ľṹ��
	struct TERRAINVERTEX
	{
		float _x, _y,_z;
		float _u, _v;
		TERRAINVERTEX(float x,float y,float z,float u,float v):_x(x),_y(y),
			_z(z),_u(u),_v(v){}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};
};

#endif // !TERRAINCLASS_H
