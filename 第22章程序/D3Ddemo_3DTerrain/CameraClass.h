//=================================================================================
// 描述：CameraClass.h头文件，定义了类CameraClass
//=================================================================================
#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <d3d9.h>
#include <d3dx9.h>

// CameraClass类实现了摄像机在三维空间中自由移动。
// 该类的核心思想是使用四个分量向量：右分量向量、上分量向量、观察分量向量、位置分量向量，
// 来确定一个摄像机相对于世界坐标系的位置和朝向。
class CameraClass
{
public:
	// 初始化Direct3D设备接口对象、右分量向量、上分量向量、观察分量向量、位置分量向量
	// 以及目标观察位置的向量。
	// 
	// 参数：pd3dDevice为Direct3D设备接口对象
	// 
	// 返回值：无。
	CameraClass(IDirect3DDevice9* pd3dDevice);

	~CameraClass();

	// 一个计算取景变换矩阵的函数
	// 
	// 参数：pMatrix为一个矩阵对象
	// 
	// 返回值：无。
	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);  // 计算取景变换矩阵

	// 返回当前投影矩阵
	// 
	// 参数：pMatrix为一个指向矩阵对象的指针
	// 
	// 返回值：无。
	VOID GetProjMatrix(D3DXMATRIX *pMatrix) { *pMatrix = m_matProj; }

	// 返回当前摄像机位置矩阵
	// 
	// 参数：pVector为一个指向向量对象的指针
	// 
	// 返回值：无。
	VOID GetCameraPosition(D3DXVECTOR3 *pVector) { *pVector = m_vCameraPosition; }

	// 返回当前的观察矩阵
	// 
	// 参数：pVector为一个指向向量对象的指针
	// 
	// 返回值：无。
	VOID GetLookVector(D3DXVECTOR3 *pVector) { *pVector = m_vLookVector; }  
	
	// 设置摄像机的目标观察位置向量
	// 
	// 参数：pLookat为一个指向向量对象的指针
	// 
	// 返回值：无。
	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);

	// 设置摄像机所在的位置向量
	// 
	// 参数：pVector为一个指向向量对象的指针
	// 
	// 返回值：无。
	VOID SetCameraPosition(D3DXVECTOR3 *pVector = NULL);

	// 设置取景变换矩阵
	// 
	// 参数：pMatrix为一个指向矩阵对象的指针
	// 
	// 返回值：无。
	VOID SetViewMatrix(D3DXMATRIX *pMatrix = NULL);

	// 设置投影变换矩阵
	// 
	// 参数：pMatrix为一个指向矩阵对象的指针
	// 
	// 返回值：无。
	VOID SetProjMatrix(D3DXMATRIX *pMatrix = NULL);

	// 沿各分量平移的三个函数
	// 
	// 参数：fUnits为摄像机沿某个向量移动的量
	// 
	// 返回值：无。
	VOID MoveAlongRightVec(FLOAT fUnits);  // 沿right向量移动
	VOID MoveAlongUpVec(FLOAT fUnits);     // 沿up向量移动
	VOID MoveAlongLookVec(FLOAT fUnits);   // 沿look向量移动

	// 绕各分量旋转的三个函数
	// 
	// 参数：fAngle为摄像机沿某个向量旋转的量
	// 
	// 返回值：无。
	VOID RotationRightVec(FLOAT fAngle);  // 绕right向量旋转
	VOID RotationUpVec(FLOAT fAngle);     // 绕up向量旋转
	VOID RotationLookVec(FLOAT fAngle);   // 绕look向量旋转

private:
	D3DXVECTOR3 m_vRightVector;  // 右分量向量
	D3DXVECTOR3 m_vUpVector;     // 上分量向量
	D3DXVECTOR3 m_vLookVector;   // 观察方向向量
	D3DXVECTOR3 m_vCameraPosition;  // 摄像机位置的向量
	D3DXVECTOR3 m_vTargetPosition;  // 目标观察位置的向量
	D3DXMATRIX m_matView;  // 取景变换矩阵
	D3DXMATRIX m_matProj;  // 投影变换矩阵
	LPDIRECT3DDEVICE9 m_pd3dDevice;  // Direct3D设备对象
};
#endif // !CAMERACLASS_H