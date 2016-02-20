#pragma once
#include<d3dx9.h>
class CamerClass
{
	
public:
	enum CameraType{air,land};
	CamerClass();
	CamerClass(CameraType cameraType);
	~CamerClass();
private:
		D3DXVECTOR3 position;
		D3DXVECTOR3 up;
		D3DXVECTOR3 look;
		D3DXVECTOR3 right;
		CameraType  cameratype;

public:
		

		//������up��right��look��ת�ĺ���
		void pitch(float angle);
		void roll(float angle);
		void yaw(float angle);

		//����ƽ�ƺ���
		void walk(float units);
		void fly(float units);
		void strafe(float units);

		//��ȡ����������ķ�������
		//ʵ�֣���look��ֵ��look;
		//���ʹ�ã�����������������һ��������
		void  getlook(D3DXVECTOR3 *Look);
		void  getup(D3DXVECTOR3 *Up);
		void  getright(D3DXVECTOR3 *Right);


		void getposition(D3DXVECTOR3 *P);
		void setposition(D3DXVECTOR3 *P);
		void setCamraType(CameraType cameraType);
		
		//���ù۲����,�൱��ԭ����ȡ���任����
		void SetViewMatrix(D3DXMATRIX * view);

};

