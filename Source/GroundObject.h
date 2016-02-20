#pragma once
#include"d3dUtility.h"
#include<list>
#include"EnemyTank.h"

class EnemyTank;

struct EXPLODEB
{
	D3DXVECTOR3 _position;//��ը��λ��
	int sign;//dds�ļ����
};
struct BILLBOARD
{
	D3DXVECTOR3 _position;
};
struct WALL
{
	D3DXVECTOR3 _position;
};
class GroundObject:public XFileMesh
{
	friend EnemyTank; //�ѵз�̹������Ϊ��Ԫ����Ϊ�з�̹����Ҫ�ж��Ƿ�ײ����ľ������Χǽ
	private : 
		std::list <EXPLODEB> _allbound;
		LPDIRECT3DVERTEXBUFFER9 Vb;//��������
		LPDIRECT3DTEXTURE9 Tex;//����ӿ�
		D3DXMATRIX MatNPC;//������λ��
		std::list <BILLBOARD> _allBillBoard;//���������
		D3DXMATRIX MatWall;//ǽ��λ��
		std::list<WALL> _allWall;//ǽ������


		//�����Ķ���ṹ
		struct vertex
		{
			vertex(){}
			vertex(float x,float y,float z,float u,float v)
			{
				_x=x;_y=y,_z=z;
				_u=u;_v=v;
			}
			float _x,_y, _z;
			float _u,_v;
			static const DWORD FVF;
		};

	public:
		GroundObject(LPDIRECT3DDEVICE9 dvice);//���캯��
		~GroundObject();//��������
		//========================================================
		//���湫����ຯ��
		//========================================================
		void InitObject(wchar_t *filename,wchar_t *wallname,int numofboard,int numwall);//��ʼ������� 
		void RanderObject(D3DXMATRIX *view);//��Ⱦ����� //����׼����D3DXMATRIX matNPCȥ����ֱ���������λ��
		bool BCollisiontest(D3DXVECTOR3 _position); //�ж�̹������������Ƿ�������ײ��
		bool BTCollisiontest(D3DXVECTOR3 _position );//�ж��ӵ�����������Ƿ�������ײ��//�����������ײ����ͬһ��������ԭ���ǣ��õ���ײ������������ͬ
		void renderbound(D3DXMATRIX *V,float timeDelta);//���Ʊ�ը����
};