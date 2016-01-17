#pragma once
#include"d3dUtility.h"
#include"XFileMesh.h"
#include"CameraClass.h"
#include"DirectInputClass.h"
#include"GameProps.h"
//血条的顶点结构
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
	friend GameProps; //将GameProps类设为友元
	public:
		Tank (LPDIRECT3DDEVICE9 _Device);
	private :
		D3DXMATRIX matHero;	
		LPDIRECT3DVERTEXBUFFER9 Vb;//顶点缓存

		LPDIRECT3DTEXTURE9 Tex[13];//纹理接口（10个，保存10条血）
		D3DXVECTOR3 tankposition;//坦克的位置
		int blood ;//坦克的血量
	public :
		void getBox (D3DXVECTOR3* min,D3DXVECTOR3* max);
		bool collisiontest (D3DXVECTOR3  Emin);
		void SetTank(CamerClass * camera,DInputClass * DInput);
		void calcublood(int i);
		int getblood (){return blood;};//获取坦克血量
		void rendblood();
		void setblood (){blood=12;};//设定坦克的血量 （用于吃到道具时调用）
};
