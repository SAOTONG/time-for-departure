#include "CameraClass.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800  
#define WINDOW_HEIGHT	600	
#endif // !WINDOW_WIDTH
	

CameraClass::CameraClass(IDirect3DDevice9 * pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);  // Ĭ����������X�������غ�
	m_vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);     // Ĭ����������Y�������غ�
	m_vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);   // Ĭ�Ϲ۲�������Z�������غ�
	// Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)
	m_vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	// Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);
	m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
}

CameraClass::~CameraClass()
{
}

// �����ȡ���任����
// ���̣��ȰѸ����������������������۲������淶��ʹ���໥��ֱ����Ϊһ����������.
// Ȼ����ʹ����ѧ����ʽ���������ȡ���任��������һ��һ��ΪpMatrix��ֵ
VOID CameraClass::CalculateViewMatrix(D3DXMATRIX * pMatrix)
{	
	// ��3���������淶����ʹ���໥��ֱ����Ϊһ����������
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);             // �淶���۲����
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);  // ��������۲�������ֱ
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);                 // �淶��������
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);  // ����������������ֱ
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);           // �淶��������

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

	pMatrix->_41 = -D3DXVec3Dot(&m_vRightVector, &m_vCameraPosition);  // -P*R
	pMatrix->_42 = -D3DXVec3Dot(&m_vUpVector, &m_vCameraPosition);     // -P*U
	pMatrix->_43 = -D3DXVec3Dot(&m_vLookVector, &m_vCameraPosition);   // -P*L
	pMatrix->_44 = 1.0f;
}

// ����������Ĺ۲�λ��
VOID CameraClass::SetTargetPosition(D3DXVECTOR3 * pLookat)
{
	// ����ָ������Ƿ�Ϊ�ռ��
	if (pLookat != NULL) m_vTargetPosition = *pLookat;
	else m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// �۲��λ�ü������λ�õõ��۲췽������
	m_vLookVector = m_vTargetPosition - m_vCameraPosition;

	// ʹ�۲����������������������淶�����໥��ֱ���õ���������
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
}

// ���������λ��
VOID CameraClass::SetCameraPosition(D3DXVECTOR3 * pVector)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	m_vCameraPosition = pVector ? *pVector : vec;
}

// ����ͶӰ�任����
VOID CameraClass::SetViewMatrix(D3DXMATRIX * pMatrix)
{
	// ����ָ��������
	if (pMatrix) m_matView = *pMatrix;
	else CalculateViewMatrix(&m_matView);

	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// ����ͶӰ�任����Ϊ�����������������۲�������ֵ
	m_vRightVector = D3DXVECTOR3(m_matView._11, m_matView._12, m_matView._13);
	m_vUpVector = D3DXVECTOR3(m_matView._21, m_matView._22, m_matView._23);
	m_vLookVector = D3DXVECTOR3(m_matView._31, m_matView._32, m_matView._33);
}

// ����ͶӰ�任����
VOID CameraClass::SetProjMatrix(D3DXMATRIX * pMatrix)
{
	// ����ָ��������
	if (pMatrix) m_matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, 
		/*�����*/(float)((double)WINDOW_WIDTH / WINDOW_HEIGHT),
		/*�ӽ������*/1.0f, /*�ӽ���Զ��*/300000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

// �����λ�ú͹۲��λ����������ƽ��fUnits����λ,
VOID CameraClass::MoveAlongRightVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition += m_vRightVector * fUnits;
}

// �����λ�ú͹۲��λ����������ƽ��fUnits����λ
VOID CameraClass::MoveAlongUpVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition += m_vUpVector * fUnits;
}

// �����λ�ú͹۲��λ���ع۲�����ƽ��fUnits����λ
VOID CameraClass::MoveAlongLookVec(FLOAT fUnits)
{
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition += m_vLookVector * fUnits;
}

// ����������תfAngle����λ�Ƕ�
VOID CameraClass::RotationRightVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRightVector, fAngle);
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	// ���¹۲��λ��,�۲�����*ģ=�۲��λ��
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}

// ����������תfAngle����λ�Ƕ�
VOID CameraClass::RotationUpVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vUpVector, fAngle);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	// ���¹۲��λ��,�۲�����*ģ=�۲��λ��
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}

// �ع۲�������תfAngle����λ�Ƕ�
VOID CameraClass::RotationLookVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vLookVector, fAngle);
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);

	// ���¹۲��λ��,�۲�����*ģ=�۲��λ��
	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}
