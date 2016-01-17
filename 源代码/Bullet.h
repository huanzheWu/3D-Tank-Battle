#pragma once
#include<list>
#include"d3dUtility.h"
#include"CameraClass.h"
#include"XFileMesh.h"
#include"DirectInputClass.h"
#include"GameProps.h"
//�ӵ�������
struct EXPLODEA
{
	D3DXVECTOR3 _position;//��ը���ֵ�λ��
	int sign;//������λ�õı�ըҪ�õ���dds�ļ����

};
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
		std::list <EXPLODEA> _allbound;//��ըλ������
		
	public :
		BULLET ();
		BULLET(LPDIRECT3DDEVICE9 Device,CamerClass * camera);//��ʼ����ʱ�������������������Ա��ȡ�������ǰλ��
		void resetbullet(Bullet *bullet);//��ʼ��һ�����ӵ��ĳ�ʼ״̬
		void addbullet();
		bool InitBullet ();
		void Update(float timeDeltas);
		void render ();
		void removedead();
		void renderbound(D3DXMATRIX *V,float timeDelta);//��Ⱦ��ըЧ��

};