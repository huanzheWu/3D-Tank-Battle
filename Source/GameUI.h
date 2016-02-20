//
//
//�������Ҫ������������Ϸ���й����еĽ����HUD 
//
//

#pragma once
#include"d3dUtility.h"
#include<list>

struct SCORE
{
	SCORE(){}
	SCORE(float _x,float _y,float _z,float _rhw,unsigned long _color,float _tu,float _tv)
	{
		x=_x,y=_y,z=_z,rhw=_rhw,color=_color,tu=_tu,tv=_tv;
	}
	float x, y, z, rhw;
	unsigned long color;
	float tu, tv;
};

class  GameUI
{
	public:
		GameUI(LPDIRECT3DDEVICE9 _Device){Device=_Device;}//���캯��
		~GameUI();//��������
	private :
		LPDIRECT3DDEVICE9 Device;//D3D�豸ָ��
		LPDIRECT3DVERTEXBUFFER9 Vb;//���㻺��
		LPDIRECT3DTEXTURE9 Tex[1];//����ӿ�
		ID3DXFont *pFont ; //����ӿ�ָ��
		float pictureScal; //ͼƬ�ķŴ���
	public :
		bool initPicture(wchar_t *FileName ,float x , float y,float pictureScal );//��ʼ������
		bool initTex(wchar_t *Fontz);
		void RenderFont(RECT rect ,wchar_t *content);
		void RenderUI();//��Ⱦ����
};