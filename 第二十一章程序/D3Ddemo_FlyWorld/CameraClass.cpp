#include "CameraClass.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800  
#define WINDOW_HEIGHT	600	
#endif // !WINDOW_WIDTH


CameraClass::CameraClass(IDirect3DDevice9 * pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // 默认右向量与X正半轴重合
	m_vUpVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);     // 默认上向量与Y正半轴重合
	m_vLookVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);   // 默认观察向量与Z正半轴重合
	// 默认摄像机坐标为(0.0f, 0.0f, -250.0f)
	m_vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	// 默认观察目标位置为(0.0f, 0.0f, 0.0f);
	m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
}

CameraClass::~CameraClass()
{
}

// 计算出取景变换矩阵
// 过程：先把构造函数给定的右向量、上向量、观察向量规范化使其相互垂直，成为一组正交矩阵.
// 然后按照使用数学矩阵公式计算出来的取景变换矩阵结果，一行一行为pMatrix赋值
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX * pMatrix)
{	
	// 规范化右向量
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
	// 使上向量与右向量垂直
	D3DXVec3Cross(&m_vUpVector, &m_vRightVector, &m_vLookVector);
	// 规范化上向量
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	// 使观察向量与上向量垂直
	D3DXVec3Cross(&m_vLookVector, &m_vUpVector, &m_vRightVector);
	// 规范化观察向量
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);

	// 创建出取景变换矩阵
	pMatrix->_11 = m_vRightVector.x;
	pMatrix->_12 = m_vUpVector.x;
	pMatrix->_13 = m_vLookVector.x;
	pMatrix->_14 = 0.0f;

	pMatrix->_21 = m_vRightVector.y;
	pMatrix->_22 = m_vUpVector.y;
	pMatrix->_23 = m_vLookVector.y;
	pMatrix->_24 = 0.0f;

	pMatrix->_31 = m_vRightVector.z;
	pMatrix->_32 = m_vUpVector.z;
	pMatrix->_33 = m_vLookVector.z;
	pMatrix->_34 = 0.0f;

	pMatrix->_41 = -D3DXVec3Dot(&m_vTargetPosition,&m_vRightVector);
	pMatrix->_42 = -D3DXVec3Dot(&m_vTargetPosition, &m_vUpVector);
	pMatrix->_43 = -D3DXVec3Dot(&m_vTargetPosition, &m_vLookVector);
	pMatrix->_44 = 1.0f;
}

VOID CameraClass::SetTargetPosition(D3DXVECTOR3 * pLookat)
{
	return VOID();
}

VOID CameraClass::SetCameraPosition(D3DXVECTOR3 * pVector)
{
	return VOID();
}

VOID CameraClass::SetViewMatrix(D3DXMATRIX * pMatrix)
{
	return VOID();
}

VOID CameraClass::SetProjMatrix(D3DXMATRIX * pMatrix)
{
	return VOID();
}

VOID CameraClass::MoveAlongRightVec(FLOAT fUnits)
{
	return VOID();
}

VOID CameraClass::MoveAlongUpVec(FLOAT fUnits)
{
	return VOID();
}

VOID CameraClass::MoveAlongLookVec(FLOAT fUnits)
{
	return VOID();
}

VOID CameraClass::RotationRightVec(FLOAT fAngle)
{
	return VOID();
}

VOID CameraClass::RotationUpVec(FLOAT fAngle)
{
	return VOID();
}

VOID CameraClass::RotationLookVec(FLOAT fAngle)
{
	return VOID();
}
