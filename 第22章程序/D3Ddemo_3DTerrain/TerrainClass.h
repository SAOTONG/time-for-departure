#pragma once
//=================================================================================
// 描述：TerrainClass.h头文件，定义了类TerrainClass
//=================================================================================
#ifndef TERRAINCLASS_H
#define TERRAINCLASS_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include "D3DUtil.h"

// TerrainClass封装了三维地形系统
// 这个类主要实现一下几个功能：
// 地形初始化
// 载入二进制类型的文件读取地形的高度信息、载入地形纹理图片
// 地形渲染绘制
class TerrainClass
{
public:
	TerrainClass(IDirect3DDevice9 *pd3dDevice);
	~TerrainClass();
	// 从文件中加载高度图和纹理信息
	bool LoadTerrainFromFile(wchar_t *pFileName, wchar_t *pTextureFile);
	// 地形初始化函数
	bool InitTerrain(int nRows, int nColumns, float fSpacing, float fHeightScale);
	// 地形渲染函数
	bool RenderTerrain(D3DXMATRIX *pMatWorld, bool bDrawFrame = FALSE);
private:
	// 指定地形大小的数据
	int m_nMeshRows;           // 网格行数
	int m_nMeshColumns;        // 网格列数
	int m_nMeshRowVertexs;     // 网格行顶点数
	int m_nMeshColumnVertexs;  // 网格列顶点数
	int m_nNumVertexs;         // 网格总顶点数
	float m_fTerrainWidth;     // 地形宽度
	float m_fTerrainDepth;     // 地形深度
	float m_fMeshSpacing;      // 网格间距
	float m_fHeightScale;      // 高度缩放系数

	// 所需的D3D对象
	LPDIRECT3DDEVICE9 m_pd3dDevice;           // Direct3D设备对象
	LPDIRECT3DTEXTURE9 m_pTexture;            // 纹理对象
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;    // 索引缓存对象
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  // 顶点缓存对象

	// 存放高度图中的高度信息的序列
	std::vector<float> m_vHeightInfo;

	// 地形FVF顶点格式的结构体
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
