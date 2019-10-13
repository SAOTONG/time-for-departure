#pragma once
//=================================================================================
// ������TerrainClass.hͷ�ļ�����������TerrainClass
//=================================================================================
#ifndef TERRAINCLASS_H
#define TERRAINCLASS_H

#include <d3d9.h>
#include <d3dx9.h>

// TerrainClass��װ����ά����ϵͳ
// �������Ҫʵ��һ�¼������ܣ�
// ���γ�ʼ��
// ������������͵��ļ���ȡ���εĸ߶���Ϣ�������������ͼƬ
// ������Ⱦ����
class TerrainClass
{
public:
	TerrainClass();
	~TerrainClass();

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
	float M_fHeightScale;      // �߶�����ϵ��

	// �����D3D����
	LPDIRECT3DDEVICE9 m_pd3dDevice;           // Direct3D�豸����
	LPDIRECT3DTEXTURE9 m_pTexture;            // �������
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;    // �����������
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  // ���㻺�����

	// ��Ÿ߶�ͼ�еĸ߶���Ϣ������

};

TerrainClass::TerrainClass()
{
}

TerrainClass::~TerrainClass()
{
}
#endif // !TERRAINCLASS_H
