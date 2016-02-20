#include"CameraClass.h"                                    

CamerClass::CamerClass()
{
	cameratype = air;
	position   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

CamerClass::CamerClass(CameraType cameraType)
{
	cameraType = cameraType;

	position   = D3DXVECTOR3(0.0f, 200.0f, -480.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}
void CamerClass::SetViewMatrix(D3DXMATRIX *view)
{
	//ʹ��������Ϸ������۲�������ҷ�����׼������
	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&up, &look, &right);
	D3DXVec3Normalize(&up, &up);

	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);
	 
	//����λ�÷������������ַ����ĵ�˽��
	float pr=-D3DXVec3Dot(&position,&right);
	float pu=-D3DXVec3Dot(&position,&up);
	float pl=-D3DXVec3Dot(&position,&look);

	//��ʼ���ȡ�������Ԫ��
	(*view)(0,0)=right.x;  (*view)(0,1)=up.x;  (*view)(0,2)=look.x;  (*view)(0,3)=0.0f;
	(*view)(1,0)=right.y;  (*view)(1,1)=up.y;  (*view)(1,2)=look.y;  (*view)(1,3)=0.0f;
	(*view)(2,0)=right.z;  (*view)(2,1)=up.z;  (*view)(2,2)=look.z;  (*view)(2,3)=0.0f;
	(*view)(3,0)=pr;	   (*view)(3,1)=pu;    (*view)(3,2)=pl;      (*view)(3,3)=1.0f;
}
//===============================================================================================================
//������ת����
//===============================================================================================================

void CamerClass::pitch(float angle)
{
	D3DXMATRIX temp; //����һ��������������ת�����Ϣ
	D3DXMatrixRotationAxis(&temp,&right,angle);//������������ת
	//��������������������
	D3DXVec3TransformCoord(&up,&up,&temp);
	D3DXVec3TransformCoord(&look,&look,&temp);
	//D3DXVec3TransformCoord(&position,&position,&temp);//���Ź۲����ת
}
void CamerClass::roll(float angle)
{
	D3DXMATRIX temp;
	D3DXMatrixRotationAxis(&temp,&look,angle);

	D3DXVec3TransformCoord(&up,&up,&temp);
	D3DXVec3TransformCoord(&right,&right,&temp);
}
void CamerClass::yaw(float angle)
{
	D3DXMATRIX temp;
	D3DXMatrixRotationAxis(&temp,&up,angle);
	D3DXVec3TransformCoord(&right,&right,&temp);
	D3DXVec3TransformCoord(&look,&look,&temp);
	//D3DXVec3TransformCoord(&position,&position,&temp);
}
//==================================================================================================================
//����ƽ�ƺ���
//==================================================================================================================
void CamerClass::fly(float units)
{
	if(cameratype==air)
		position+=up*units;
	else if(cameratype==land)
		position.y +=units;
}
void CamerClass::strafe(float units)
{
	if(cameratype==air)
	{
		position+=right*units;
	}
	else if(cameratype==land)
	{
		position+=D3DXVECTOR3(right.x,0.0f,right.z)*units;
	}
}
void CamerClass::walk(float units)
{
	if(cameratype==air)
		position+=look*units;
	else if(cameratype==land)
		position+=D3DXVECTOR3(look.x,0.0f,look.z)*units;
}

//===================================================================
//����get����
//===================================================================
void CamerClass::getlook(D3DXVECTOR3 *Look)
{
	*Look=look;
}
void CamerClass::getright(D3DXVECTOR3 *Right)
{
	*Right=right;
}
void CamerClass::getup(D3DXVECTOR3 *Up)
{
	*Up=up;
}
//===============================================
void CamerClass::getposition(D3DXVECTOR3 *Position)
{
	*Position=position;
}


void CamerClass::setposition(D3DXVECTOR3 *Position)
{
	position=*Position;
}

void CamerClass::setCamraType (CameraType cameraType)
{
	cameratype=cameraType;
}
//��������

CamerClass::~CamerClass()
{

}
