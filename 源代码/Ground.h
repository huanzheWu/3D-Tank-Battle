#pragma once
#include"XFileMesh.h"//网格x文件头文件
#include<list>

class Ground
{
	private :
		LPDIRECT3DDEVICE9 Device;//D3D设备指针
		LPDIRECT3DTEXTURE9 Tex[1];//纹理接口
		std::list <D3DXVECTOR3> _allWalls;//围墙的链表
		XFileMesh *Wall; //围墙的X文件指针
		D3DXMATRIX WallPos;//墙壁的位置矩阵
		D3DXMATRIX WallSca;//墙壁的缩放矩阵
	public:
		Ground (LPDIRECT3DDEVICE9 _Device){Device=_Device;};//构造函数
		~Ground();//析构函数
		bool initObject ();//初始化函数
		bool createWall(D3DXVECTOR3 centre);
		void renderGround ();

};