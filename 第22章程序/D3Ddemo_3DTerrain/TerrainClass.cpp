#include "TerrainClass.h"

// 初始化各成员变量
TerrainClass::TerrainClass(IDirect3DDevice9 * pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pIndexBuffer = NULL;
	m_pTexture = NULL;
	m_pVertexBuffer = NULL;
	m_nMeshColumns = 0;
	m_nMeshColumnVertexs = 0;
	m_nMeshRows = 0;
	m_nMeshRowVertexs = 0;
	m_nNumVertexs = 0;
	m_fHeightScale = 0.0f;
	m_fMeshSpacing = 0.0f;
	m_fTerrainDepth = 0.0f;
	m_fTerrainWidth = 0.0f;
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::LoadTerrainFromFile(wchar_t * pFileName, wchar_t * pTextureFile)
{
	std::ifstream inFile;
	inFile.open(pFileName, std::ios::binary);
	inFile.seekg(0, std::ios::end);
	std::vector<byte> inData(inFile.tellg());
	inFile.seekg(std::ios::beg);
	inFile.read((char*)&inData[0], inData.size());
	inFile.close();
	m_vHeightInfo.resize(inData.size());
	for (int i = 0; i < inData.size(); i++)
	{
		m_vHeightInfo[i] = inData[i];
	}
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, pTextureFile, &m_pTexture)))
		return false;
	return true;
}

bool TerrainClass::InitTerrain(int nRows, int nColumns, float fSpacing, float fHeightScale)
{
	m_nMeshRows = nRows;
	m_nMeshColumns = nColumns;
	m_fMeshSpacing = fSpacing;
	m_fHeightScale = fHeightScale;
	m_fTerrainWidth = nRows * fSpacing;
	m_fTerrainDepth = nColumns * fSpacing;
	m_nMeshColumnVertexs = nColumns + 1;
	m_nMeshRowVertexs = nRows + 1;
	m_nNumVertexs = m_nMeshColumnVertexs * m_nMeshRowVertexs;

	for (auto a : m_vHeightInfo)
		a = a * m_fHeightScale;
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_nNumVertexs * sizeof(TERRAINVERTEX),
		D3DUSAGE_WRITEONLY, TERRAINVERTEX::FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, 0)))
		return false;
	TERRAINVERTEX *pVertex = NULL;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
	float fStartX = -m_fTerrainWidth / 2, fEndX = m_fTerrainWidth / 2;
	float fStartZ = m_fTerrainDepth / 2, fEndZ = -m_fTerrainDepth / 2;
	float fCoordU = 3.0f / (float)m_nMeshRows;     // 指定纹理的横坐标
	float fCoordV = 3.0f / (float)m_nMeshColumns;  // 指定纹理的纵坐标
	int nIndex = 0, i = 0, j = 0;
	for (float z = fStartZ; z >fEndZ; z-=m_fMeshSpacing)
	{
		j = 0;
		for (float x = fStartX; x < fEndX; x+=m_fMeshSpacing,j++)
		{
			nIndex = i * m_nMeshRows + j;  // 指定当前顶点在顶点缓存中的位置
			pVertex[nIndex] = TERRAINVERTEX(x, m_vHeightInfo[nIndex], z, 
				j*fCoordU, i*fCoordV);
			nIndex++;
		}
	}

	//4，解锁
	m_pVertexBuffer->Unlock();

	//---------------------------------------------------------------
	// 处理地形的索引
	//---------------------------------------------------------------
	//1.创建索引缓存
	if (FAILED(m_pd3dDevice->CreateIndexBuffer(m_nNumVertexs * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, 0)))
		return FALSE;
	//2.加锁
	WORD* pIndices = NULL;
	m_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	//3.访问，赋值
	nIndex = 0;
	for (int row = 0; row < m_nMeshRows - 1; row++)  // 遍历每行
	{
		for (int col = 0; col < m_nMeshColumns - 1; col++)  // 遍历每列
		{
			// 三角形ABC的三个顶点
			pIndices[nIndex] = row * m_nMeshRows + col;			   // 顶点A
			pIndices[nIndex + 1] = row * m_nMeshRows + col + 1;    // 顶点B
			pIndices[nIndex + 2] = (row + 1) * m_nMeshRows + col;  // 顶点C
			// 三角形CBD的三个顶点
			pIndices[nIndex + 3] = (row + 1) * m_nMeshRows + col;      // 顶点C
			pIndices[nIndex + 4] = row * m_nMeshRows + col + 1;	       // 顶点B
			pIndices[nIndex + 5] = (row + 1) * m_nMeshRows + col + 1;  // 顶点D
			// 处理完一个单元格，索引加上6
			nIndex += 6;  // 索引自加6
		}
	}
	//4、解锁
	m_pIndexBuffer->Unlock();

	return true;
}

bool TerrainClass::RenderTerrain(D3DXMATRIX * pMatWorld, bool bDrawFrame)
{
	return false;
}
