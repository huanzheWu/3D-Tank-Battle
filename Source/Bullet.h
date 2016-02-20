#pragma once
#include<list>
#include"d3dUtility.h"
#include"CameraClass.h"
#include"XFileMesh.h"
#include"DirectInputClass.h"
//�ӵ�������
struct Bullet
{
		float  _age;//����
		float _lifetime;//����
		bool _alive;//�Ƿ����
		D3DXVECTOR3 _position;//�ӵ�λ��
		D3DXVECTOR3 _speed;//�ӵ����ٶȣ�����
		enum Whos{my,enemy};//˭���ӵ�
};

class BULLET :public XFileMesh{
	private :
		CamerClass * _camera;
		std::list <Bullet>  _allbuttet;
	public :
		BULLET ();
		BULLET(LPDIRECT3DDEVICE9 Device,CamerClass * camera);//��ʼ����ʱ�������������������Ա��ȡ�������ǰλ��
		void resetbullet(Bullet *bullet);//��ʼ��һ�����ӵ��ĳ�ʼ״̬
		void addbullet();
		bool InitBullet ();
		void Update(float timeDeltas);
		void render ();
		void removedead();

};