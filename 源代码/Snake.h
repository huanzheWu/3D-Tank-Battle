#pragma once
#include"d3dUtility.h"
#include<list>
#include"XFileMesh.h"
#include"DirectInputClass.h"

#define SPEEDUP 2
#define ADDBODY 1


//�ߵĽṹ��
struct SNAKE
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	//λ�á�ǰ�����򣨰����һ�ŵķ���������������
};


//ת�۵�ṹ��
struct TURNING
{
	//��Ҫת�۵�λ���Լ���ת����
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
};

class Snake 
{
public :
	Snake(PDIRECT3DDEVICE9 _Device){Device=_Device;};
	~Snake();
	
	private:
		XFileMesh * Psnake;//��ͷ��XFileMesh��ָ��
		LPDIRECT3DDEVICE9 Device;//D3D�豸ָ��
		std::list <TURNING> _allTurning;//����ת�۵���Ϣ������	//��ʲôʱ����Ҫ��������أ�---��ͷת���ʱ��
		std::list<SNAKE> _allSnakeNode;//һ�������������нڵ���Ϣ����������������½ڵ�ʱ�������ڵ��ȡ���һ���ڵ㡣
		IDirect3DTexture9 *tex;
	private:
		void addTurning(D3DXVECTOR3 position,D3DXVECTOR3 direction);//���һ��ת�۵�,��Ҫ��ͷ���������Ϣ
		void addSnake(D3DXVECTOR3 position,D3DXVECTOR3 direction);//���ߵ�β�����һ���ڵ㡣
	public:
		bool initSnake ();//��ʼ������������һ��������ͷ��X�ļ�ģ�ͣ�ָ��ģ�͵ĳ�ʼλ�õȵ�
		void upDateSanke(float timeDelta,DInputClass* Dinput);//���½ڵ��λ���뷽��
		void RenderSnake();//��Ⱦ����
		//********************��һ�˳��Ӿ�����************************
		void getHeadPos(D3DXVECTOR3* position);
};