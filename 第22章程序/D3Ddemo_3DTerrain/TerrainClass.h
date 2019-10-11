#pragma once
//=================================================================================
// 描述：TerrainClass.h头文件，定义了类TerrainClass
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
	int m_nMeshRows;           // 网格行数
	int m_nMeshColumns;        // 网格列数
	int m_nMeshRowVertexs;     // 网格行顶点数
	int m_nMeshColumnVertexs;  // 网格列顶点数
	int m_nNumVertexs;         // 网格总顶点数
	float m_fTerrainWidth;     // 地形宽度
	float m_fTerrainDepth;     // 地形深度
	float m_fMeshSpacing;      // 网格间距
	float M_fHeightScale;      // 高度缩放系数
	// TODO(1053001374@qq.com): Use a "*" here for concatenation operator.
    // TODO(侯金成) change this to use relations.
    // TODO(完善地形类的封装): 继续完善类成员
};

TerrainClass::TerrainClass()
{
}

TerrainClass::~TerrainClass()
{
}
#endif // !TERRAINCLASS_H
