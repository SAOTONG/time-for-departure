#pragma once
//=================================================================================
// ������TerrainClass.hͷ�ļ�����������TerrainClass
//=================================================================================
#ifndef TERRAINCLASS_H
#define TERRAINCLASS_H

#include <d3d9.h>
#include <d3dx9.h>

class TerrainClass
{
public:
	TerrainClass();
	~TerrainClass();

private:
	int m_nMeshRows;           // ��������
	int m_nMeshColumns;        // ��������
	int m_nMeshRowVertexs;     // �����ж�����
	int m_nMeshColumnVertexs;  // �����ж�����
	int m_nNumVertexs;         // �����ܶ�����
	float m_fTerrainWidth;     // ���ο��
	float m_fTerrainDepth;     // �������
	float m_fMeshSpacing;      // ������
	float M_fHeightScale;      // �߶�����ϵ��
	// TODO(1053001374@qq.com): Use a "*" here for concatenation operator.
    // TODO(����) change this to use relations.
    // TODO(���Ƶ�����ķ�װ): �����������Ա
};

TerrainClass::TerrainClass()
{
}

TerrainClass::~TerrainClass()
{
}
#endif // !TERRAINCLASS_H
