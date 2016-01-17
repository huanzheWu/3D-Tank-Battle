#pragma once
#include"d3dUtility.h"
#include<list>
#include"XFileMesh.h"
#include"DirectInputClass.h"

#define SPEEDUP 2
#define ADDBODY 1


//蛇的结构体
struct SNAKE
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	//位置、前进方向（把最后一颗的方向向量赋给它）
};


//转折点结构体
struct TURNING
{
	//需要转折点位置以及所转方向；
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
};

class Snake 
{
public :
	Snake(PDIRECT3DDEVICE9 _Device){Device=_Device;};
	~Snake();
	
	private:
		XFileMesh * Psnake;//蛇头的XFileMesh类指针
		LPDIRECT3DDEVICE9 Device;//D3D设备指针
		std::list <TURNING> _allTurning;//保存转折点信息的链表	//那什么时候需要添加链表呢？---蛇头转向的时候
		std::list<SNAKE> _allSnakeNode;//一条保存了蛇所有节点信息的链表；方便在添加新节点时，遍历节点获取最后一个节点。
		IDirect3DTexture9 *tex;
	private:
		void addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction);//添加一个转折点,需要蛇头传入相关信息
		void addSnake(D3DXVECTOR3 position,D3DXVECTOR3 direction);//在蛇的尾部添加一个节点。
	public:
		bool initSnake ();//初始化函数，在这一步创建蛇头的X文件模型，指定模型的初始位置等等
		void upDateSanke(float timeDelta,DInputClass* Dinput);//更新节点的位置与方向
		void RenderSnake();//渲染蛇身
		//********************第一人称视觉试验************************
		void getHeadPos(D3DXVECTOR3* position);
};