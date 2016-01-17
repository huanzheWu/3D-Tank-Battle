#pragma once
#include"d3dUtility.h"
#include"XFileMesh.h"
#include"CameraClass.h"
#include"DirectInputClass.h"
#include"GameProps.h"
//Ѫ���Ķ���ṹ
struct GUIVERTEX1
{
	GUIVERTEX1(){}
	GUIVERTEX1(float _x,float _y,float _z,float _rhw,unsigned long _color,float _tu,float _tv)
	{
		x=_x,y=_y,z=_z,rhw=_rhw,color=_color,tu=_tu,tv=_tv;
	}
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};



class GameProps;
class Tank : public XFileMesh
{
	friend GameProps; //��GameProps����Ϊ��Ԫ
	public:
		Tank (LPDIRECT3DDEVICE9 _Device);
	private :
		D3DXMATRIX matHero;	
		LPDIRECT3DVERTEXBUFFER9 Vb;//���㻺��

		LPDIRECT3DTEXTURE9 Tex[13];//����ӿڣ�10��������10��Ѫ��
		D3DXVECTOR3 tankposition;//̹�˵�λ��
		int blood ;//̹�˵�Ѫ��
	public :
		void getBox (D3DXVECTOR3* min,D3DXVECTOR3* max);
		bool collisiontest (D3DXVECTOR3  Emin);
		void SetTank(CamerClass * camera,DInputClass * DInput);
		void calcublood(int i);
		int getblood (){return blood;};//��ȡ̹��Ѫ��
		void rendblood();
		void setblood (){blood=12;};//�趨̹�˵�Ѫ�� �����ڳԵ�����ʱ���ã�
};
