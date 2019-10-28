//=================================================================================
// ������CameraClass.hͷ�ļ�����������CameraClass
//=================================================================================
#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <d3d9.h>
#include <d3dx9.h>

// CameraClass��ʵ�������������ά�ռ��������ƶ���
// ����ĺ���˼����ʹ���ĸ������������ҷ����������Ϸ����������۲����������λ�÷���������
// ��ȷ��һ��������������������ϵ��λ�úͳ���
class CameraClass
{
public:
	// ��ʼ��Direct3D�豸�ӿڶ����ҷ����������Ϸ����������۲����������λ�÷�������
	// �Լ�Ŀ��۲�λ�õ�������
	// 
	// ������pd3dDeviceΪDirect3D�豸�ӿڶ���
	// 
	// ����ֵ���ޡ�
	CameraClass(IDirect3DDevice9* pd3dDevice);

	~CameraClass();

	// һ������ȡ���任����ĺ���
	// 
	// ������pMatrixΪһ���������
	// 
	// ����ֵ���ޡ�
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);  // ����ȡ���任����

	// ���ص�ǰͶӰ����
	// 
	// ������pMatrixΪһ��ָ���������ָ��
	// 
	// ����ֵ���ޡ�
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }

	// ���ص�ǰ�����λ�þ���
	// 
	// ������pVectorΪһ��ָ�����������ָ��
	// 
	// ����ֵ���ޡ�
	VOID GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = m_vCameraPosition; }

	// ���ص�ǰ�Ĺ۲����
	// 
	// ������pVectorΪһ��ָ�����������ָ��
	// 
	// ����ֵ���ޡ�
	VOID GetLookVector(D3DXVECTOR3 *pVector) { *pVector = m_vLookVector; }  
	
	// �����������Ŀ��۲�λ������
	// 
	// ������pLookatΪһ��ָ�����������ָ��
	// 
	// ����ֵ���ޡ�
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	// ������������ڵ�λ������
	// 
	// ������pVectorΪһ��ָ�����������ָ��
	// 
	// ����ֵ���ޡ�
	VOID SetCameraPosition(D3DXVECTOR3 *pVector = NULL);

	// ����ȡ���任����
	// 
	// ������pMatrixΪһ��ָ���������ָ��
	// 
	// ����ֵ���ޡ�
	VOID SetViewMatrix(D3DXMATRIX *pMatrix = NULL);

	// ����ͶӰ�任����
	// 
	// ������pMatrixΪһ��ָ���������ָ��
	// 
	// ����ֵ���ޡ�
	VOID SetProjMatrix(D3DXMATRIX *pMatrix = NULL);

	// �ظ�����ƽ�Ƶ���������
	// 
	// ������fUnitsΪ�������ĳ�������ƶ�����
	// 
	// ����ֵ���ޡ�
	VOID MoveAlongRightVec(FLOAT fUnits);  // ��right�����ƶ�
	VOID MoveAlongUpVec(FLOAT fUnits);     // ��up�����ƶ�
	VOID MoveAlongLookVec(FLOAT fUnits);   // ��look�����ƶ�

	// �Ƹ�������ת����������
	// 
	// ������fAngleΪ�������ĳ��������ת����
	// 
	// ����ֵ���ޡ�
	VOID RotationRightVec(FLOAT fAngle);  // ��right������ת
	VOID RotationUpVec(FLOAT fAngle);     // ��up������ת
	VOID RotationLookVec(FLOAT fAngle);   // ��look������ת

private:
	D3DXVECTOR3 m_vRightVector;  // �ҷ�������
	D3DXVECTOR3 m_vUpVector;     // �Ϸ�������
	D3DXVECTOR3 m_vLookVector;   // �۲췽������
	D3DXVECTOR3 m_vCameraPosition;  // �����λ�õ�����
	D3DXVECTOR3 m_vTargetPosition;  // Ŀ��۲�λ�õ�����
	D3DXMATRIX m_matView;  // ȡ���任����
	D3DXMATRIX m_matProj;  // ͶӰ�任����
	LPDIRECT3DDEVICE9 m_pd3dDevice;  // Direct3D�豸����
};
#endif // !CAMERACLASS_H