#pragma once
#include<list>
#include"d3dUtility.h"
#include"CameraClass.h"
#include"XFileMesh.h"
#include"DirectInputClass.h"
#include"GameProps.h"
//子弹的属性
struct EXPLODEA
{
	D3DXVECTOR3 _position;//爆炸出现的位置
	int sign;//标记这个位置的爆炸要用到的dds文件标号

};
struct Bullet
{
		float  _age;//年龄
		float _lifetime;//寿命
		bool _alive;//是否活着
		D3DXVECTOR3 _position;//子弹位置
		D3DXVECTOR3 _speed;//子弹的速度（方向）
		enum Whos{my,enemy};//谁的子弹
};
class BULLET :public XFileMesh{


	private :
		CamerClass * _camera;
		std::list <Bullet>  _allbuttet;
		std::list <EXPLODEA> _allbound;//爆炸位置链表
		
	public :
		BULLET ();
		BULLET(LPDIRECT3DDEVICE9 Device,CamerClass * camera);//初始化的时候把虚拟摄像机传进来以便获取摄像机当前位置
		void resetbullet(Bullet *bullet);//初始化一个新子弹的初始状态
		void addbullet();
		bool InitBullet ();
		void Update(float timeDeltas);
		void render ();
		void removedead();
		void renderbound(D3DXMATRIX *V,float timeDelta);//渲染爆炸效果

};