#include "CameraClass.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800  
#define WINDOW_HEIGHT	600	
#endif // !WINDOW_WIDTH
	

CameraClass::CameraClass(IDirect3DDevice9 * pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // 默认右向量与X正半轴重合
	m_vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);     // 默认上向量与Y正半轴重合
	m_vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);   // 默认观察向量与Z正半轴重合
	// 默认摄像机坐标为(0.0f, 0.0f, -250.0f)
	m_vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	// 默认观察目标位置为(0.0f, 0.0f, 0.0f);
	m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
}

CameraClass::~CameraClass()
{
}

// 计算出取景变换矩阵
// 过程：先把给定的右向量、上向量、观察向量规范化使其相互垂直，成为一组正交矩阵.
// 然后按照使用数学矩阵公式计算出来的取景变换矩阵结果，一行一行为pMatrix赋值
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX * pMatrix)
{	
	// 把3个向量都规范化并使其相互垂直，成为一组正交矩阵
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);             // 规范化观察分量
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);  // 上向量与观察向量垂直
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);                 // 规范化上向量
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);  // 右向量与上向量垂直
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);           // 规范化右向量

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

	pMatrix->_41 = -D3DXVec3Dot(&m_vRightVector, &m_vCameraPosition);  // -P*R
	pMatrix->_42 = -D3DXVec3Dot(&m_vUpVector, &m_vCameraPosition);     // -P*U
	pMatrix->_43 = -D3DXVec3Dot(&m_vLookVector, &m_vCameraPosition);   // -P*L
	pMatrix->_44 = 1.0f;
}

// 设置摄像机的观察位置
VOID CameraClass::SetTargetPosition(D3DXVECTOR3 * pLookat)
{
	// 进行指针参数是否为空检查
	if (pLookat != NULL) m_vTargetPosition = *pLookat;
	else m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// 观察点位置减摄像机位置得到观察方向向量
	m_vLookVector = m_vTargetPosition - m_vCameraPosition;

	// 使观察向量、上向量、右向量规范化并相互垂直，得到正交矩阵
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
}

// 设置摄像机位置
VOID CameraClass::SetCameraPosition(D3DXVECTOR3 * pVector)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	m_vCameraPosition = pVector ? *pVector : vec;
}

// 设置投影变换矩阵
VOID CameraClass::SetViewMatrix(D3DXMATRIX * pMatrix)
{
	// 进行指针参数检查
	if (pMatrix) m_matView = *pMatrix;
	else CalculateViewMatrix(&m_matView);

	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// 利用投影变换矩阵为右向量、上向量、观察向量赋值
	m_vRightVector = D3DXVECTOR3(m_matView._11, m_matView._12, m_matView._13);
	m_vUpVector = D3DXVECTOR3(m_matView._21, m_matView._22, m_matView._23);
	m_vLookVector = D3DXVECTOR3(m_matView._31, m_matView._32, m_matView._33);
}

// 设置投影变换矩阵
VOID CameraClass::SetProjMatrix(D3DXMATRIX * pMatrix)
{
	// 进行指针参数检查
	if (pMatrix) m_matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, 
		/*长宽比*/(float)((double)WINDOW_WIDTH / WINDOW_HEIGHT),
		/*视截体近景*/1.0f, /*视截体远景*/300000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

// 摄像机位置和观察点位置沿右向量平移fUnits个单位,
VOID CameraClass::MoveAlongRightVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition += m_vRightVector * fUnits;
}

// 摄像机位置和观察点位置沿上向量平移fUnits个单位
VOID CameraClass::MoveAlongUpVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition += m_vUpVector * fUnits;
}

// 摄像机位置和观察点位置沿观察向量平移fUnits个单位
VOID CameraClass::MoveAlongLookVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;
}

// 沿右向量旋转fAngle个单位角度
VOID CameraClass::RotationRightVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRightVector, fAngle);
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	// 更新观察点位置,观察向量*模=观察点位置
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}

// 沿上向量旋转fAngle个单位角度
VOID CameraClass::RotationUpVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vUpVector, fAngle);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	// 更新观察点位置,观察向量*模=观察点位置
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}

// 沿观察向量旋转fAngle个单位角度
VOID CameraClass::RotationLookVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vLookVector, fAngle);
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);

	// 更新观察点位置,观察向量*模=观察点位置
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}
