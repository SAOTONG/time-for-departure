#include "CameraClass.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800  
#define WINDOW_HEIGHT	600	
#endif // !WINDOW_WIDTH


CameraClass::CameraClass(IDirect3DDevice9 * pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // Ĭ����������X�������غ�
	m_vUpVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);     // Ĭ����������Y�������غ�
	m_vLookVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);   // Ĭ�Ϲ۲�������Z�������غ�
	// Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)
	m_vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	// Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);
	m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
}

CameraClass::~CameraClass()
{
}

// �����ȡ���任����
// ���̣��Ȱѹ��캯�������������������������۲������淶��ʹ���໥��ֱ����Ϊһ����������.
// Ȼ����ʹ����ѧ����ʽ���������ȡ���任��������һ��һ��ΪpMatrix��ֵ
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX * pMatrix)
{	
	// �淶��������
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
	// ʹ����������������ֱ
	D3DXVec3Cross(&m_vUpVector, &m_vRightVector, &m_vLookVector);
	// �淶��������
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	// ʹ�۲���������������ֱ
	D3DXVec3Cross(&m_vLookVector, &m_vUpVector, &m_vRightVector);
	// �淶���۲�����
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);

	// ������ȡ���任����
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
